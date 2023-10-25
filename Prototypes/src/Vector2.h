#pragma once

#include <cmath>

template<typename T>
class Vector2;
template<typename T>
Vector2<T> operator-(Vector2<T> lhs, const Vector2<T>& rhs);

template<typename T>
class Vector2 {
public:
	T x;
	T y;

	Vector2(T x = 0.0, T y = 0.0) : x(x), y(y) {

	}

	Vector2<T> operator-() const {
		return Vector2<T>(-x, -y);
	}

	Vector2<T>& operator+=(const Vector2<T>& other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	Vector2<T>& operator-=(const Vector2<T>& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	Vector2<T>& operator*=(T t) {
		x *= t;
		y *= t;
		return *this;
	}

	Vector2<T> getOrthogonal() const {
		return Vector2<T>(-y, x);
	}

	T getNorm() const {
		return std::sqrt(x * x + y * y);
	}

	T getDistance(const Vector2<T>& other) const {
		return (*this - other).getNorm();
	}

	T getDet(const Vector2<T>& other) const {
		return x * other.y - y * other.x;
	}
};

template<typename T>
Vector2<T> operator+(Vector2<T> lhs, Vector2<T> rhs) {
	lhs += rhs;
	return lhs;
}

template<typename T>
Vector2<T> operator-(Vector2<T> lhs, const Vector2<T>& rhs)
{
	lhs -= rhs;
	return lhs;
}

template<typename T>
Vector2<T> operator*(T t, Vector2<T> vec)
{
	vec *= t;
	return vec;
}

template<typename T>
Vector2<T> operator*(Vector2<T> vec, T t)
{
	return t * vec;
}