coSetCppProjectDefaults("prebuild")
kind "ConsoleApp"
coSetProjectDependencies{"lang", "debug", "app", "pattern", "memory", "parser", "parser_clang", "container", "io", "platform", "math"}
--links {"lang", "debug", "app", "pattern", "memory", "parser", "parser_clang", "container", "io", "platform", "math"}
links {co_externalAbsPath .. "/clang/lib/vs/x64/libclang"}

debugargs {"$(OutputPath)../../../src", "$(OutputPath)../../gen", "math", "math/pch.h"}

filter { "configurations:prebuildRelease" }
postbuildcommands{'copy /Y "$(OutputPath)prebuild.exe" "'..co_prebuildPath..'"'}
filter {}
