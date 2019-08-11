-- Premake 5
workspace "TriVEngine"
	architecture "x64"
	configurations {"Debug", "Release"}

-- General Variables
outputDir = "%{cfg.buildcfg}-%{cfg.system}-x64"
includePhysX = false
useScripting = false

project "TriVEngine"
	kind "StaticLib"
	staticruntime "on"
	language "C++"
	cppdialect "C++latest"
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputDir .. "/%{prj.name}")
	files
	{
		"Source/**.h", 
		"Source/**.hpp", 
		"Source/**.cpp",
		"Source/Scripting/**"
	}
	
	if not includePhysX then
		removefiles{"Source/Engine/Physics/PhysX/**"}
	end
	
	if not useScripting then
		removefiles{"Source/Scripting/**"}
	end
	
	includedirs
	{
		"Libraries/glm-0.9.9.5/glm",
		"Libraries/glfw-3.3.bin.WIN64/include",
		"Libraries/vulkan-1.1.114.0/Include"
	}

	if includePhysX then
		includedirs{"Libraries/physx-4.1.0-x64/include"}
	end

	--We don't want this annoying "Member not initialized" spam.
	disablewarnings{"26495"}
	
	filter "system:windows"
			systemversion "latest"
	
	filter "configurations:Debug"
		defines { "TriV_DEBUG" }
		symbols "on"
		links
		{
			"Libraries/glfw-3.3.bin.WIN64/lib-vc2019/Debug/glfw3.lib",
			"Libraries/vulkan-1.1.114.0/Lib/vulkan-1.lib"
		}
		if includePhysX then
			links
			{
				"Libraries/physx-4.1.0-x64/lib-vc2019/Checked/PhysX_static_64.lib",
				"Libraries/physx-4.1.0-x64/lib-vc2019/Checked/PhysXCommon_static_64.lib",
				"Libraries/physx-4.1.0-x64/lib-vc2019/Checked/PhysXExtensions_static_64.lib",
				"Libraries/physx-4.1.0-x64/lib-vc2019/Checked/PhysXFoundation_static_64.lib",
				"Libraries/physx-4.1.0-x64/lib-vc2019/Checked/PhysXPvdSDK_static_64.lib"
			}
		end
		
	filter "configurations:Release"
		defines { "TriV_RELEASE" }
		optimize "on"
		links
		{
			"Libraries/glfw-3.3.bin.WIN64/lib-vc2019/Release/glfw3.lib",
			"Libraries/vulkan-1.1.114.0/Lib/vulkan-1.lib"
		}
		if includePhysX then
			links
			{
				"Libraries/physx-4.1.0-x64/lib-vc2019/Release/PhysX_static_64.lib",
				"Libraries/physx-4.1.0-x64/lib-vc2019/Release/PhysXCommon_static_64.lib",
				"Libraries/physx-4.1.0-x64/lib-vc2019/Release/PhysXExtensions_static_64.lib",
				"Libraries/physx-4.1.0-x64/lib-vc2019/Release/PhysXFoundation_static_64.lib",
				"Libraries/physx-4.1.0-x64/lib-vc2019/Release/PhysXPvdSDK_static_64.lib"
			}
		end
		
	postbuildcommands
	{
		--TODO
	}