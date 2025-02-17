#include <carbon/lua/lua.hpp>

static int l_loadstring(lua_State* L) {
    int n = lua_gettop(L);

	// Validate the number of arguments
    if (n < 2) {
		lua_pushliteral(L, "loadstring expected 2 arguments (string [code], table [environment]) got 0");
        lua_error(L);
    }
    else if (n > 2) {
		lua_pushfstring(L, "loadstring expected at most 2 arguments (string [code], table [environment]), got %d", n);
		lua_error(L);
    }

	// Validate the environment, numbers can be passed to escape the environment
    if (n > 1 && !lua_istable(L, 2)) {
		lua_pushliteral(L, "second argument must be a table (environment)");
        lua_error(L);
    }

	const char* code = luaL_checkstring(L, 1); // Get the code from the first argument

	// Check the code isn't bytecode, as this is a way to escape the environment
	// Even if this is a user error, the user is most likely trying to escape the environment
	// So they don't need the error message in the UI, they shouldn't be trying to be malicious
	// if they want nice things.
	if (strstr(code, LUA_SIGNATURE) != NULL) {
		lua_pushliteral(L, "code cannot be bytecode");
		lua_error(L);
	}

    if (luaL_loadstring(L, code) == LUA_OK) {
		if (n > 1) {
			lua_pushvalue(L, 2); // Bring the env table to the top
			lua_setfenv(L, -2);
		}

        return 1;
    }
    else {
		lua_pushnil(L); // No function loaded, return nil
        lua_pushstring(L, lua_tostring(L, -2)); // Error
        return 2;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);

		// Add the loadstring function to the sandboxed environment
		LuaExecutor::GetInstance()->OnInitialize([](lua_State* L) {
			lua_getglobal(L, "unsafe_env");
			lua_pushcclosure(L, l_loadstring, 0);
			lua_setfield(L, -2, "loadstring");
			lua_pop(L, 1);
			});
	}

	return TRUE;
}
