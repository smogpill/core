coSetProjectDefaults("prebuild")
kind "ConsoleApp"
links {"lang", "container", "debug", "app", "pattern", "memory"}

clangPath = externalAbsPath .. "/clang"
includedirs {clangPath.."/include"}
links {clangPath.."/lib/vs/x64/libclang"}
