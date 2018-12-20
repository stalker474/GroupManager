#ifndef MATH_H
#define MATH_H
#define _USE_MATH_DEFINES
#include <math.h>

template<typename T>
struct Vector2
{
	T x, y;

	void normalize()
	{
		double l = length();
		x /= l;
		y /= l;
	}

	Vector2<T> normalized() const
	{
		Vector2d copy = *this;
		copy.normalize();
		return copy;
	}

	T length() const
	{
		return sqrt(x*x + y*y);
	}

	void rotate(const double Theta)
	{
		double cs = cos(Theta);
		double sn = sin(Theta);

		double newX = x * cs - y * sn;
		double newY = x * sn + y * cs;
		x = newX;
		y = newY;
	}

	double dot(const Vector2<T>& v) const
	{
		return x*v.x + y*v.y;
	}

	double angle(const Vector2<T>& v) const
	{
		return atan2(v.y, v.x) - atan2(y, x);
	}

	Vector2<T> operator*(const double value) const
	{
		return { x*value,y*value };
	}

	Vector2<T> operator+(const Vector2<T>& value) const
	{
		return { x + value.x , y + value.y };
	}
};

using Vector2d = Vector2<double>;
using Vector2i = Vector2<int>;
#endif