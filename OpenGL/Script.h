#pragma once

extern "C"{
#include"lua.hpp"
}

#include<string>
#include<map>

#include"ColorConsole.h"

// LUA EXAMPLES

    /*
    // Testing LUA.
    struct sPlayer {
        std::string name;
        float health;
        float armor;
        float power;
    };


    lua_State* luaState = luaL_newstate();
    luaL_openlibs(luaState); // Open/Allow mostly used and default lua libraries, like math.

    int result = luaL_dofile(luaState, "simpleScript.lua");
    std::string rBuffer;


    // Link function to lua virtual machine.
    lua_register(luaState, "doSomething", luaDoSomething); // Just like we mapped in command handler...
    */



    /*
    if (validateScriptResult(luaState, result, rBuffer)) {

        lua_getglobal(luaState, "doSomething"); // pop function result value to stack top.

        if (lua_isfunction(luaState, -1)) {

            lua_pushnumber(luaState, 0.1f);
            lua_pushnumber(luaState, 0.9f);

            if (validateScriptResult(luaState, lua_pcall(luaState, 2, 1, 0), rBuffer)) {

                using namespace std;
                cout << color(colors::GREEN);
                cout << "doSomething result = " << (float)lua_tonumber(luaState, -1) << white << endl;

            }
            else {

                using namespace std;
                cout << color(colors::RED);
                cout << "Script Error: " << rBuffer << white << endl;
            }
        }
    }
    */

    // Lua functions.
    /*
    sPlayer* bogdan = new sPlayer();
    if (validateScriptResult(luaState, result, rBuffer)) {

        lua_getglobal(luaState, "getPlayer"); // pop function result value to stack top.

        if (lua_isfunction(luaState, -1)) {

            lua_pushnumber(luaState, 1);

            //                                                     args, return
            if (validateScriptResult(luaState, lua_pcall(luaState, 1, 1, 0), rBuffer)) {  // execute single function with arguments we provided...


                if (lua_istable(luaState, -1)) {

                    // Get first element from player table
                    lua_pushstring(luaState, "name"); // Pop value from player table to stacks top...
                    lua_gettable(luaState, -2); // Get first element in table.

                    bogdan->name = lua_tostring(luaState, -1); // We popped "name" to stack firsts...

                    lua_pop(luaState, 1);

                    // .. do same with other entries, here get next value from player table
                    lua_pushstring(luaState, "health"); // Pop value from player table to stacks top...
                    lua_gettable(luaState, -2); // Get first element in table.

                    bogdan->health = (float)lua_tonumber(luaState, -1); // We popped "name" to stack firsts...

                    lua_pop(luaState, 1);


                    // .. do same with other entries, here get next value from player table
                    lua_pushstring(luaState, "armor"); // Pop value from player table to stacks top...
                    lua_gettable(luaState, -2); // Get first element in table.

                    bogdan->armor = (float)lua_tonumber(luaState, -1); // We popped "name" to stack firsts...

                    lua_pop(luaState, 1);



                    // .. do same with other entries, here get next value from player table
                    lua_pushstring(luaState, "power"); // Pop value from player table to stacks top...
                    lua_gettable(luaState, -2); // Get first element in table.

                    bogdan->power = (float)lua_tonumber(luaState, -1); // We popped "name" to stack firsts...

                    lua_pop(luaState, 1);
                }
                using namespace std;
                cout << color(colors::GREEN);
                cout << "Name: " << bogdan->name << endl;
                cout << "Health: " << bogdan->health << endl;
                cout << "Armor: " << bogdan->armor << endl;
                cout << "Power: " << bogdan->power << white << endl;
            }
            else {

                using namespace std;
                cout << color(colors::RED);
                cout << "Script Error: " << rBuffer << white << endl;
            }
        }

    }
    */

    //sPlayer* bogdan = new sPlayer();

    /*
    if (validateScriptResult(luaState, result, rBuffer)) {

        // Naive approach ot extract data.


        lua_getglobal(luaState, "name"); // Pop value of a and put it on top of the lua stack.
        bogdan->name = lua_tostring(luaState, -1);

        lua_getglobal(luaState, "health"); // Pop value of a and put it on top of the lua stack.
        bogdan->health = (float)lua_tonumber(luaState, -1);

        lua_getglobal(luaState, "armor"); // Pop value of a and put it on top of the lua stack.
        bogdan->armor = (float)lua_tonumber(luaState, -1);

        lua_getglobal(luaState, "power"); // Pop value of a and put it on top of the lua stack.
        bogdan->power= (float)lua_tonumber(luaState, -1);



        lua_getglobal(luaState, "player"); // Pop value of a and put it on top of the lua stack.
        if (lua_istable(luaState, -1)) {

            // Get first element from player table
            lua_pushstring(luaState, "name"); // Pop value from player table to stacks top...
            lua_gettable(luaState, -2); // Get first element in table.

            bogdan->name = lua_tostring(luaState, -1); // We popped "name" to stack firsts...

            lua_pop(luaState, 1);

            // .. do same with other entries, here get next value from player table
            lua_pushstring(luaState, "health"); // Pop value from player table to stacks top...
            lua_gettable(luaState, -2); // Get first element in table.

            bogdan->health = (float)lua_tonumber(luaState, -1); // We popped "name" to stack firsts...

            lua_pop(luaState, 1);


            // .. do same with other entries, here get next value from player table
            lua_pushstring(luaState, "armor"); // Pop value from player table to stacks top...
            lua_gettable(luaState, -2); // Get first element in table.

            bogdan->armor = (float)lua_tonumber(luaState, -1); // We popped "name" to stack firsts...

            lua_pop(luaState, 1);



            // .. do same with other entries, here get next value from player table
            lua_pushstring(luaState, "power"); // Pop value from player table to stacks top...
            lua_gettable(luaState, -2); // Get first element in table.

            bogdan->power = (float)lua_tonumber(luaState, -1); // We popped "name" to stack firsts...

            lua_pop(luaState, 1);
        }



        using namespace std;
        cout << color(colors::GREEN);
        cout << "Name: " << bogdan->name << endl;
        cout << "Health: " << bogdan->health << endl;
        cout << "Armor: " << bogdan->armor << endl;
        cout << "Power: " << bogdan->power << white << endl;

    }
    */


    /*
    if (!validateScriptResult(luaState, result, rBuffer)) {

        using namespace std;
        cout << color(colors::RED);
        cout << "Script Error: " << rBuffer << white << endl;
    }
    else {

        lua_getglobal(luaState, "a"); // Pop value of a and put it on top of the lua stack.
        if (lua_isnumber(luaState, -1)) { // -1 means get the upper most stack element in lua stack. Next elements can be get through -2, -3... Where 1 is the lowermost stack element.

            float n = (float)lua_tonumber(luaState, -1); // Get a as number in C++.

            using namespace std;
            cout << color(colors::GREEN);
            cout << "Script Result: " << n << white << endl;


            result = luaL_dofile(luaState, "simpleScript.lua"); // Execute this new command with same value in lua stack but altered.


            if (!validateScriptResult(luaState, result, rBuffer)) {

                using namespace std;
                cout << color(colors::RED);
                cout << "Script Error: " << rBuffer << white << endl;
            }
            else {

                lua_getglobal(luaState, "a"); // Pop value of a and put it on top of the lua stack.
                if (lua_isnumber(luaState, -1)) { // -1 means get the upper most stack element in lua stack. Next elements can be get through -2, -3... Where 1 is the lowermost stack element.

                    float n = (float)lua_tonumber(luaState, -1); // Get a as number in C++.

                    using namespace std;
                    cout << color(colors::GREEN);
                    cout << "Script Result: " << n << white << endl;
                }


            }
        }
    }
    */

    /*
    if (result == LUA_OK) { // Validate script.

        lua_getglobal(luaState, "a"); // Pop value of a and put it on top of the lua stack.
        if (lua_isnumber(luaState, -1)) { // -1 means get the upper most stack element in lua stack. Next elements can be get through -2, -3... Where 1 is the lowermost stack element.

            float n = (float)lua_tonumber(luaState, -1); // Get a as number in C++.

            using namespace std;
            cout << color(colors::GREEN);
            cout << "Script Result: " << n << white << endl;
        }
    }
    else {

        using namespace std;
        cout << color(colors::RED);
        cout << "Script Error: " << lua_tostring(luaState, -1) << white << endl;
    }
    */


// Lua callable functions.
int luaDoSomething(lua_State* L);


// Defines functions for lua virtual machine,
// like:
// "doSomething", luaDoSomething, where the second is the function pointer
typedef int(*ScriptFunction)(lua_State* L); // Returns int, only luaState as param
struct LuaFunctionBindingMap {

	std::map<std::string, ScriptFunction> map;
};


struct Script {

	std::string m_Path;

	Script(std::string path) : m_Path(path) {};
};



class LuaBinding {
public:

	static LuaBinding* get() {

		if (!g_pLua) g_pLua = new LuaBinding();

		return g_pLua;
	}

	static void del() {
		if (g_pLua) delete g_pLua;
	}


	static bool startUp(LuaFunctionBindingMap* luaFuncs = nullptr);

	static bool execute(Script script);


private:

	static LuaBinding* g_pLua;


	lua_State* m_LuaState = nullptr; // Should be initialized once.

private:

	LuaBinding() = default; // Default private constructor
	LuaBinding(const LuaBinding&) = delete; // No copy
	LuaBinding& operator=(const LuaBinding&) = delete; // No assignment




	static bool _validate(lua_State* L, int r, std::string errorBuffer);

};
