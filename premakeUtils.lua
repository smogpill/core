
function coInitParams(_params)
	baseAbsPath = os.getcwd()
	print("baseAbsPath: "..baseAbsPath)
	externalAbsPath = baseAbsPath .. "/external"
	buildPath = "build/" .. _ACTION
	genAbsPath = baseAbsPath .. "/build/gen"
	buildAbsPath = baseAbsPath .. "/" .. buildPath
	coreRelativePath = "."
	if _params.coreRelativePath then
		coreRelativePath = _params.coreRelativePath
	end
	coreAbsolutePath = path.getabsolute(coreRelativePath)
	prebuildPath = coreAbsolutePath .. "/build/prebuild.exe"
	versionMajor = 0
	versionMinor = 0
	versionBuild = 0
end

function coSetWorkspaceDefaults(_name, _srcDirs)
	print("Generating workspace ".._name.."... (in "..buildPath..")")
	workspace(_name)
	configurations {"debug", "release", "prebuildDebug", "prebuildRelease"}
	location(buildPath)
	objdir(buildPath .. "/obj")
	libdirs { buildPath .. "/bin" }
	targetdir(buildPath .. "/bin")
	defines { "coVERSION_MAJOR="..versionMajor, "coVERSION_MINOR="..versionMinor, "coVERSION_BUILD="..versionBuild }
	includedirs { "build/gen" }
	includedirs(_srcDirs)
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
	local buildLocation = buildAbsPath.."/projects"
	print("Generating project ".._name.."... (in "..buildLocation..")")
	project(_name)

	architecture "x86_64"
	warnings "Extra"
	kind "StaticLib"
	location(buildLocation)
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
	runtime "Release" -- Even on debug builds, Unreal is setup this way anyway.
	flags { "Symbols", "NoMinimalRebuild", "FatalWarnings", "C++14", "MultiProcessorCompile" }
	files { "**.cpp", "**.h"}

	if os.isfile("pch.h") then
		coSetPCH(co_projectDir, _name, "pch")
	end

	filter { "gmake" }
		buildoptions { "-Wno-reorder", "-Wno-deprecated" }
		includedirs { gmakeIncludeDir }

	filter { "action:vs*" }
		defines { "_HAS_EXCEPTIONS=0" }
		--flags { "StaticRuntime" }
		--linkoptions { "/ENTRY:mainCRTStartup" } -- TODO: Not working with DLL, should avoid that case somehow.
		linkoptions {"/ignore:4221"} -- warns when .cpp are empty depending on the order of obj linking.
		
	filter { "configurations:release or prebuildRelease" }
		flags { "OptimizeSpeed", "NoFramePointer"}
	filter {"configurations:debug or release"}
		if os.isfile("reflect.cpp") then
			local command = prebuildPath .. ' "' .. path.getabsolute(co_projectDir, baseAbsPath) .. "/.." .. '" "' .. genAbsPath .. '" "' .. _name .. '" "'.. _name ..'/pch.h"'
			command = command .. ' -I="$(UniversalCRT_IncludePath)"'
			for _, d in pairs(co_srcDirs) do
				command = command .. ' -I="'..d..'"'
			end
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

function coFindDir(_srcDirs, _dirName)
	for _, d in pairs(_srcDirs) do
		local foundDirs = os.matchdirs(d.."/".._dirName)
		for _, e in pairs(foundDirs) do
			return e
		end
	end
	return nil
end

function coIncludeProject(_srcDirs, _projectName)
	local foundDir = coFindDir(_srcDirs, _projectName)
	if foundDir == nil then
		error("Failed to find the project: ".._projectName)
	else
		co_projectDir = foundDir
		include(foundDir)
	end
end

function coGenerateProjectWorkspace(_params)
	co_srcDirs = _params.srcDirs
	for k, d in pairs(co_srcDirs) do
		co_srcDirs[k] = path.getabsolute(d)
	end
	coSetWorkspaceDefaults(_params.name, _params.srcDirs)
	startproject(_params.projects[0])
	for _, p in pairs(_params.projects) do
		coIncludeProject(_params.srcDirs, p)
		--include(pPath)
	end
	if co_dependencies then
		for _, d in pairs(co_dependencies) do
			coIncludeProject(_params.srcDirs, d)
		end
	end
end
