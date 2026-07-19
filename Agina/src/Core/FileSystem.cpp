#include "FileSystem.hpp"

#include <AginaConfig.hpp>

std::filesystem::path Agina::FileSystem::s_AssetRoot;

void Agina::FileSystem::Init(const std::filesystem::path& root)
{
    if (root.empty()) s_AssetRoot = std::filesystem::path(ENGINE_SOURCE_DIR);
    else s_AssetRoot = root;
}

std::filesystem::path Agina::FileSystem::EngineAssets()
{
	return s_AssetRoot / "Agina" / "assets";
}

std::filesystem::path Agina::FileSystem::AppAssets()
{
    return s_AssetRoot / "Editor" / "assets";
}

