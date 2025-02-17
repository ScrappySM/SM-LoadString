# Carbon Lib

A Scrap Mechanic modding library, currently in an extremely early stage of development, very unstable and not recommended for use.

## Features

- Lua "bindings" (wrappers) for Scrap Mechanic's Lua library (it uses the games `lua51.dll` to run Lua code)
- Contraption (game singleton) and console (with macros and fmtlib support) classes
- Lua Executor class handling Lua code execution (e.g. hooking the update function to run at the correct time)
