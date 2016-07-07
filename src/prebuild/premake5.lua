coSetProjectDefaults("prebuild")
kind "ConsoleApp"
links {"lang"}

clangPath = externalAbsPath .. "/clang"
includedirs {clangPath.."/include"}
links {clangPath.."/lib/vs/x64/libclang"}
