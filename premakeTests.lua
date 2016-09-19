dofile("premakeUtils.lua")

coGenerateProjectWorkspace{name = "tests", projects={"test_container", "test_io", "test_math"}}