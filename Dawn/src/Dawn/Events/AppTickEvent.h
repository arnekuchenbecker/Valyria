#pragma once

#include "Dawn/Events/Event.h"

namespace Dawn {

	class AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

		EVENT_CLASS_TYPE(APP_TICK)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_APPLICATION)
	};

}