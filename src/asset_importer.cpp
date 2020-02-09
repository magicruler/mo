#include "asset_importer.h"
#include "json.hpp"
#include "string_utils.h"
#include <filesystem>

using json = nlohmann::json;

void AssetImporter::ImportFile(const std::string& path)
{
	std::filesystem::path pathObj(path);

	std::string fileName = pathObj.filename().generic_string();
	std::string relativePath = StringUtils::ParseDirectory(pathObj.generic_string());

	std::string suffix;
	std::string name;
	bool hasSuffix;
	StringUtils::ParseFileName(fileName, name, suffix, hasSuffix);

	if (hasSuffix)
	{
		if (suffix == "jpg" || suffix == "png" || suffix == "hdr" || suffix == "exr")
		{
			ImportTexture(relativePath + fileName, relativePath + name + ".json");
		}
	}
}

void AssetImporter::ImportTexture(const std::string& assetPath, const std::string& metaPath)
{
	json j;
	j["type"] = "texture2d";
	j["mipmap"] = true;
	j["path"] = assetPath;
	
	std::string content = j.dump();
	StringUtils::WriteFile(metaPath, content);
}
