#include <pch.h>

inline std::string string_formatBytes(size_t bytes) {
    const char* units[] = {"Bytes", "KB", "MB", "GB", "TB", "PB"};
    const size_t unitCount = sizeof(units) / sizeof(units[0]);

    double size = static_cast<double>(bytes);
    size_t unitIndex = 0;

    while (size >= 1024 && unitIndex < unitCount - 1) {
        size /= 1024;
        ++unitIndex;
    }

    std::ostringstream formattedSize;
    formattedSize << std::fixed << std::setprecision(2) << size << " " << units[unitIndex];

    return formattedSize.str();
}