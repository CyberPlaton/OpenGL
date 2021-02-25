#include"Script.h"

LuaBinding* LuaBinding::g_pLua = nullptr;



// Lua callable functions.
int luaDoSomething(lua_State* L) {

	float a = (float)lua_tonumber(L, 1);
	float b = (float)lua_tonumber(L, 2);

	float c = 256 * (a + b);

	lua_pushnumber(L, c); // Push result back to stack.

	return 1;
}



bool LuaBinding::startUp(LuaFunctionBindingMap* luaFuncs) {

	if (!g_pLua->m_LuaState) {

		g_pLua->m_LuaState = luaL_newstate();

		// Enable all standard libraries for lua.
		luaL_openlibs(g_pLua->m_LuaState);


		// Register specified functions.
		if (luaFuncs) {

			for (auto it : luaFuncs->map) {

				lua_register(g_pLua->m_LuaState, it.first.c_str(), it.second);
			}
		}



		return true;
	}
	return false;
}

bool LuaBinding::_validate(lua_State* L, int r, std::string errorBuffer) {

	if (r != LUA_OK) {

		errorBuffer = lua_tostring(L, -1);
		return false;
	}
}


bool LuaBinding::execute(std::string script) {


	// Example, not final!

	int result = luaL_dofile(g_pLua->m_LuaState, script.c_str());
	std::string rBuffer;
	if (_validate(g_pLua->m_LuaState, result, rBuffer)) {


		lua_getglobal(g_pLua->m_LuaState, "add"); // pop function result value to stack top.


		if (lua_isfunction(g_pLua->m_LuaState, -1)) {

			lua_pushnumber(g_pLua->m_LuaState, 0.1f);
			lua_pushnumber(g_pLua->m_LuaState, 0.9f);


			if (_validate(g_pLua->m_LuaState, lua_pcall(g_pLua->m_LuaState, 2, 1, 0), rBuffer)) {

				using namespace std;
				cout << color(colors::GREEN);
				cout << "doSomething result = " << (float)lua_tonumber(g_pLua->m_LuaState, -1) << white << endl;


				return true;
			}
			else {

				using namespace std;
				cout << color(colors::RED);
				cout << "Script Error: " << rBuffer << white << endl;
			}
		}
	}


	return false;
}

