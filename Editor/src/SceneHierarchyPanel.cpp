#include "SceneHierarchyPanel.hpp"
#include <ECS/Components.hpp>
#include <UI/UI.hpp>

SceneHierarchyPanel::SceneHierarchyPanel(Scene* sceneContext)
{
	SetContext(sceneContext);
}

void SceneHierarchyPanel::SetContext(Scene* sceneContext)
{
	m_Context = sceneContext;
	m_SelectionContext = Entity{}; 
}

void SceneHierarchyPanel::OnUIRender()
{
	UI::BeginWindow("Scene Hierarchy");

	if (m_Context)
	{
		m_Context->EachEntity([&](Entity entity)
			{
				DrawEntityNode(entity);
			});

		if (UI::IsMouseDown(0) && UI::IsWindowHovered())
		{
			m_SelectionContext = Entity{};
		}
	}

	UI::EndWindow();
		
	UI::BeginWindow("Properties");

	if (m_SelectionContext.IsValid())
	{
		DrawComponents(m_SelectionContext);
	}

	UI::EndWindow();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
	auto& tag = entity.GetComponent<TagComponent>().tag;

	uint32_t entityID = static_cast<uint32_t>(entity.GetHandle());
	bool selected = (m_SelectionContext == entity);

	bool opened = UI::BeginEntityNode(tag, entityID, selected);

	if (UI::IsItemClicked())
	{
		m_SelectionContext = entity;
	}

	if (opened)
	{
		UI::EndEntityNode();
	}
}

template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
{
	if (!entity.HasComponent<T>()) return;

	bool removeComponent = false;
	bool open = UI::BeginComponentHeader(name, typeid(T).hash_code(), removeComponent);

	if (open)
	{
		uiFunction(entity.GetComponent<T>());
		UI::EndComponentHeader();
	}

	if (removeComponent)
	{
		entity.RemoveComponent<T>();
	}
}

void SceneHierarchyPanel::DrawComponents(Entity entity)
{
	if (entity.HasComponent<TagComponent>())
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;
		UI::InputText("##Tag", tag);
	}

	DrawComponent<Transform>("Transform", entity, [](auto& component)
		{
			UI::DragVec3("Position", component.Position);
			UI::DragVec3("Rotation", component.Rotation); 
			UI::DragVec3("Scale", component.Scale);
		});

	DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			UI::Checkbox("Primary Camera", &component.IsPrimary);
		});

	//DrawComponent<Rigidbody>("Rigidbody", entity, [](auto& component)
	//	{
	//		UI::Checkbox("Is Static", &component.isStatic);
	//		UI::DragVec3("Velocity", component.velocity);
	//		UI::DragVec3("Acceleration", component.acceleration);
	//	});

	// Box Collider
	//DrawComponent<BoxCollider>("Box Collider", entity, [](auto& component)
	//	{
	//		UI::DragVec3("Local Min", component.localMin);
	//		UI::DragVec3("Local Max", component.localMax);
	//	});
}



