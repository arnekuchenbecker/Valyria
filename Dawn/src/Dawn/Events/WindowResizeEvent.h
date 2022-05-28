#pragma once

#include "Dawn/Events/Event.h"

namespace Dawn {

	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: width(width), height(height) {}

		inline unsigned int getWidth() const { return width; }
		inline unsigned int getHeight() const { return height; }

		std::string toString() const override
		{
			std::stringstream sstream;
			sstream << "WindowResizeEvent: " << width << ", " << height;
			return sstream.str();
		}

		EVENT_CLASS_TYPE(WINDOW_RESIZE)
		EVENT_CLASS_CATEGORY(EVENTCATEGORY_APPLICATION)

	private:
		unsigned int width;
		unsigned int height;
	};

}