dofile("premakeUtils.lua")

coInitParams{}
coGenerateProjectWorkspace{name = "tests", projects={"test_container", "test_io", "test_math", "test_neural"}, srcDirs={"src"}}