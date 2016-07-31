coSetProjectDefaults("prebuild")
kind "ConsoleApp"
links {"lang", "debug", "app", "pattern", "memory", "parser", "parser_clang", "container", "io", "platform", "math"}
links {externalAbsPath .. "/clang/lib/vs/x64/libclang"}

filter { "configurations:release" }
postbuildcommands{'copy /Y "$(OutputPath)prebuild.exe" "$(OutputPath)prebuild_dist.exe"'}
filter {}
