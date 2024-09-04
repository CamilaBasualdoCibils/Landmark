#include "Folder.h"
class IO {

private:

static inline std::string resourcePath = "resources";
static inline std::string cachePath = "cache";

static Folder GetOrCreate(const std::string& path) {
    Folder f(path);
    if (!f.Exists()) f.Create();
    return f;
}
public:
static Folder GetResources() {
    return GetOrCreate(resourcePath);
}
static Folder GetCache() {
    return GetOrCreate(cachePath);
}

static Folder GetFolder(const std::string& path) {
    return Folder(path);
}
};