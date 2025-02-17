# SM-LoadString

This mod extends the [SM](https://scrapmechanic.com/) Lua API to allow you to run Lua code from a string in UGC/env

## Installation

> [!TIP]
> The recommended method of installing this is [Carbon Launcher](https://github.com/ScrappySM/CarbonLauncher) which is maintained by me and will let you simply press download and launch the game!

However, there are many other ways. Here's a launcher agnostic way of installing this mod:
- Go to releases and download the latest `.dll` file.
- Open a tool like Process Hacker/System Informer and find the process of Scrap Mechanic.
- Inject the `.dll` file as a module into the process.

You can also use a tool like [SM-DLL-Injector](https://github.com/QuestionableM/SM-DLL-Injector) maintained by [QuestionableM](https://github.com/QuestionableM) and will let you simply inject any DLL files in a folder called `DllModules`, you could use this project and put the `.dll` file in the folder and it will automatically inject it for you.

## Usage

This mod is for developers and modders, it allows you to run Lua code from a string in UGC/env. This is useful for many things, such as:

- Making a computers mod that runs Lua from an in game GUI.
- Allowing users to run Lua code from a string in any UGC/env.

## Examples

### Global Env (same as wherever you call the function from)

```lua
local code = [[
  print("Hello, World!")
]]

local func, err = loadstring(code, _G) -- An env must always be passed, _G is the global env for the game
if err then
  print("Error: " .. err)
else
  print(func() or "No return value")
end
```

### Custom Env

```lua
local env = {
    print = print,
    a = 5
}

local code = [[
    print(a)
    a = a + 5
]]

function runCode()
    local func, err = loadstring(code, env)
    if err then
        print("Error: " .. err)
    else
        print(func() or "No return value")
    end
end

runCode()
runCode()
runCode()

-- Output:
-- 5
-- 10
-- 15
```

## Errors

If there is an error in the code, it will return the error as a string as the second return value. If there is no error, it will return `nil`.
However, if there is a misuse of the function, it will throw an error. You can catch these with `pcall` however these errors only exist when you misuse the API and so are best to be fixed rather than caught.

## Checking if the DLL is installed

You can check if the DLL is installed by running the following code:

```lua
if loadstring then
    print("DLL is installed")
else
    print("DLL is not installed")
end
```

## Thanks

Thanks to [crackx02](https://github.com/crackx02) and [QuestionableM](https://github.com/QuestionableM) for pointing out any sandbox escapes allowing the mod to be more secure.
