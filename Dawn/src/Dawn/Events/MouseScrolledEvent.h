#pragma once

#include "Dawn/Events/Event.h"

namespace Dawn {

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: xOffset(xOffset), yOffset(yOffset) {}

		inline float getXOffset() const { return xOffset; }
		inline float getYOffset() const { return yOffset; }

		std::string toString() const override
		{
			std::stringstream sstream;
			sstream << "MouseScrolledEvent: " << xOffset << ", " << yOffset;
			return sstream.str();
		}

		EVENT_CLASS_TYPE(MOUSE_SCROLL)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_MOUSE | EVENTCATEGORY_INPUT)

	private:
		float xOffset;
		float yOffset;
	};

}