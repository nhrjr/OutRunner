#!lua

solution "Outrunner"
    configurations {"Debug", "Release"}
        location "Build"

    -- configuration "Debug"
    --     targetdir "Build/Debug"

    -- configuration "Release"
    --     targetdir "Build/Release"

project "Runner"
    kind "WindowedApp"
    language "C++"
    -- location "Build"

    pchheader "stdafx.h"
    pchsource "stdafx.cpp"
    -- includedirs {".","Runner/src"}
    includedirs {"./**"}
    targetdir "Runner"

    files {
        -- "Runner/src/**.h",
        "Runner/src/**"
         }
    -- print(project().basedir)
    
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

