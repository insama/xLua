
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdint.h>
extern "C"{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"


class LuaRandom;

typedef int (LuaRandom::*cmtype)(lua_State* L);

#define METHOD(CLASS, METHODNAME) {#METHODNAME, &CLASS::METHODNAME}

static int _random(unsigned int &seed)
{

    static const uint32_t M = 2147483647L;   // 2^31-1
    static const uint64_t A = 16807;  // bits 14, 8, 7, 5, 2, 1, 0
    // We are computing
    //       seed_ = (seed_ * A) % M,    where M = 2^31-1
    //
    // seed_ must not be zero or M, or else all subsequent computed values
    // will be zero or M respectively.  For all other values, seed_ will end
    // up cycling through every number in [1,M-1]
    uint64_t product = seed * A;

    // Compute (product % M) using the fact that ((x << 31) % M) == x.
    seed = static_cast<uint32_t>((product >> 31) + (product & M));
    // The first reduction may overflow by 1 bit, so we may need to
    // repeat.  mod == M is not possible; using > allows the faster
    // sign-bit-based test.
    if (seed > M) {
        seed -= M;
    }
    return seed;
}
static char ClassName[0x20] = {"Lua-Random"};	

class LuaRandom {
public:
	struct methods{
		const char* name;
		cmtype method;
	};
	
public:
	static int Create(lua_State* L) {
		LuaRandom** pobj;
		
		static struct LuaRandom::methods methodtable[] = {
			METHOD(LuaRandom, Random),
			METHOD(LuaRandom, SetName),
			METHOD(LuaRandom, GetName),
            METHOD(LuaRandom, SetSeed),
			{NULL, NULL}
		};
		
		//参数检测
		if (lua_gettop(L) != 2){
	
			lua_pushstring(L, "parameter error");
			lua_error(L);
			return 0;	 //返回nil
        }

		
		const char* name = luaL_checkstring(L, 1);	 //第一个实参
		int seed = luaL_checkinteger(L, 2);			 //第二个实参
		
		//清空当前栈
		lua_settop(L, 0);
		
		pobj = (LuaRandom**)lua_newuserdata(L, sizeof(LuaRandom**));
		if (pobj == NULL){
			lua_pushstring(L, "mem allocation fail");
			lua_error(L);
			return 0;	 //空间分配错误
			}
		try {
			*pobj = new LuaRandom(name, seed);
		} catch (...) {
			lua_pushstring(L, "failed instantiation");
			lua_error(L);
			return 0;
        }
		
		//userdata在栈中的位置
		int userdata_index = lua_gettop(L);
		
		//这回我们也来自己实现一手函数表
		//我们把对应的成员函数地址存进一个结构体数组里
		//把结构体作为上值，当我们在lua中调用时，就可以通过thunk调用到相应的函数啦
		
		//老规矩，先判断全局表中是否已经存在这张函数表
		lua_getglobal(L, ClassName);
		
		int method_index = lua_gettop(L);
		
		if (lua_type(L, method_index) == LUA_TNIL) {
			
			//创建一张用来存放函数的表
			lua_newtable(L);
			//函数表在栈中的位置
			method_index = lua_gettop(L); 

			//这里和官方的例子的做法一模一样
			for (struct LuaRandom::methods* p = ( LuaRandom::methods*)methodtable;
				(p->name) && (p->method); p++) {
			
				lua_pushstring(L, p->name);
				lua_pushlightuserdata(L, p);
				lua_pushcclosure(L, LuaRandom::Thunk, 1);
				lua_settable(L, method_index);
			}

			//主动回收
			lua_pushliteral(L, "destroy");
			lua_pushcfunction(L, LuaRandom::destroy);
			lua_settable(L, method_index);
		}
		
		
		if (luaL_newmetatable(L, ClassName) != 0) {
			if (luaL_getmetatable(L, ClassName) == 0)	 //获取出错
				return 1;	 //nil被压栈，直接返回
		}
		
		//元表在栈中的索引
		int mtable_index = lua_gettop(L);
		
		//下面的步骤就跟上面的之前的例子中一模一样
		//设置元表的__index
		lua_pushliteral(L, "__index");
		//先拷贝一份methods表
		lua_pushvalue(L, method_index);
		//添加到元表中
		lua_settable(L, mtable_index);
		
		//我们还可以保护好元表（隐藏起来）
		lua_pushliteral(L, "__metatable");
		lua_pushstring(L, "hidden metatable");
		lua_settable(L, mtable_index);
		
		//接下来就是垃圾回收问题了
		//我们得考虑的是：垃圾回收的控制权在谁的手里。
		//我们将两种都加入到我们的表中
		
		//被动回收
		lua_pushliteral(L, "__gc");
		lua_pushcfunction(L, LuaRandom::destroy);
		lua_settable(L, mtable_index);
		
		//设置弱引用
		lua_pushliteral(L, "__mode");
		lua_pushliteral(L, "kv");
		lua_settable(L, mtable_index);
		
		//最后，把元表与userdata绑定
		lua_pushvalue(L, mtable_index);
		lua_setmetatable(L, userdata_index);
		
		//降低栈，让userdata位于栈顶
		lua_settop(L, userdata_index);
		
		return 1;
	}
	
	//这是调用函数的核心
	static int Thunk(lua_State* L) {
		LuaRandom* o;
		
		//确保万无一失
		if (LuaRandom::CheckPointer(L, &o) == false){
			lua_pushstring(L, "check pointer failed");
			lua_error(L);
    			return 0;
        }
		
		struct LuaRandom::methods* p = (LuaRandom::methods*)lua_touserdata(L, lua_upvalueindex(1));
		
		if (!p) {
			lua_pushstring(L, "methods table error");
			lua_error(L);
			return 0;
		}
		
		//把栈底的userdata删了，因为return那调用的函数中可以使用this指针
		lua_remove(L, 1);
		//调用成员函数
		return (o->*(p->method))(L);
	}
	
	static bool CheckPointer(lua_State* L, LuaRandom** po)
	{
		LuaRandom** pobj;
		
		//检查是否有参数
		if (lua_gettop(L) == 0) {
			lua_pushstring(L, "parameter error");
			lua_error(L);
			return false;
		}
		
		//检查第一个参数是否是userdata
		if (lua_type(L, 1) != LUA_TUSERDATA) {
			lua_pushstring(L, "parameter error");
			lua_error(L);
			return false;
		}
		
		//取出userdata
		pobj = (LuaRandom**)lua_touserdata(L, 1);
		
		//判断用来装对象指针的那块内存是否已经被回收
		if (pobj != NULL) {
			//判断对象是否已经被销毁
			if ((*po = *pobj) == NULL){
			    lua_pushstring(L, "LuaRandom destroyed");
			    lua_error(L);

			}
			return true;
		} else {
			lua_pushstring(L, "mem released");
			lua_error(L);
			return false;
		}
	}
	
	static int destroy(lua_State* L) {
		LuaRandom** pobj;
		
		if (lua_gettop(L) == 0) {
			    lua_pushstring(L, "parameter error");
			    lua_error(L);
		} else {
			if (lua_type(L, 1) == LUA_TUSERDATA) {
				pobj = (LuaRandom**)lua_touserdata(L, 1);
				if (pobj != NULL) {
					if (*pobj != NULL) {
						delete *pobj;
						*pobj = NULL;
					}
				}
			}
		}
		
		return 0;
	}
	
	~LuaRandom(void){
		delete this->name;
	};
	
	//////////////////////////////////////////////////////
	//下面这些类成员方法直接绑定到Lua中
	int Random(lua_State* L) {
		int32_t from    = (int32_t)luaL_checkinteger(L, 1);
        int32_t to  = (int32_t)luaL_checkinteger(L, 2);
        int val = _random(this->seed) % (to - from + 1) + from;
        lua_pushinteger(L, val);
		return 1;
	}
    int SetSeed(lua_State* L){
        unsigned int seed    = (unsigned int)luaL_checkinteger(L, 1);
        this->seed = seed;
        return 1;
    }
    int GetName(lua_State* L) {
		lua_pushstring(L, this->name->c_str());
		return 1;
	}
	int SetName(lua_State* L) {
		const char* name = luaL_checkstring(L, 1);
		delete this->name;
		this->name = new std::string(name);
		return 1;
	}
	////////////////////////////////////////////////////////
private:
	unsigned int seed;
	std::string* name;
	
	LuaRandom(const char* name, unsigned int s):seed(s){
		this->name = new std::string(name);
	};
	LuaRandom(void){}
};


static const luaL_Reg methods[] = {
    // string <--> json
    {"Create", LuaRandom::Create},
    { NULL, NULL }
};
	

LUALIB_API int luaopen_luarandom(lua_State *L) {
    luaL_newlib(L, methods);
    return 1;
}
}
