#pragma once

#include "Dawn/Events/Event.h"

namespace Dawn {

	class AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

		EVENT_CLASS_TYPE(APP_RENDER)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_APPLICATION)
	};

}