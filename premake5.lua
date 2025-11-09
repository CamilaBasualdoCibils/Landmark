-- premake5.lua
workspace "Landmark"
   configurations { "Debug", "Release" }
    project "Landmark"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++20"
        targetdir "bin/%{cfg.buildcfg}"
        flags{"MultiProcessorCompile"}
        files { "src/**.h", "src/**.c","src/**.cpp"}
        pchheader "pch.h"
        includedirs {"src","lib","glslang"}

        links {"GL","GLEW","glfw3","glm","glslang","imgui","implot","implot3d","SPIRV","vulkan","EGL"}
        buildoptions "-mavx2"
       --libdirs{"lib/RenderDoc/lib","lib/GLFW/lib","lib/glslang/lib"}
        local vcpkg = os.getenv("VCPKG_ROOT") or "vcpkg"
        local triplet = os.getenv("VCPKG_DEFAULT_TRIPLET") or "x64-linux"
        local vcpkg_installed = "vcpkg_installed/".. triplet

        includedirs { vcpkg_installed .. "/include" }
        print(vcpkg_installed)
        libdirs { vcpkg_installed .. "/lib" }

        filter "configurations:Debug"
            defines { "_DEBUG" }
            symbols "On"

        filter "configurations:Release"
            defines { "_NDEBUG","_RELEASE" }
            symbols "On"
            optimize "On"



function customClean()
    -- Specify the directories or files to be cleaned
    local dirsToRemove = {
        "bin",
        "obj",
        "Intermediate",
        "vcpkg_installed"
    }

    local filesToRemove = {
        "Makefile",
        --"imgui.ini",
        "log.txt",
    }

    -- Specify extensions to remove
    local extensionsToRemove = {
        ".tmp",
        ".log",
        ".bak",
        ".obj",
        ".pch",
        ".make",
        ".vcxproj",
        ".vcxproj.filters",
        ".sln"
    }

    -- Helper: check if string ends with suffix
    local function endsWith(str, suffix)
        return str:sub(-#suffix) == suffix
    end

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

    -- Remove files by extension (scans current dir only)
    local files = os.matchfiles("*")
    for _, file in ipairs(files) do
        for _, ext in ipairs(extensionsToRemove) do
            if endsWith(file, ext) then
                os.remove(file)
                print("Removed file by extension: " .. file)
            end
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

newaction 
{
    trigger = "setup",
    description = "Setups up dependencies",
    execute = function ()
        --local vcpkgDir = path.join(os.getenv("HOME") or ".", "vcpkg")
        local manifestDir = os.getcwd()
         os.execute("git clone https://github.com/microsoft/vcpkg.git ")
         os.execute("./vcpkg/bootstrap-vcpkg.sh")
         os.execute("./vcpkg/vcpkg install")
    end
}

newaction
{
    trigger = "GenDocs",
    description = "generate documentation",
    execute = function ()
        os.execute("doxygen Doxyfile")
    end

}