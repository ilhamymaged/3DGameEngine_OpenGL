#pragma once

#include <ECS/Scene.hpp>
#include <ECS/Entity.hpp>
#include <memory>

using namespace Agina;
class SceneHierarchyPanel
{
public:
	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(Scene* sceneContext);

	void SetContext(Scene* sceneContext);
	void OnUIRender();

	Entity GetSelectedEntity() const { return m_SelectionContext; }
	void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }

private:
	void DrawEntityNode(Entity entity);
	void DrawComponents(Entity entity);

private:
	Scene* m_Context = nullptr;
	Entity m_SelectionContext;
};



