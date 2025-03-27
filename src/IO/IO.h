#include "Folder.h"
#include "File.h"
#include "MeshFiles/MeshFile.hpp"
class IO
{

private:
    static inline std::string assetsPath = "assets";
    static inline std::string cachePath = "cache";

    static Folder GetOrCreate(const std::string &path)
    {
        Folder f(path);
        if (!f.Exists())
            f.Create();
        return f;
    }

public:
    static Folder GetAssets()
    {
        return GetOrCreate(assetsPath);
    }
    static Folder GetCache()
    {
        return GetOrCreate(cachePath);
    }

    static Folder GetFolder(const std::string &path)
    {
        return Folder(path);
    }
};