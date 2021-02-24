#pragma once

extern "C"{
#include"lua.hpp"
}

#include<string>

bool validateScriptResult(lua_State* L, int r, std::string errorBuffer);
