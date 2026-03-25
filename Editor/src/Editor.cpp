#include <EntryPoint.hpp>
#include <EditorLayer/EditorLayer.hpp>
#include <Core/Utility/LocU.hpp>

namespace Agina
{

	static std::string s_LogoPath = (GetEngineRoot() / "logos/logo0.jpg").string(); 
	class Editor : public Application
	{
	public:
		Editor() : Application("Agina Editor", 1280, 720, true, s_LogoPath) 
		{
			m_LayerStack.PushLayer(new EditorLayer());
		}

		void ShutDown() override { Application::ShutDown(); }
	};

	Application* CreateApplication()
	{
		return new Editor();
	}
}
