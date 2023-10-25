#pragma once

#include "Dawn/Events/Event.h"

namespace Dawn {
	
	class EventDispatcher
	{
		template<typename CellType>
		using EventFn = std::function<bool(CellType&)>;

	public:
		EventDispatcher(Event& event)
			: event(event)
		{
		}

		template<typename CellType>
		bool dispatch(EventFn<CellType> func)
		{
			if (event.getEventType() == CellType::getStaticType())
			{
				event.handled = func(*(CellType*)&event);
				return true;
			}
			return false;
		}

	private:
		Event& event;
	};

}