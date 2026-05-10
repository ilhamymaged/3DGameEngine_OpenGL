#include "FileSystem.hpp"

std::filesystem::path Agina::FileSystem::s_AssetRoot;

void Agina::FileSystem::Init(const std::filesystem::path& root)
{
	s_AssetRoot = root;
}

std::filesystem::path Agina::FileSystem::GetAssetPath(const std::string& relative)
{
	return s_AssetRoot / relative;
}
