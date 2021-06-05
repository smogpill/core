dofile("premakeUtils.lua")

coInitParams{}
coGenerateProjectWorkspace{name = "core", projects={"app", "event", "test_container", "test_io", "test_math", "test_app", "prebuild"}, dependencies={}}