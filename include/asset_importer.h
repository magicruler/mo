#pragma once
#include "common.h"


class AssetImporter
{
public:
	static void ImportFile(const std::string& path);
	static void ImportTexture(const std::string& assetPath, const std::string& metaPath);
};