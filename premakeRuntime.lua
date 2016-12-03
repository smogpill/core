dofile("premakeUtils.lua")

coInitParams{}
coGenerateProjectWorkspace{name = "runtime", projects={"runtime"}, srcDirs={"src"}}