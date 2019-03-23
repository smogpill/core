
function coInitParams(_params)
	co_baseAbsPath = os.getcwd()
	print("co_baseAbsPath: "..co_baseAbsPath)
	co_externalAbsPath = co_baseAbsPath .. "/external"
	co_buildPath = "build/" .. _ACTION
	local coreRelativePath = "."
	if _params.coreRelativePath then
		coreRelativePath = _params.coreRelativePath
	end
	local coreAbsolutePath = path.getabsolute(coreRelativePath)
	co_prebuildPath = coreAbsolutePath .. "/build/prebuild.exe"
	co_versionMajor = 0
	co_versionMinor = 0
	co_versionBuild = 0
end

function coSetWorkspaceDefaults(_name)
	print("Generating workspace ".._name.."... (in "..co_buildPath..")")
	workspace(_name)
	configurations {"debug", "release", "prebuildDebug", "prebuildRelease"}
	location(co_buildPath)
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
	local projectBasePath = "../.."
	local buildAbsPath = projectBasePath .. "/" .. co_buildPath
	local buildLocation = buildAbsPath.."/projects"
	print("Generating project ".._name.."... (in "..buildLocation..")")
	project(_name)

	location(buildLocation)
	architecture "x86_64"
	warnings "Extra"
	kind "StaticLib"
	objdir(buildAbsPath .. "/obj")
	targetdir(buildAbsPath .. "/bin/$(Configuration)")
	libdirs { "$(OutDir)" }
	defines { "coVERSION_MAJOR="..co_versionMajor, "coVERSION_MINOR="..co_versionMinor, "coVERSION_BUILD="..co_versionBuild }
	includedirs { projectBasePath.."/build/gen" }
	includedirs(co_srcDirs)
	debugdir "$(OutDir)"
	defines { "coPROJECT_NAME=".._name }

	if not (_options and _options.prebuildDependency) then
		--removeconfigurations {"prebuild*"}
		configurations { "debug", "release" }
	end

	filter{"configurations:debug or prebuildDebug"}
		defines {"coDEBUG"}
	filter{"configurations:debug or release"}
		defines {"coREFLECT_ENABLED"}
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
		files { "*.natvis"}
		defines { "_HAS_EXCEPTIONS=0" }
		--flags { "StaticRuntime" }
		--linkoptions { "/ENTRY:mainCRTStartup" } -- TODO: Not working with DLL, should avoid that case somehow.
		linkoptions {"/ignore:4221"} -- warns when .cpp are empty depending on the order of obj linking.
		
	filter { "configurations:release or prebuildRelease" }
		flags { "OptimizeSpeed", "NoFramePointer"}
	filter {"configurations:debug or release"}
		if os.isfile("reflect.cpp") then
			local projectBasePath = "../.."
			local genAbsPath = "../../gen"
			local command = co_prebuildPath .. ' "' .. path.getabsolute(co_projectDir, path.getabsolute(projectBasePath)) .. "/.." .. '" "' .. genAbsPath .. '" "' .. _name .. '" "'.. _name ..'/pch.h"'
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
			return d, e
		end
	end
	return nil
end

function coIncludeProject(_srcDirs, _projectName)
	local srcDir, foundDir = coFindDir(_srcDirs, _projectName)
	if foundDir == nil then
		error("Failed to find the project: ".._projectName)
	else
		co_projectSrcDir = srcDir
		co_projectDir = foundDir
		include(foundDir)
	end
end

function coGenerateProjectWorkspace(_params)
	srcDirs = {path.getabsolute("src")}
	co_workspaceDependencies = {}
	for k, d in pairs(_params.dependencies) do
		table.insert(srcDirs, path.getabsolute(d.."/src"))
		co_workspaceDependencies[k] = path.getabsolute(d)
	end

	co_srcDirs = srcDirs

	coSetWorkspaceDefaults(_params.name)
	startproject(_params.projects[0])
	for _, p in pairs(_params.projects) do
		coIncludeProject(srcDirs, p)
	end
	if co_dependencies then
		for _, d in pairs(co_dependencies) do
			coIncludeProject(srcDirs, d)
		end
	end
end
