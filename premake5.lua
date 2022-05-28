workspace "Dawn"
	architecture "x64"
	
	configurations
	{
		"Debug",
		"Release",
		"Dist"	
	}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to solution directory
IncludeDir = {}
IncludeDir["GLFW"] = "Dawn/vendor/GLFW/include"
IncludeDir["Glad"] = "Dawn/vendor/Glad/include"
IncludeDir["ImGui"] = "Dawn/vendor/ImGui"
IncludeDir["glm"] = "Dawn/vendor/glm"

--include "Dawn/vendor/GLFW"
--include "Dawn/vendor/Glad"
--include "Dawn/vendor/ImGui"
	
project "Dawn"
	location "Dawn"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "dwpch.h"
	pchsource "Dawn/src/dwpch.cpp"
	
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/**.hpp",
		"%{prj.name}/vendor/**.inl"
	}
	
	includedirs
	{
--		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
--		"%{IncludeDir.GLFW}",
--		"%{IncludeDir.Glad}",
--		"%{IncludeDir.ImGui}",
--		"%{IncludeDir.glm}"
	}

--	links
--	{
--		"GLFW",
--		"Glad",
--		"ImGui",
--		"opengl32.lib"
--	}
	
	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"DW_PLATFORM_WINDOWS",
			"DW_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"IMGUI_IMPL_OPENGL_LOADER_GLAD"
		}
		
	filter "configurations:Debug"
		defines 
		{
			"DW_DEBUG",
			"DW_ENABLE_ASSERTS"
		}
		buildoptions "/MDd"
		symbols "on"
		
	filter "configurations:Release"
		defines "DW_RELEASE"
		buildoptions "/MD"
		optimize "on"
		
	filter "configurations:Dist"
		defines "DW_DIST"
		buildoptions "/MD"
		optimize "on"
		
--project "Sandbox"
--	location "Sandbox"
--	kind "ConsoleApp"
--	language "C++"
--	cppdialect "C++17"
--	staticruntime "on"	
--	
--	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
--	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
--
--	files
--	{
--		"%{prj.name}/src/**.h",
--		"%{prj.name}/src/**.cpp"
--	}
--		
--	includedirs
--	{
--		"Dawn/vendor/spdlog/include",	
--		"Dawn/src",
--		"Dawn/vendor",
--		"%{IncludeDir.glm}"
--	}
--	
--	links
--	{
--		"Dawn"	
--	}
--	
--	filter "system:windows"
--		systemversion "latest"
--		
--		defines
--		{
--			"DW_PLATFORM_WINDOWS"
--		}
--		
--	filter "configurations:Debug"
--		defines 
--		{
--			"DW_DEBUG",
--			"DW_ENABLE_ASSERTS"
--		}
--		buildoptions "/MDd"
--		symbols "on"
--		
--	filter "configurations:Release"
--		defines "DW_RELEASE"
--		buildoptions "/MD"
--		optimize "on"
--		
--	filter "configurations:Dist"
--		defines "DW_DIST"
--		buildoptions "/MD"
--		optimize "on"
--	