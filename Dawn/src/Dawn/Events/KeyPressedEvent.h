#pragma once

#include "Dawn/Events/Event.h"
#include "Dawn/Events/KeyEvent.h"

namespace Dawn {

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			: KeyEvent(keyCode), repeatCount(repeatCount) {}

		inline int getRepeatCount() const { return repeatCount; }

		std::string toString() const override
		{
			std::stringstream sstream;
			sstream << "KeyPressedEvent: " << keyCode << " (" << repeatCount << " repeats)";
			return sstream.str();
		}

		EVENT_CLASS_TYPE(KEY_PRESS)

	private:
		int repeatCount;
	};

}