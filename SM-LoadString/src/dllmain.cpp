#include <carbon/lua/lua.hpp>

static int l_loadstring(lua_State* L) {
	int n = lua_gettop(L);

	// Validate the number of arguments
	if (n != 2)
		return luaL_error(L, "loadstring expected 2 arguments (string [code], table [environment]) got %d", n);

	// Validate the environment, numbers can be passed to escape the environment so we make sure it's a table
	luaL_checktype(L, 2, LUA_TTABLE);

	size_t codeLen = 0;
	const char* code = luaL_checklstring(L, 1, &codeLen); // Get the code from the first argument

	if (codeLen >= 4 && strncmp(code, LUA_SIGNATURE, 4) == 0)
		return luaL_error(L, "code cannot be bytecode");

	// Attempt to load the code
	if (luaL_loadstring(L, code) != LUA_OK) {
		lua_pushnil(L); // No function loaded, return nil
		lua_pushstring(L, lua_tostring(L, -2)); // Error
		return 2; // nil, msg
	}

	lua_pushvalue(L, 2); // Bring the env table to the top
	lua_setfenv(L, -2); // Set the environment of the function (only valid for Lua 5.1, SM currently uses 5.1 so this is fine)

	return 1; // func, nil
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);

		// Add the loadstring function to the sandboxed environment
		LuaExecutor::GetInstance()->OnInitialize([](lua_State* L) {
			lua_getglobal(L, "unsafe_env");

			if (!lua_istable(L, -1)) {
				lua_pop(L, 1);
				return luaL_error(L, "unsafe_env not found");
			}

			lua_pushcclosure(L, l_loadstring, 0);
			lua_setfield(L, -2, "loadstring");
			lua_pop(L, 1);
			});
	}

	return TRUE;
}
