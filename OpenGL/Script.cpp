#include"Script.h"

bool validateScriptResult(lua_State* L, int r, std::string errorBuffer) {
	if (r != LUA_OK) {

		errorBuffer = lua_tostring(L, -1);
		return false;
	}
}
