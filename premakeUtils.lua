baseAbsPath = os.getcwd()
srcAbsPath = baseAbsPath .. "/src"
externalAbsPath = baseAbsPath .. "/external"
buildPath = "build/" .. _ACTION
genAbsPath = baseAbsPath .. "/build/gen"
buildAbsPath = baseAbsPath .. "/" .. buildPath
versionMajor = 0
versionMinor = 0
versionBuild = 0

function coSetWorkspaceDefaults()
	configurations {"debug", "release", "prebuildDebug", "prebuildRelease"}
	location(buildPath)
	objdir(buildPath .. "/obj")
	libdirs { buildPath .. "/bin" }
	targetdir(buildPath .. "/bin")
	defines { "coVERSION_MAJOR="..versionMajor, "coVERSION_MINOR="..versionMinor, "coVERSION_BUILD="..versionBuild }
	includedirs { "src", "build/gen" }
end

function coSetPCH(_dir, _projectName, _fileName)
	pchheader(_projectName .. "/".. _fileName .. '.h')
	pchsource(_fileName .. '.cpp')
	--[[
	filter { "action:vs*" }
		pchheader(_fileName .. '.h')
		pchsource(_fileName .. '.cpp')
	filter { "action:not vs*" }
		pchheader(_dir .. '/' .. _fileName .. '.h')
	filter {}
	--]]
end

function coSetProjectDefaults(_name, _options)
	print("Generating project ".._name.."...")
	project(_name)

	architecture "x86_64"
	warnings "Extra"
	kind "StaticLib"
	location (buildAbsPath.."/projects")
	projectDir = "src/".._name
	defines { "coPROJECT_NAME=".._name }
	debugdir "$(OutDir)"

	if not (_options and _options.prebuildDependency) then
		--removeconfigurations {"prebuild*"}
		configurations { "debug", "release" }
	end

	filter{"configurations:debug or release"}
		defines {"coREFLECT_ENABLED"}
	filter { "configurations:debug or prebuildDebug" }
		targetsuffix "_d"
	filter { "configurations:release or prebuildRelease" }
		optimize "On"
	filter {}	
end

function coSetCppProjectDefaults(_name)
	coSetProjectDefaults(_name)

	rtti "Off"
	language "C++"
	exceptionhandling "Off"
	vectorextensions "SSE2"
	floatingpoint "Fast"
	editandcontinue "Off"
	flags { "Symbols", "NoMinimalRebuild", "FatalWarnings", "C++14", "MultiProcessorCompile" }
	files { "**.cpp", "**.h"}

	if os.isfile("pch.h") then
		coSetPCH(projectDir, _name, "pch")
	end

	filter { "gmake" }
		buildoptions { "-Wno-reorder", "-Wno-deprecated" }
		includedirs { gmakeIncludeDir }
	filter { "vs*" }
		defines { "_HAS_EXCEPTIONS=0" }
		--flags { "StaticRuntime" }
		linkoptions { "/ENTRY:mainCRTStartup" }
	filter { "configurations:release or prebuildRelease" }
		flags { "OptimizeSpeed", "NoFramePointer"}
	filter {"configurations:debug or release"}
		if os.isfile("reflect.cpp") then
			local command = '$(OutputPath)prebuild_dist.exe "' .. srcAbsPath .. '" "' .. genAbsPath .. '" "' .. _name .. '" "'.. _name ..'/pch.h"'
			command = command .. ' -I="$(UniversalCRT_IncludePath)"'
			prebuildcommands{command}
		end
	filter {}
end

function coSetShaderProjectDefaults(_name)
	coSetProjectDefaults(_name)
	kind "Utility"

	-- Defaults
	files { "**.vert", "**.frag"}
	--language "C++" -- We don't have better here

	---[[
	shaderOutPath = "$(OutDir)/shaders/%{file.name}.spv"
	filter {'files:**.vert or **.frag'}
		buildmessage 'Compiling %{file.relpath}'
		buildcommands
		{
			'$(VULKAN_SDK)/Bin/glslangValidator.exe -V -o "'..shaderOutPath ..'" %{file.relpath}'
		}
		buildoutputs { shaderOutPath }
	filter {}
	--]]
end

function coSetProjectDependencies(_deps)
	if not co_dependencies then
		co_dependencies = {}
	end
	for _,v in pairs(_deps) do table.insert(co_dependencies, v) end
	links(_deps)
end

function coGenerateProjectWorkspace(_params)
	print("Generating workspace ".._params.name.."...")
	workspace(_params.name)
	coSetWorkspaceDefaults()
	startproject(_params.projects[0])
	for _, p in pairs(_params.projects) do
		include("src/"..p)
	end
	if co_dependencies then
		for _, d in pairs(co_dependencies) do
			include("src/"..d)
		end
	end
end
