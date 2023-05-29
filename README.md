# Atom Destiny
Unreal Engine 5 Atom destiny game project.

### Build Dependencies
- Unreal engine 5.2
- clang 15.0.5+ for Mac
- msvc toolchain 14.33 (VS2022) or newier for Windows

### Contribution

For assets and editor we use [Unrealistic](https://unrealistic.dev/posts/unrealistic-style-guide) style guide. For C++ we use [Our](https://github.com/AtomDestiny/AtomDestiny/blob/main/CodingStyle.md) coding style guide.
So for contribution you should use them both.

### Third-party libraries

Project actively uses C++20 coroutines, so realization used from [UE5Coro](https://github.com/landelare/ue5coro).

### Build
#### Windows
Build can fail with UBT linker error, code 0x0000006.
Possible solution to specify concrete cl.exe compiler to build configuration.
It can be done at folder ```<USER>/AppData/Roaming/Unreal Engine/UnrealBuildTool/BuildConfiguration.xml```

Add follows to configuration file. Change compiler version if needed.
```
    <WindowsPlatform>
        <CompilerVersion>14.33.31629</CompilerVersion>
    </WindowsPlatform>
```
