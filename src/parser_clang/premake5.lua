coSetCppProjectDefaults("parser_clang")

clangPath = co_externalAbsPath .. "/clang"
includedirs {clangPath.."/include"}
--links {clangPath.."/lib/vs/x64/libclang"}
