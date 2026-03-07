#include <scene/scene.hpp>

SceneManager::SceneManager(int width, int height, Input& i) 
{
    Transform transform;
    std::shared_ptr<Model> model = std::make_shared<Model>("../../../assets/models/backpack/backpack.obj");
    //player = std::make_unique<Player>(model, transform, i);

    std::vector<Entity> entities; 
    entities.emplace_back(model, transform);
    
    currentScene = std::make_unique<World>(entities);
    camera = std::make_unique<Camera>(width, height, i);
}

void SceneManager::onEvent(std::shared_ptr<Event> e)
{
    camera->onEvent(e);
}

void SceneManager::Update(float deltaTime)
{
    if (currentScene)
    {
        currentScene->update(deltaTime);
        camera->Update(deltaTime);
    }
}

void SceneManager::setCurrentScene(std::unique_ptr<World> scene)
{
    currentScene = std::move(scene);
}

std::unique_ptr<World> SceneManager::GetCurrentScene() const
{
    return std::make_unique<World>(*currentScene);
}
