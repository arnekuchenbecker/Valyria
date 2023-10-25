#pragma once

#include "dwpch.h"

#include "Dawn/Core.h"
#include "Dawn/Events/Event.h"

namespace Dawn {

	struct WindowProperties
	{
		std::string title;
		unsigned int width;
		unsigned int height;

		WindowProperties(const std::string& title = "Dawn Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: title(title), width(width), height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void onUpdate() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual void isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static Window* createWindow(const WindowProperties& properties = WindowProperties());
	};

}