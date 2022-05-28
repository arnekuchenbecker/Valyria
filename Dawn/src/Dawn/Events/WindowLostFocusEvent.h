#pragma once

#include "Dawn/Events/Event.h"

namespace Dawn {

	class WindowLostFocusEvent : public Event
	{
	public: 
		WindowLostFocusEvent() {}

		EVENT_CLASS_TYPE(WINDOW_LOST_FOCUS)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_APPLICATION)
	};

}