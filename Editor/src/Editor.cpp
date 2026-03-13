#include <EntryPoint.hpp>
#include <EditorLayer/EditorLayer.hpp>

namespace Agina
{
	class Editor : public Application
	{
	public:
		Editor() : Application("Agina Editor", 1280, 720, true) 
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