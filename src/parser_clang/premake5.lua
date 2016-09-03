coSetCppProjectDefaults("parser_clang")

clangPath = externalAbsPath .. "/clang"
includedirs {clangPath.."/include"}
--links {clangPath.."/lib/vs/x64/libclang"}
