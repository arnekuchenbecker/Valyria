#pragma once

#include "Dawn/Events/Event.h"

namespace Dawn {

	class WindowFocusEvent : public Event
	{
	public:
		WindowFocusEvent() {}

		EVENT_CLASS_TYPE(WINDOW_FOCUS)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_APPLICATION)
	};

}