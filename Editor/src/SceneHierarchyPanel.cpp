#include "SceneHierarchyPanel.hpp"
#include <ECS/Components.hpp>
#include <UI/UI.hpp>
#include <Renderer/Renderer.hpp>

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
		
	UI::BeginWindow("Settings");

	bool wireframe = Renderer::GetWireFrameMode();

	if (UI::Checkbox("Enable Wireframe", &wireframe))
	{
		Renderer::SetWireFrameMode(wireframe);
	}

	bool shadowsEnabled = Renderer::GetShadowsEnabled();
	if (UI::Checkbox("Enable Shadows", &shadowsEnabled))
	{
		Renderer::SetShadowsEnabled(shadowsEnabled);
	}

	UI::EndWindow();

	UI::BeginWindow("Content Browser");
	UI::EndWindow();
}

void SceneHierarchyPanel::DrawEntityNode(Entity entity)
{
	auto& tag = entity.GetComponent<TagComponent>().tag;

	uint32_t entityID = static_cast<uint32_t>(entity.GetHandle());
	bool selected = (m_SelectionContext == entity);

	bool opened = UI::BeginEntityNode(tag, entityID, selected);

	if (UI::IsItemClicked()) m_SelectionContext = entity;
	if (opened) UI::EndEntityNode();
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
		UI::PushFont(1);
		auto& tag = entity.GetComponent<TagComponent>().tag;
		UI::PopFont();
		UI::InputText("##Tag", tag);
	}

	DrawComponent<Transform>("Transform", entity, [](auto& component)
		{
			if (UI::BeginPropertyGrid("TransformProperties"))
			{
				UI::DragVec3("Position", component.Position);
				UI::DragVec3("Rotation", component.Rotation);
				UI::DragVec3("Scale", component.Scale, 0.1f, -999.0f, 999.0f, 1.0f);
				UI::EndPropertyGrid();
			}
		});

	DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			UI::Checkbox("Primary Camera", &component.IsPrimary);

			const char* projectionTypeLabels[] = { "Perspective", "Orthographic" };
			auto& camera = component.Cam;
			Agina::CameraProjectionType currentProjType = camera.GetProjectionType();

			const char* currentLabel = projectionTypeLabels[static_cast<int>(currentProjType)];

			if (UI::BeginCombo("Projection", currentLabel))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = (currentLabel == projectionTypeLabels[i]);

					if (UI::Selectable(projectionTypeLabels[i], isSelected))
					{
						currentLabel = projectionTypeLabels[i];
						camera.SetProjectionType(static_cast<Agina::CameraProjectionType>(i));
					}

					if (isSelected)
						UI::SetItemDefaultFocus();
				}
				UI::EndCombo();
			}

			if (currentProjType == Agina::CameraProjectionType::PERSPECTIVE)
			{
				UI::DragFloat("Field of View", camera.GetRefFOV(), 0.5f, 1.0f, 120.0f);
				UI::DragFloat("Near Clip", camera.GetRefPerspectiveNear(), 0.05f, 0.001f, 10.0f);
				UI::DragFloat("Far Clip", camera.GetRefPerspectiveFar(), 1.0f, 10.0f, 5000.0f);
			}
			else
			{
				UI::DragFloat("Ortho Size", camera.GetRefOrthoSize(), 0.1f, 0.1f, 100.0f);
				UI::DragFloat("Near Clip", camera.GetRefOrthoNear(), 0.1f, -100.0f, 100.0f);
				UI::DragFloat("Far Clip", camera.GetRefOrthoFar(), 1.0f, 10.0f, 5000.0f);
			}
		});
}



