#pragma once
#include <pch.h>
#include "IO/File/Folder.hpp"


const static inline std::string LandmarkFileExtension = "lmf";
const static inline std::string AssetFolderName = "assets";
const static inline std::string ImportFolderName = "import";
const static inline Folder AssetFolder = Folder(AssetFolderName);
const static inline Folder ImportFolder = Folder(ImportFolderName);

using AssetUUIDGenType = boost::uuids::random_generator;
static inline AssetUUIDGenType AssetUUIDGenerator;
using AssetUUID = boost::uuids::uuid;

using AssetTimeType = std::filesystem::file_time_type;

