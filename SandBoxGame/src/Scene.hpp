#include <entt/entt.hpp>

class Scene
{
public:
    Scene() = default;

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&&) noexcept = default;
    Scene& operator=(Scene&&) noexcept = default;

    void OnCreate();
    void OnDestroy();

    inline entt::registry& GetRegistry() { return m_Registry; }

private:
    entt::registry m_Registry;
};