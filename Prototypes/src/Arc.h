#pragma once

#include "Diagram.h"

template<typename T>
class Event;

template<typename T>
struct Arc {
	enum class Color{Red, Black};
	enum class Side{Left, Right};

	Arc<T>* parent;
	Arc<T>* left;
	Arc<T>* right;

	typename Diagram<T>::Site* site;
	typename Diagram<T>::HalfEdge* leftHalfEdge;
	typename Diagram<T>::HalfEdge* rightHalfEdge;
	Event<T>* event;

	Arc<T>* prev;
	Arc<T>* next;

	Color color;
	Side side;
};