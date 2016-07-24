coSetProjectDefaults("prebuild")
kind "ConsoleApp"
links {"lang", "debug", "app", "pattern", "memory", "parser", "parser_clang", "container", "io", "platform"}
links {externalAbsPath .. "/clang/lib/vs/x64/libclang"}
