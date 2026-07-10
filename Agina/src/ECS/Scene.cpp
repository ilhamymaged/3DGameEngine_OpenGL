#include "Scene.hpp"
#include <vector>

namespace Agina {

    Entity Scene::CreateEntity()
    {
        return Entity{ m_Registry.create(), &m_Registry };
    }

}

