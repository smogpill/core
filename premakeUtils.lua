
function coInitParams(_params)
	co_basePathAbs = os.getcwd()
	print("co_basePathAbs: "..co_basePathAbs)
	co_externalAbsPath = co_basePathAbs .. "/external"
	co_buildPathFromRoot = "build/" .. _ACTION
	co_buildPathAbs = path.join(co_basePathAbs, co_buildPathFromRoot)
	co_devPathAbs = path.join(co_basePathAbs, "dev")
	co_versionMajor = 0
	co_versionMinor = 0
	co_versionBuild = 0
end

function coSetWorkspaceDefaults(_name)
	local locationAbs = path.join(co_buildPathAbs, "workspaces")
	print("Generating workspace ".._name.."... (in "..locationAbs..")")
	workspace(_name)
	co_allConfigurations = {"debug", "dev", "release"}
	configurations(co_allConfigurations)
	location(locationAbs)
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
	local buildRelativePath = path.getrelative(path.getabsolute("."), co_buildPathAbs)
	local locationAbs = path.join(co_buildPathAbs, "projects")
	local locationFromProject = path.getrelative(locationAbs, path.getabsolute("."))
	local objDirAbs = path.join(co_buildPathAbs, "obj")
	local targetDirAbs = path.join(co_buildPathAbs, "bin/$(Configuration)")
	print("Generating project ".._name.."... (in "..locationAbs..")")
	project(_name)

	location(locationAbs)
	architecture "x86_64"
	warnings "Extra"
	kind "StaticLib"
	objdir(objDirAbs)
	targetdir(targetDirAbs)
	libdirs { "$(OutDir)" }
	defines { "coVERSION_MAJOR="..co_versionMajor, "coVERSION_MINOR="..co_versionMinor, "coVERSION_BUILD="..co_versionBuild }
	includedirs(co_srcDirs)
	debugdir "$(OutDir)"
	defines { "coPROJECT_NAME=".._name }

	if not (_options) then
		configurations { "debug", "dev", "release" }
	end

	filter{"configurations:debug or dev"}
		defines {"coDEBUG", "coDEV"}
	filter{"configurations:debug or dev or release"}
		defines {"coREFLECT_ENABLED"}
	filter { "configurations:dev or release" }
		optimize "On"
	filter{"configurations:release"}
		defines {"coRELEASE"}
	filter {}	
end

function coSetCppProjectDefaults(_name)
	coSetProjectDefaults(_name)

	rtti "Off"
	language "C++"
	exceptionhandling "Off"
	vectorextensions "SSE2"
	floatingpoint "Strict" -- Not slower than Fast, and helps for cross-platform/compiler determinism.
	editandcontinue "Off"
	symbols "On"
	cppdialect "C++17"
	runtime "Release" -- Even on debug builds, Unreal is setup this way anyway. But can't use the CRT library memory leaks detector

	flags { "NoMinimalRebuild", "FatalWarnings", "MultiProcessorCompile" }
	files { "**.cpp", "**.h", "**.inl", "**.frag", "**.vert", "**.comp", "**.tesc", "**.tese", "**.geom", "**.glsl", "**.importShaders"}

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
		
	filter { "configurations:dev or release" }
		optimize "Speed"
		omitframepointer "On"
	filter {"configurations:debug or dev or release"}

	filter {'files:**.vert or **.frag or **.comp or **.geom'}
		--shaderOutPath = path.join(co_buildAbsPath, "bin/$(Configuration)/shaders/" .. _name .. "/%{file.name}.spv")
		shaderOutPath = "$(OutDir)/shaders/" .. _name .. "/%{file.name}.spv"
		buildmessage 'Compiling %{file.relpath}'
		buildcommands { '$(GLSLANG)/glslangValidator.exe -G -o "'.. shaderOutPath ..'" %{file.relpath}' }
		buildoutputs { shaderOutPath }
	filter {}
end

function coSetShaderDefaults(_name)
	-- Defaults
	--language "C++" -- We don't have better here

	---[[
	

	--]]
end

function coSetProjectDependencies(_deps)
	if not co_dependencies then
		co_dependencies = {}
	end
	for _,v in pairs(_deps) do 
		table.insert(co_dependencies, v)
	end

	links(_deps)
end

function coSetProjectDeployBuildFiles(srcDirFromOutDir, destDirFromRoot)
	local srcDirAbs = "$(OutDir)/"..srcDirFromOutDir
	local destDirAbs = path.join(co_basePathAbs, destDirFromRoot)
	local destDirFromProject = path.getrelative(path.getabsolute("."), destDirAbs)
	postbuildcommands { "{COPY} \""..srcDirAbs.."\" \""..destDirFromProject.."\"" }
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
