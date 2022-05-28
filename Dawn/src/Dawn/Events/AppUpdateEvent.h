#pragma once

#include "Dawn/Events/Event.h"

namespace Dawn {

	class AppUpdateEvent : public Event
	{
	public:
		AppUpdateEvent() {}

		EVENT_CLASS_TYPE(APP_UPDATE)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_APPLICATION)
	};

}