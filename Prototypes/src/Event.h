#pragma once

#include "Vector2.h"
#include "Diagram.h"

template<typename T>
class Arc;

template<typename T>
class Event {
public:
	enum class Type{Site, Circle};

	explicit Event(typename Diagram<T>::Site* site) : type(Type::Site), x(site->point.x), y(site->point.y), index(-1), site(site) {

	}

	Event(T y, Vector2<T> point, Arc<T>* arc) : type(Type::Circle), x(point.x), y(y), index(-1), point(point), arc(arc) {

	}

	const Type type;
	T x;
	T y;
	int index;

	typename Diagram<T>::Site* site;

	Vector2<T> point;
	Arc<T>* arc;
};

template<typename T>
bool operator<(const Event<T>& lhs, const Event<T>& rhs) {
	return lhs.y < rhs.y || (lhs.y == rhs.y && lhs.x < rhs.x);
}