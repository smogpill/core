coSetProjectDefaults("runtime")
kind "WindowedApp"
links {"lang", "debug", "app", "pattern", "memory", "container", "io", "platform", "math", "render", "render_vulkan"}
libdirs "$(VULKAN_SDK)/Bin"
links {"vulkan-1.lib"}
