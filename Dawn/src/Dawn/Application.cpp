#include "Application.h"

namespace Dawn {
	
	Application* Application::instance = nullptr;

	Application::Application()
	{
		instance = this;
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