local OutputDir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"
workspace "WilliamMidterm"
    architecture "x86_64"
    configurations {"Debug", "Release"}
    startproject "Game"
    targetdir ("Binaries/" .. OutputDir .. "/%{prj.name}")
    objdir("Binaries/Intermediates/" .. OutputDir .. "/%{prj.name}")
    location "build"


project "Game"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"

    files {
        "Game/src/**.cpp",
        "Game/include/**.h"
    }
    includedirs{
        "Vendor/raylib/src",
	"Game/include"
    }
    
    links {"Raylib"}

    filter {"system:windows"}
        links {"opengl32", "gdi32", "winmm"}
    
    implicitlink "Off"
include "Vendor/raylib/Build-raylib.lua"