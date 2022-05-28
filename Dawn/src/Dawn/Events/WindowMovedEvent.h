#pragma once

#include "Dawn/Events/Event.h"

namespace Dawn {

	class WindowMovedEvent : public Event
	{
	public:
		WindowMovedEvent(float windowX, float windowY)
			: windowX(windowX), windowY(windowY) {}

		inline float getX() const { return windowX; }
		inline float getY() const { return windowY; }

		std::string toString() const override
		{
			std::stringstream sstream;
			sstream << "WindowMovedEvent: " << windowX << ", " << windowY;
			return sstream.str();
		}

		EVENT_CLASS_TYPE(WINDOW_MOVED)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_APPLICATION)

	private:
		float windowX;
		float windowY;
	};

}