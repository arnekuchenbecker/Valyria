#pragma once

#include "Dawn/Events/Event.h"
#include "Dawn/Events/MouseButtonEvent.h"

namespace Dawn {

	class MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string toString() const override
		{
			std::stringstream sstream;
			sstream << "MouseButtonReleasedEvent: " << button;
			return sstream.str();
		}

		EVENT_CLASS_TYPE(MOUSE_BUTTON_RELEASE)
	};

}
