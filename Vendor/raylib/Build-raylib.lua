project "Raylib"
    kind "StaticLib"
    language "C"
    
    files{
        "src/*.c",
        "src/*.h"
    }
    
    includedirs{
        "src/external/glfw/include/",
        "src"
    }

    defines{
        "PLATFORM_DESKTOP_GLFW", "GRAPHICS_API_OPENGL_33"
    }
