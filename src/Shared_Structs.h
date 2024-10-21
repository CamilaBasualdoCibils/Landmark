#pragma once
#include <pch.h>

using VersionInt = uint32_t;
struct Version2 {
    VersionInt major = 1;
    VersionInt minor = 1;
};
struct Version3 : Version2 {
    VersionInt patch = 1;
};
struct Version4: Version3 {
    VersionInt variant = 1;
};
