#!lua

solution "Outrunner"
    configurations {"Debug", "Release"}
    -- location "Build"

    -- configuration "Debug"
    --     targetdir "Build/Debug"

    -- configuration "Release"
    --     targetdir "Build/Release"

project "runner"
    kind "WindowedApp"
    language "C++"
    -- location "Build"

    -- pchheader "stdafx.h"
    -- pchsource "stdafx.cpp"
    includedirs {".","Runner/src"}
    targetdir "Runner"

    files {
        "Runner/src/**.h",
         "Runner/src/**.cpp"
         }
    
    links {
        "sfml-window",
        "sfml-graphics",
        "sfml-system",
        "sfml-network",
        "uuid"
        }
    buildoptions { "-std=c++17","-fopenmp"}
    configuration "Debug"        
        flags { "Symbols" }

    configuration "Release"
        flags { "OptimizeSpeed" }

