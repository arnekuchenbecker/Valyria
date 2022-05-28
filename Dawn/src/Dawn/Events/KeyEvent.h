#pragma once

#include "Dawn/Events/Event.h"

namespace Dawn {

	class KeyEvent : public Event
	{
	public:
		inline int getKeyCode() const { return keyCode; }

		EVENT_CLASS_CATEGORY(EVENTCATEGORY_KEYBOARD | EVENTCATEGORY_INPUT)

	protected:
		KeyEvent(int keyCode)
			: keyCode(keyCode) {}

		int keyCode;
	};

}