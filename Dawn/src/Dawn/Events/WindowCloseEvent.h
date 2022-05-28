#pragma once

#include "Dawn/Events/Event.h"

namespace Dawn {

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

		EVENT_CLASS_TYPE(WINDOW_CLOSE)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_APPLICATION)
	};

}