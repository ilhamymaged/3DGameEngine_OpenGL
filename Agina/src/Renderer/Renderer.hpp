#include "RenderCommand.hpp"
#include <vector>
#include <glm/glm.hpp>
#include "Texture.hpp"

namespace Agina {
	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();

		static void SubmitQuad(const glm::vec2& pos,
                           const glm::vec2& size,
                           Texture& texture);


		static void SetClearColor(float r, float g, float b, float a);
		static void Clear();

	private:
		static void Flush();

	private:
		static std::vector<RenderCommand> s_CommandQueue;

	};
}

