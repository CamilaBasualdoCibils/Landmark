-- premake5.lua
workspace "Landmark"
   configurations { "Debug", "Release" }

   --[[
project "Demo"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   flags{"MultiProcessorCompile"}

   links { "Landmark","ImGui","ImPlot","vulkan","glfw",
   "glslang","MachineIndependent","OSDependent","GenericCodeGen" }
   
   files { "demo/src/**.h", "demo/src/**.c","demo/src/**.cpp"}
   includedirs {"demo/src","src","lib"}
   libdirs {"bin/%{cfg.buildcfg}"}
   --
   filter "configurations:Editor"
      defines {"_EDITOR"}
      symbols "On"
   
   filter "configurations:Minimal"
      defines { "_MINIMAL" }
      symbols "On"
      optimize "On"

   filter "configurations:Release"
      defines { "_NDEBUG","_RELEASE" }
      optimize "On"
]]

project "Landmark"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   flags{"MultiProcessorCompile"}

   files { "src/**.h", "src/**.c","src/**.cpp"}
   pchheader "pch.h"
   includedirs {"src","lib","imgui","glslang","lib/RenderDoc/include","lib/GLFW/include"}
   links { "ImGui","ImPlot","vulkan","glfw3",
   "glslang","MachineIndependent","OSDependent","GenericCodeGen","EGL","GL","GLEW" }
   libdirs{"lib/RenderDoc/lib","lib/GLFW/lib"}
   dependson {"ImGui","ImPlot"}

   filter "configurations:Debug"
      defines { "_DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "_NDEBUG","_RELEASE" }
      optimize "On"

   --filter "action:gmake" IMPORTANT FOR COMPILING IN LINUX FOR WINDOWS
      --gccprefix "x86_64-w64-mingw32-" --compilation in linux for windows


project "ImGui"
   kind "StaticLib"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   flags{"MultiProcessorCompile"}

   includedirs {"lib/GLFW/include"}
   files { "lib/imgui/**.h","lib/imgui/**.cpp"  }
   filter "configurations:Debug"
      defines { "_DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "_NDEBUG","_RELEASE" }
      optimize "On"

project "ImPlot"
   kind "StaticLib"
   language "C++"
   targetdir "bin/%{cfg.buildcfg}"
   flags{"MultiProcessorCompile"}

   files { "lib/implot/**.h","lib/implot/**.cpp"  }
   includedirs {"lib"}
   
   filter "configurations:Debug"
      defines { "_DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "_NDEBUG","_RELEASE" }
      optimize "On"

function customClean()
    -- Specify the directories or files to be cleaned
    local dirsToRemove = {
        "bin",
        "obj",
        "Intermediate"
    }

    local filesToRemove = {
        "Makefile",
        "myproject.sln",
        "Demo.make",
        "ImGui.make",
        "ImPlot.make",
        "Landmark.make",
      --"imgui.ini",
        "log.txt",
        "Demo.vcxproj",
        "ImGui.vcxproj",
        "ImPlot.vcxproj",
        "Landmark.vcxproj",
        "Landmark.vcxproj.filters",
        "Landmark.sln"
    }

    -- Remove specified directories
    for _, dir in ipairs(dirsToRemove) do
        if os.isdir(dir) then
            os.rmdir(dir)
            print("Removed directory: " .. dir)
        end
    end

    -- Remove specified files
    for _, file in ipairs(filesToRemove) do
        if os.isfile(file) then
            os.remove(file)
            print("Removed file: " .. file)
        end
    end
end

-- Add the custom clean function to the clean action
newaction {
    trigger = "clean",
    description = "Custom clean action",
    execute = function()
        customClean()
    end
}