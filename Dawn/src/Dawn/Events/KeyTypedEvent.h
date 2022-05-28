#pragma once

#include "Dawn/Events/Event.h"
#include "Dawn/Events/KeyEvent.h"

namespace Dawn {

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		std::string toString() const override
		{
			std::stringstream sstream;
			sstream << "KeyTypedEvent: " << keyCode;
			return sstream.str();
		}

		EVENT_CLASS_TYPE(KEY_TYPE)
	};

}