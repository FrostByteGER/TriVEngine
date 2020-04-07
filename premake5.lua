-- Premake 5
workspace "TriVEngine"
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
	files
	{
		"%{prj.name}/Source/Engine/**.h", 
		"%{prj.name}/Source/Engine/**.hpp", 
		"%{prj.name}/Source/Engine/**.cpp"
	}
	
	-- Exclude PhysX folder
	if not includePhysX then
		removefiles{"%{prj.name}/Source/Engine/Physics/PhysX/**"}
	end
	
	-- Include Scripting folder
	if useScripting then
		files{"%{prj.name}/Source/Scripting/**"}
	end
	
	includedirs
	{
		"Libraries/glm-0.9.9.5/glm",
		"Libraries/glfw-3.3.bin.WIN64/include",
		"Libraries/vulkan-1.1.114.0/Include",
		"Libraries/spdlog-1.3/include",
		"Libraries/imgui-1.72b-docking"
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
			"Libraries/vulkan-1.1.114.0/Lib/vulkan-1.lib",
			"Libraries/glm-0.9.9.5/lib-vc2019/Debug/glm_static.lib"
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
			"Libraries/vulkan-1.1.114.0/Lib/vulkan-1.lib",
			"Libraries/glm-0.9.9.5/lib-vc2019/Release/glm_static.lib"
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
	
project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	staticruntime "on"
	language "C++"
	cppdialect "C++latest"
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediates/" .. outputDir .. "/%{prj.name}")
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