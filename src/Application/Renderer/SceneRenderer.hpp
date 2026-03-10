#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

// Forward declarations
class Scene;
class Camera;
class Entity;

struct SceneData
{
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
    glm::vec3 cameraPosition;
};

class SceneRenderer
{
public:
    static void Render(Scene& scene, Camera& camera);

private:
    static void RenderEntity(Entity& entity);
    static SceneData* s_SceneData;
};


