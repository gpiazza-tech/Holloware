workspace "Holloware"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Holloware/vendor/GLFW/include"
IncludeDir["Glad"] = "Holloware/vendor/Glad/include"
IncludeDir["ImGui"] = "Holloware/vendor/imgui"
IncludeDir["glm"] = "Holloware/vendor/glm"
IncludeDir["stb_image"] = "Holloware/vendor/stb_image"
IncludeDir["entt"] = "Holloware/vendor/entt/include"
IncludeDir["nlohmann_json"] = "Holloware/vendor/nlohmann_json/include"
IncludeDir["tcc"] = "Holloware/vendor/tcc/win32/libtcc"
IncludeDir["efsw"] = "Holloware/vendor/efsw/include"

group "Dependencies"
    include "Holloware/vendor/GLFW"
    include "Holloware/vendor/Glad"
    include "Holloware/vendor/imgui"
    include "Holloware/vendor/efsw"
group ""

project "Holloware"
    location "Holloware"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    toolset "msc-v145"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "hwpch.h"
    pchsource "Holloware/src/hwpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
        "%{prj.name}/vendor/stb_image/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.nlohmann_json}",
        "%{IncludeDir.tcc}",
        "%{IncludeDir.efsw}",
    }

    libdirs 
    {
        "Holloware/vendor/tcc/bin/Debug-windows-x86_64/tcc",
        "Holloware/vendor/efsw/bin/Debug-windows-x86_64/efsw"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib",
        "efsw-static-debug",
        "tcc"
    }

    buildoptions
    {
        "/utf-8"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "HW_PLATFORM_WINDOWS",
            "HW_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }
    
    filter "configurations:Debug"
        defines "HW_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "HW_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "HW_DIST"
        runtime "Release"
        optimize "on"

project "Holloware-Editor"
    location "Holloware-Editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    toolset "msc-v145"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Holloware/vendor/spdlog/include",
        "Holloware/src",
        "Holloware/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.nlohmann_json}",
        "%{IncludeDir.tcc}",
    }

    libdirs 
    { 
        "Holloware/vendor/tcc/bin/Debug-windows-x86_64/tcc",
        "Holloware/vendor/efsw/bin/Debug-windows-x86_64/efsw"
    }

    links
    {
        "Holloware",
        "efsw-static-debug",
        "tcc"
    }

    buildoptions
    {
        "/utf-8"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "HW_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "HW_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "HW_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "HW_DIST"
        runtime "Release"
        optimize "on"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    toolset "msc-v145"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "Holloware/vendor/spdlog/include",
        "Holloware/src",
        "Holloware/vendor",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.nlohmann_json}",
        "%{IncludeDir.tcc}",
        "%{IncludeDir.efsw}",
    }

    links
    {
        "Holloware"
    }

    buildoptions
    {
        "/utf-8"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "HW_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "HW_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "HW_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "HW_DIST"
        runtime "Release"
        optimize "on"
