dofile("premakeUtils.lua")

coInitParams{}
coGenerateProjectWorkspace{name = "prebuild", projects={"prebuild"}, srcDirs={"src"}}