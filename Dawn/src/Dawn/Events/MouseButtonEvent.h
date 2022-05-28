#pragma once

#include "Dawn/Events/Event.h"

namespace Dawn {

	class MouseButtonEvent : public Event
	{
	public:
		inline int getMouseButton() const { return button; }

		EVENT_CLASS_CATEGORY(EVENTCATEGORY_MOUSE | EVENTCATEGORY_MOUSEBUTTON | EVENTCATEGORY_INPUT)

	protected:
		MouseButtonEvent(int button)
			: button(button) {}

		int button;
	};

}