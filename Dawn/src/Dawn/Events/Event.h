#pragma once

#include "dwpch.h"

#include "Dawn/Core.h"

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::##type; }\
							   virtual EventType getEventType() const override { return getStaticType(); }\
							   virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

namespace Dawn {
	
	//Events are currently blocking. TODO: Implement some kind of Event Queue.

	enum class EventType
	{
		NONE = 0,
		WINDOW_CLOSE, WINDOW_RESIZE, WINDOW_FOCUS, WINDOW_LOST_FOCUS, WINDOW_MOVE,
		APP_TICK, APP_UPDATE, APP_RENDER,
		KEY_PRESS, KEY_RELEASE, KEY_TYPE,
		MOUSE_BUTTON_PRESS, MOUSE_BUTTON_RELEASE, MOUSE_MOVE, MOUSE_SCROLL
	};

	enum EventCategory
	{
		NONE = 0,
		EVENTCATEGORY_APPLICATION   = BIT(0),
		EVENTCATEGORY_INPUT         = BIT(1),
		EVENTCATEGORY_KEYBOARD      = BIT(2),
		EVENTCATEGORY_MOUSE         = BIT(3),
		EVENTCATEGORY_MOUSEBUTTON   = BIT(4)
	};

	class Event
	{
		friend class EventDispatcher;

	public:
		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		inline bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & category;
		}

		bool handled = false;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}

}