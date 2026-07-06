#include "FileSystem.hpp"

#include <Core/Logger/Logger.hpp>

std::filesystem::path Agina::FileSystem::s_AssetRoot;

void Agina::FileSystem::Init(const std::filesystem::path& root)
{
	s_AssetRoot = root;
}

std::filesystem::path Agina::FileSystem::EngineAssets()
{
	return s_AssetRoot / "Agina" / "assets";
}

