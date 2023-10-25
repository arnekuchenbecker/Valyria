#pragma once

#include "Dawn/Core.h"

#include "Dawn/Window.h"
#include "Dawn/Events/WindowCloseEvent.h"

namespace Dawn {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void run();

		void onEvent(Event& e);

		//void pushLayer(Layer* layer);
		//void pushOverlay(Layer* overlay);

		inline Window& getWindow() { return *window; }

		static inline Application& get() { return *instance; }
	private:
		bool onWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> window;
		//ImGuiLayer* imGuiLayer;
		bool running = true;
		//LayerStack layerStack;

		static Application* instance;
	};

	Application* createApplication();

}
