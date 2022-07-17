dofile("premakeUtils.lua")

coInitParams{}
coGenerateProjectWorkspace{name = "core", projects={"app", "event", "imgui", "optick", "test_container", "test_io", "test_math", "test_app", "test_pattern"}, dependencies={}}