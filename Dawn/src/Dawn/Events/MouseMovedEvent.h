#pragma once

#include "Dawn/Events/Event.h"

namespace Dawn {

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: mouseX(x), mouseY(y) {}

		inline float getX() const { return mouseX; }
		inline float getY() const { return mouseY; }

		std::string toString() const override
		{
			std::stringstream sstream;
			sstream << "MouseMovedEvent: " << mouseX << ", " << mouseY;
			return sstream.str();
		}

		EVENT_CLASS_TYPE(MOUSE_MOVE)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_MOUSE | EVENTCATEGORY_INPUT)

	private:
		float mouseX;
		float mouseY;
	};

}
