#pragma once

#include "Dawn/Events/Event.h"
#include "Dawn/Events/KeyEvent.h"

namespace Dawn {

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			: KeyEvent(keyCode) {}

		std::string toString() const override
		{
			std::stringstream sstream;
			sstream << "KeyReleasedEvent: " << keyCode;
			return sstream.str();
		}

		EVENT_CLASS_TYPE(KEY_RELEASE)
	};

}