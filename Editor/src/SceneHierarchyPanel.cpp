#include "SceneHierarchyPanel.hpp"
#include <ECS/Components.hpp>
#include <UI/UI.hpp>
#include <Renderer/Renderer.hpp>
#include <Renderer\Mesh.hpp>
#include <Core/FileSystem.hpp>

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
				if (entity.IsValid()) DrawEntityNode(entity);
			});

		if (UI::IsMouseDown(0) && UI::IsWindowHovered()) // Left Click
		{
			m_SelectionContext = Entity{};
		}

		if (UI::IsMouseDown(1) && UI::IsWindowHovered() && !UI::IsAnyItemHovered()) //Right Click
		{
			UI::OpenPopup("CreateEntityMenu");
		}

		if (UI::BeginPopup("CreateEntityMenu"))
		{
			if (UI::MenuItem("Create Empty Entity"))
			{
				Entity New = m_Context->CreateEntity();
				m_SelectionContext = New;
			}

			if (UI::BeginMenu("Create 3D Object"))
			{
				if (UI::MenuItem("Cube"))
				{
					Entity newCube = m_Context->CreateEntity("Cube");
					newCube.AddComponent<MeshComponent>();
					newCube.AddComponent<BoxCollider>();
					newCube.AddComponent<Rigidbody>();
					m_SelectionContext = newCube;
				}
				if (UI::MenuItem("Sphere"))
				{
					Entity newSphere = m_Context->CreateEntity("Sphere");  
					newSphere.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::SPHERE));
					m_SelectionContext = newSphere;
				}
				if (UI::MenuItem("Grid"))
				{
					Entity newGrid = m_Context->CreateEntity("Grid");
					newGrid.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::GRID),
						Material::Create(MaterialType::GRID));
					m_SelectionContext = newGrid;
				}
				if (UI::MenuItem("Terrain"))
				{
					Entity newTerrain = m_Context->CreateEntity("Terrain");
					newTerrain.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::TERRAIN),
						Material::Create(MaterialType::LIT));
					m_SelectionContext = newTerrain;
				}
				UI::EndMenu();
			}

			if (UI::BeginMenu("Create 2D Object"))
			{
				if (UI::MenuItem("Quad"))
				{
					Entity newQuad = m_Context->CreateEntity("Quad");
					newQuad.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::QUAD),
						Material::Create(MaterialType::LIT));
					m_SelectionContext = newQuad;
				}
				if (UI::MenuItem("Triangle"))
				{
					Entity newTriangle = m_Context->CreateEntity("Triangle");
					newTriangle.AddComponent<MeshComponent>(AssetManager::LoadMesh(MeshType::TRIANGLE),
						Material::Create(MaterialType::LIT));
					m_SelectionContext = newTriangle;
				}

				UI::EndMenu();
			}

			if (UI::BeginMenu("Create Camera"))
			{
				if (UI::MenuItem("3D Camera"))
				{
					Entity new3DCamera = m_Context->CreateEntity("3D Camera");
					new3DCamera.AddComponent<CameraComponent>(Camera(), false);
					m_SelectionContext = new3DCamera;
				}
				if (UI::MenuItem("2D Camera"))
				{
					Entity new2DCamera = m_Context->CreateEntity("2D Camera");
					new2DCamera.AddComponent<CameraComponent>(Camera().SetProjectionType(
					CameraProjectionType::ORTHO), false);
					m_SelectionContext = new2DCamera;
				}


				UI::EndMenu();
			}

			if (UI::MenuItem("SkyBox"))
			{
				std::string assetPath = (FileSystem::AppAssets()).string();
				auto newSkyBox = m_Context->CreateEntity("SkyBox");
				std::vector<std::string> facePaths = 
				{
					assetPath + "/skyboxes/sky1/" + "right.jpg",
					assetPath + "/skyboxes/sky1/" + "left.jpg",
					assetPath + "/skyboxes/sky1/" + "top.jpg",
					assetPath + "/skyboxes/sky1/" + "bottom.jpg",
					assetPath + "/skyboxes/sky1/" + "front.jpg",
					assetPath + "/skyboxes/sky1/" + "back.jpg",
				};
				std::Ref<CubemapTexture> c = std::make_Ref<CubemapTexture>(facePaths);
				newSkyBox.AddComponent<SkyboxComponent>(std::make_Ref<Skybox>(c),
					Material::Create(MaterialType::SKYBOX));
				m_SelectionContext = newSkyBox;
			}

			UI::EndPopup();
		}

		if (UI::BeginPopup("EntityContextMenu"))
		{
			if (UI::MenuItem("Delete Entity"))
			{
				if (m_SelectionContext.IsValid())
				{
					m_Context->DestroyEntity(m_SelectionContext);
					m_SelectionContext = Entity{};
				}
			}
			UI::EndPopup();
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

	if (UI::IsItemClicked(0))
	{
		m_SelectionContext = entity;
	}

	if (UI::IsItemClicked(1))
	{
		m_SelectionContext = entity;
		UI::OpenPopup("EntityContextMenu");
	}

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
	//Every Entity always Has a Tag Component
	UI::PushFont(1);
	auto& tag = entity.GetComponent<TagComponent>().tag;
	UI::PopFont();
	UI::InputText("##Tag", tag);

	//Add Component
	UI::SameLine();
	if (UI::Button("Add Component"))
	{
		UI::OpenPopup("AddNewComponent");
	}
	
	if (UI::BeginPopup("AddNewComponent"))
	{
		AllComponents::ForEach([&]<typename T>()
		{
			if (!entity.HasComponent<T>())
			{
				if (UI::MenuItem(ComponentName<T>()))
				{
					entity.AddComponent<T>();
				}
			}
		});

		UI::EndPopup();
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

	DrawComponent<MeshComponent>("Mesh", entity, [](auto& component)
		{
			UI::Text(Mesh::MeshTypeToString(component.MeshAsset->GetMeshType()));
			if (UI::BeginPropertyGrid("Material"))
			{
				auto& colorVariant = component.MaterialAsset->Get("u_Color");
				if (!std::holds_alternative<Vec3>(colorVariant)) 
				{
					colorVariant = Vec3(1.0f); 
				}

				auto& HasColorVariant = component.MaterialAsset->Get("u_HasColor");
				if (!std::holds_alternative<bool>(HasColorVariant))
				{
					HasColorVariant = false;
				}

				UI::DragVec3("Color", std::get<Vec3>(colorVariant), 0.01f, 0.0f, 1.0f, 0.0f);
				UI::Checkbox("HasColor", &std::get<bool>(HasColorVariant));
				UI::EndPropertyGrid();
			}
		});

	DrawComponent<BoxCollider>("Box Collider", entity, [](auto& component)
		{
			if (UI::BeginPropertyGrid("BoxColliderProperties"))
			{
				UI::DragVec3("Local Min", component.localMin);
				UI::DragVec3("Local Max", component.localMax);
				UI::EndPropertyGrid();
			}
		}
	);

	DrawComponent<Rigidbody>("Rigidbody", entity, [](auto& component)
		{
			if (UI::BeginPropertyGrid("RigidbodyProperties"))
			{
				UI::Checkbox("Static", &component.isStatic);
				UI::DragFloat("Mass",component.mass, 0.1f, 0.001f, 10000.0f);
				UI::DragFloat("Gravity Scale", component.gravityScale, 0.1f, -100.0f, 100.0f);
				UI::DragVec3("Velocity", component.velocity);
				UI::DragVec3("Acceleration",component.acceleration);
				UI::DragVec3("Force", component.forceAccumulator);
				UI::EndPropertyGrid();
			}
		}
	);

	DrawComponent<DirectionalLightComponent>("Light", entity, [](auto& component)
		{
			if (UI::BeginPropertyGrid("LightProperties"))
			{
				UI::DragVec3("Target", component.Target);
				UI::EndPropertyGrid();
			}
		});

	DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
		{
			UI::Checkbox("Primary Camera", &component.IsPrimary);

			const char* projectionTypeLabels[] = { "Perspective", "Orthographic" };
			auto& camera = component.Cam;
			Agina::CameraProjectionType currentProjType = camera.GetProjectionType();

			UI::DragFloat("Speed", camera.GetRefSpeed(), 1.0f, -999.9f, 999.0f);

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



