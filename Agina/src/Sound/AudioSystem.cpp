#include "AudioSystem.hpp"
#include <Core/Logger.hpp>

namespace Agina {

	static ma_engine g_Engine;

	void AudioSystem::Init()
	{
		ma_engine_init(NULL, &g_Engine);
	}

	void AudioSystem::ShutDown()
	{
		ma_engine_uninit(&g_Engine);
	}

	void AudioSystem::PlaySoundEffect(const std::filesystem::path& path)
	{
		if (!std::filesystem::exists(path))
		{
			AG_CORE_ERROR("Sound file missing! {}", path.string());
			return;
		}

		auto result = ma_engine_play_sound(&g_Engine, path.string().c_str(), NULL);
		if (result != MA_SUCCESS) AG_CORE_ERROR("FAILED TO PLAY SOUND EFFECT {0}", path.string());
	}
}
