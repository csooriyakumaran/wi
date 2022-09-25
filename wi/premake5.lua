
project "wi"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("build/" .. outputdir .. "/%{prj.name}")
    
    pchheader "wipch.h"
    pchsource "src/wipch.cpp"

    files 
    { 
        "src/**.h", 
        "src/**.cpp",
    }

    includedirs
    {  
        "src",
        "../wi/vendor/glm",
        "../wi/vendor/spdlog/include",

    }

    links
    {
    }

    defines
    {
        "_USE_MATH_DEFINES"
    }


    filter "system:windows"
        systemversion "latest"
        defines "WI_PLATFORM_WINDOWS"

    filter  "configurations:Debug" 
        defines { "WI_DEBUG" }
        symbols "On"

    filter  "configurations:Release"
        defines { "WI_RELEASE" }
        optimize "On"

    filter  "configurations:Dist"
        defines { "WI_DIST" }
        optimize "On"