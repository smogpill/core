baseAbsPath = os.getcwd()
srcAbsPath = baseAbsPath .. "/src"
buildPath = "build/" .. _ACTION
buildAbsPath = baseAbsPath .. "/" .. buildPath
versionMajor = 0
versionMinor = 0
versionBuild = 0

function setSolutionDefaults()
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

function setPCH(_dir, _projectName, _fileName)
	pchheader(_projectName .. "/".. _fileName .. '.h')
	pchsource(_dir .. "/" .. _fileName .. '.cpp')
	--[[
	filter { "action:vs*" }
		pchheader(_fileName .. '.h')
		pchsource(_fileName .. '.cpp')
	filter { "action:not vs*" }
		pchheader(_dir .. '/' .. _fileName .. '.h')
	filter {}
	--]]
end

function setProjectDefaults(_projectDir, _projectName, _prefix, _postfix)
	__prefix = _prefix
	__postfix= _postfix
	location (buildAbsPath.."/projects")
	includedirs { _projectDir }
	--files { _prefix.."PCH".._postfix..".cpp" }
	files { _projectDir .. "/" .. "**.cpp", _projectDir .. "/" .. "**.h"}
	setPCH(_projectDir, _projectName, "pch")
	--vpaths { ["*"] = _projectDir }
	defines { "coPROJECT_NAME=".._projectName }
	language "C++"
	filter {}
end

function includeProject(_name)
	project(_name)
	setStaticLibDefaults()
	setProjectDefaults("src/".._name, _name , "", "")
end

function setStaticLibDefaults()
	kind "StaticLib"
end

function setConsoleAppDefaults()
	kind "ConsoleApp"
end

function setWindowedAppDefaults()
	kind "WindowedApp"
end

function setConsoleAppDefaults()
	kind "ConsoleApp"
end

workspace("core")
	setSolutionDefaults()
	includedirs { "src" }

	includeProject("debug")
	includeProject("lang")
	includeProject("math")
	includeProject("memory")
	includeProject("container")
