#pragma once

#include "Dawn/Events/Event.h"
#include "Dawn/Events/MouseButtonEvent.h"

namespace Dawn {

	class MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string toString() const override
		{
			std::stringstream sstream;
			sstream << "MouseButtonPressedEvent: " << button;
			return sstream.str();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_PRESS)
	};

}