-- Premake 5

-- Dependency Inclusion
include("conanbuildinfo.premake.lua")

workspace "TriVEngine"
	conan_basic_setup()
	startproject "Sandbox"
	architecture "x64"
	configurations {"Debug", "Release"}

-- General Variables
outputDir = "%{cfg.buildcfg}-%{cfg.system}-x64"
includePhysX = false
useScripting = false

project "TriVEngine"
	location "TriVEngine"
	kind "StaticLib"
	staticruntime "on"
	language "C++"
	cppdialect "C++latest"
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputDir .. "/%{prj.name}")
	linkoptions { conan_exelinkflags }
	files
	{
		"%{prj.name}/Source/Engine/**.h", 
		"%{prj.name}/Source/Engine/**.hpp", 
		"%{prj.name}/Source/Engine/**.cpp"
	}
	
	-- Include Scripting folder
	if useScripting then
		files{"%{prj.name}/Source/Scripting/**"}
	end
	
	includedirs
	{
		"Libraries/vulkan-1.1.114.0/Include"
	}

	--We don't want this annoying "Member not initialized" spam.
	disablewarnings{"26495"}
	
	filter "system:windows"
			systemversion "latest"
	
	filter "configurations:Debug"
		defines { "TriV_DEBUG" }
		symbols "on"
		links
		{
			"Libraries/vulkan-1.1.114.0/Lib/vulkan-1.lib"
		}
		
	filter "configurations:Release"
		defines { "TriV_RELEASE" }
		optimize "on"
		links
		{
			"Libraries/vulkan-1.1.114.0/Lib/vulkan-1.lib"
		}
		
	postbuildcommands
	{
		--TODO
	}
	
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	staticruntime "on"
	language "C++"
	cppdialect "C++latest"
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputDir .. "/%{prj.name}")
	linkoptions { conan_exelinkflags }
	files
	{
		"%{prj.name}/Source/**.h", 
		"%{prj.name}/Source/**.hpp", 
		"%{prj.name}/Source/**.cpp"
	}
	
	links
	{
		"TriVEngine"
	}
	
	filter "system:windows"
			systemversion "latest"
	
	filter "configurations:Debug"
		defines { "TriV_DEBUG" }
		symbols "on"
		
	filter "configurations:Release"
		defines { "TriV_RELEASE" }
		optimize "on"