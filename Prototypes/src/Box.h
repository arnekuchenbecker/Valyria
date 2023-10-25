#pragma once

#include <array>

#include "Vector2.h"
#include "Util.h"

template<typename T>
class Diagram;

template<typename T>
class FortuneAlgorithm;

template<typename T>
class Box {
public:
	T left;
	T bottom;
	T right;
	T top;

	bool contains(const Vector2<T>& point) const {
		return almostBetween(point.x, left, right) && almostBetween(point.y, bottom, top);
	}

private:
	friend Diagram<T>;
	friend FortuneAlgorithm<T>;

	enum class Side : int {Left, Bottom, Right, Top};

	struct Intersection {
		Side side;
		Vector2<T> point;
	};

	Intersection getFirstIntersection(const Vector2<T>& origin, const Vector2<T>& direction) const {
		auto intersection = Intersection{};
		auto t = std::numeric_limits<T>::infinity();
		if (direction.x > static_cast<T>(0.0)) {
			t = (right - origin.x) / direction.x;
			intersection.side = Side::Right;
			intersection.point = origin + t * direction;
		}
		else if (direction.x < static_cast<T>(0.0)) {
			t = (left - origin.x) / direction.x;
			intersection.side = Side::Left;
			intersection.point = origin + t * direction;
		}
		if (direction.y > static_cast<T>(0.0)) {
			auto newT = (top - origin.y) / direction.y;
			if (newT < t) {
				intersection.side = Side::Top;
				intersection.point = origin + newT * direction;
			}
		}
		else if (direction.y < static_cast<T>(0.0)) {
			auto newT = (bottom - origin.y) / direction.y;
			if (newT < t) {
				intersection.side = Side::Bottom;
				intersection.point = origin + newT * direction;
			}
		}
		return intersection;
	}

	int getIntersections(const Vector2<T>& origin, const Vector2<T>& destination, std::array<Intersection, 2>& intersections) const {
		auto direction = destination - origin;
		auto t = std::array<T, 2>();
		auto i = std::size_t(0);

		if (strictlyLower(origin.x, left) || strictlyLower(destination.x, left)) {
			t[i] = (left - origin.x) / direction.x;
			if (strictlyBetween(t[i], static_cast<T>(0.0), static_cast<T>(1.0))) {
				intersections[i].side = Side::Left;
				intersections[i].point = origin + t[i] * direction;

				if (almostBetween(intersections[i].point.y, bottom, top)) {
					i++;
				}
			}
		}

		if (strictlyGreater(origin.x, right) || strictlyGreater(destination.x, right)) {
			t[i] = (right - origin.x) / direction.x;
			if (strictlyBetween(t[i], static_cast<T>(0.0), static_cast<T>(1.0))) {
				intersections[i].side = Side::Right;
				intersections[i].point = origin + t[i] * direction;

				if (almostBetween(intersections[i].point.y, bottom, top)) {
					i++;
				}
			}
		}

		if (strictlyLower(origin.y, bottom) || strictlyLower(destination.y, bottom)) {
			t[i] = (bottom - origin.y) / direction.y;
			if (i < 2 && strictlyBetween(t[i], static_cast<T>(0.0), static_cast<T>(1.0))) {
				intersections[i].side = Side::Bottom;
				intersections[i].point = origin + t[i] * direction;

				if (almostBetween(intersections[i].point.x, left, right)) {
					i++;
				}
			}
		}

		if (strictlyGreater(origin.y, top) || strictlyGreater(destination.y, top)) {
			t[i] = (top - origin.y) / direction.y;
			if (i < 2 && strictlyBetween(t[i], static_cast<T>(0.0), static_cast<T>(1.0))) {
				intersections[i].side = Side::Top;
				intersections[i].point = origin + t[i] * direction;

				if (almostBetween(intersections[i].point.x, left, right)) {
					i++;
				}
			}
		}

		if (i == 2 && t[0] > t[1]) {
			std::swap(intersections[0], intersections[1]);
		}
		return i;
	}
};