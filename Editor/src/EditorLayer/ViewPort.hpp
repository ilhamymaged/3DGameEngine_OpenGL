#pragma once

#include <Scene/scene.hpp>
#include <imgui.h>
#include <glm/glm.hpp>
#include <Renderer/FrameBuffer.hpp>
#include "SceneState.hpp"

namespace Agina
{
    void DrawViewPort(Scene*& activeScene, std::unique_ptr<Scene>& editorScene,
        Framebuffer* framebuffer, glm::vec2& viewportSize, SceneState& state);
}