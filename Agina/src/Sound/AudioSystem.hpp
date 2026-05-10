#pragma once

#include <string>
#include <filesystem>
#include <unordered_map>	
#include <miniaudio/miniaudio.h>

namespace Agina {

	class AudioSystem
	{
	public:
		static void Init();
		static void ShutDown();

		static void PlaySoundEffect(const std::filesystem::path& path);
	};

}
