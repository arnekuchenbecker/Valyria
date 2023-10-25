#include "dwpch.h"

#include "Application.h"
#include "Dawn/Events/EventDispatcher.h"

namespace Dawn {
	
	Application* Application::instance = nullptr;

	Application::Application()
	{
		DW_CORE_ASSERT(!instance, "Application already exists!");
		instance = this;

		window = std::unique_ptr<Window>(Window::createWindow());
	}

	Application::~Application()
	{

	}

	bool Application::onWindowClose(WindowCloseEvent& e)
	{
		running = false;
		return true;
	}

}