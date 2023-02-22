#include "Vector2f.h"

// Default constructor with vector (0, 0)
Vector2f::Vector2f()
{
	myX = 0.0f;
	myY = 0.0f;
}

// Parameterized construction
Vector2f::Vector2f(float _x, float _y)
{
	myX = _x;
	myY = _y;
}


// Subtract one vector from another
const Vector2f Vector2f::operator-(const Vector2f& other) const
{
	Vector2f v(myX - other.myX, myY - other.myY);
	return v;
}

// Add one vector and another
const Vector2f Vector2f::operator+(const Vector2f& other) const
{
	Vector2f v(myX + other.myX, myY + other.myY);
	return v;
}

// Dot product
const Vector2f Vector2f::operator*(const Vector2f& other) const
{
	Vector2f v(myX * other.myX, myY * other.myY);
	return v;
}


// Scalar product with float
const Vector2f Vector2f::operator*(const float other) const
{
	Vector2f v(myX * other, myY * other);
	return v;
}

// Scalar product with int
const Vector2f Vector2f::operator*(const int other) const
{
	Vector2f v(myX * other, myY * other);
	return v;
}


// Divide a vector by an int
const Vector2f Vector2f::operator/(const int other) const
{
	Vector2f v(myX / other, myY / other);
	return v;
}


// Syntax sugar for + operation
Vector2f& Vector2f::operator+=(const Vector2f& other)
{
	myX = myX + other.myX;
	myY = myY + other.myY;

	return *this;
}

// Syntax sugar for scalar production operation
Vector2f& Vector2f::operator*=(const float other)
{
	myX *= other;
	myY *= other;

	return *this;
}

// Syntax sugar for / operation
Vector2f& Vector2f::operator/=(const float other)
{
	myX /= other;
	myY /= other;

	return *this;
}

// Check if 2 vectors are equal
const bool Vector2f::operator==(const Vector2f& other) const
{
	if (myX == other.myX && myY == other.myY)
	{
		return true;
	}
	return false;
}

// Check if 2 vectors are not equal
const bool Vector2f::operator!=(const Vector2f& other) const
{
	if (myX != other.myX && myY != other.myY)
	{
		return true;
	}
	return false;
}

// Get the length of vector
float Vector2f::Length() const
{
	return sqrt(myX * myX + myY * myY);
}

// Normalize the vector
void Vector2f::Normalize()
{
	float length = Length();

	if (length > 0.f)
		*this /= length;
}