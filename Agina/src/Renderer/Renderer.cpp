#include "Renderer.hpp"
#include "RenderBackend.hpp"

std::vector<Agina::RenderCommand> Agina::Renderer::s_CommandQueue;

void Agina::Renderer::Init()
{
	RenderBackend::Init();
}

void Agina::Renderer::Shutdown()
{
	RenderBackend::Shutdown();
}

void Agina::Renderer::BeginFrame()
{

}

void Agina::Renderer::EndFrame()
{
	Flush();
}

void Agina::Renderer::SubmitQuad(const glm::vec2 &pos, const glm::vec2 &size, Texture &texture)
{
	RenderCommand cmd;
    cmd.type = RenderCommand::Quad;
    cmd.position = pos;
    cmd.size = size;
    cmd.texture = &texture;

    s_CommandQueue.push_back(cmd);
}

void Agina::Renderer::Flush()
{
	for (auto& cmd : s_CommandQueue)
    {
        switch (cmd.type)
        {
            case RenderCommand::Quad:
                RenderBackend::DrawQuad(cmd.position, cmd.size, *cmd.texture);
                break;
        }
    }

    s_CommandQueue.clear();
}

void Agina::Renderer::SetClearColor(float r, float g, float b, float a)
{
	RenderBackend::SetClearColor(r, g, b, a);
}

void Agina::Renderer::Clear()
{
	RenderBackend::Clear();
}
