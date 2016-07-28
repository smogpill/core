baseAbsPath = os.getcwd()
srcAbsPath = baseAbsPath .. "/src"
externalAbsPath = baseAbsPath .. "/external"
buildPath = "build/" .. _ACTION
buildAbsPath = baseAbsPath .. "/" .. buildPath
versionMajor = 0
versionMinor = 0
versionBuild = 0

function coSetSolutionDefaults()
	configurations {"debug", "release"}
	architecture "x86_64"
	location(buildPath)
	objdir(buildPath .. "/obj")
	libdirs { buildPath .. "/bin" }
	targetdir(buildPath .. "/bin")
	defines { "coVERSION_MAJOR="..versionMajor, "coVERSION_MINOR="..versionMinor, "coVERSION_BUILD="..versionBuild }
	rtti "Off"
	exceptionhandling "Off"
	vectorextensions "SSE2"
	warnings "Extra"
	floatingpoint "Fast"
	editandcontinue "Off"
	flags { "Symbols", "NoMinimalRebuild", "FatalWarnings", "C++14", "MultiProcessorCompile" }
	filter { "gmake" }
		buildoptions { "-Wno-reorder", "-Wno-deprecated" }
		includedirs { gmakeIncludeDir }
	filter { "configurations:debug" }
		targetsuffix "_d"
	filter { "configurations:release" }
		optimize "On"
		flags { "OptimizeSpeed", "NoFramePointer"}
	filter { "vs*" }
		defines { "_HAS_EXCEPTIONS=0" }
		--flags { "StaticRuntime" }
		linkoptions { "/ENTRY:mainCRTStartup" }
	filter {}
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

function coSetProjectDefaults(_name, _params)
	project(_name)
	filter {}

	-- Defaults
	kind "StaticLib"
	location (buildAbsPath.."/projects")
	projectDir = "src/".._name
	files { "**.cpp", "**.h"}
	coSetPCH(projectDir, _name, "pch")
	--vpaths { ["*"] = _projectDir }
	defines { "coPROJECT_NAME=".._name }
	language "C++"
	
	if _params then
		if _params.kind then
			kind(_params.kind)
		end
		if _params.links then
			links {_params.links}
		end
	end
end

workspace("core")
	coSetSolutionDefaults()
	includedirs { "src", "build/gen" }

	include("src/lang")
	include("src/debug")
	include("src/math")
	include("src/memory")
	include("src/container")
	include("src/pattern")
	include("src/platform")
	include("src/test")
	include("src/event")
	include("src/io")
	include("src/app")
	include("src/parser")
	include("src/parser_clang")
	include("src/prebuild")
	include("src/test_math")
	include("src/test_container")
	include("src/test_io")
