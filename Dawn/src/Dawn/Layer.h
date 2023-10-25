#pragma once

#include "Dawn/Core.h"
#include "Dawn/Events/Event.h"

namespace Dawn {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate() {}
		virtual void onImGuiRender() {}
		virtual void onEvent(Event& event) {}

		inline const std::string& getName() const { return debugName; }

	protected:
		std::string debugName;
	};

}


