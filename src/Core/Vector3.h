#pragma once

/*
	Class to represent a vector and its operation in 3D

	@author Fernando del Molino
*/
class Vector3
{
public:
	float x, y, z;

public:
	Vector3() : x(0), y(0), z(0) {

	}

	Vector3(float x, float y, float z) : x(x), y(y), z(z) {

	}

	/* Asignation operation */

	Vector3& operator=(const Vector3& b) {
		this->x = b.x;
		this->y = b.y;
		this->z = b.z;
	}

	/* Adition and substraction*/
	Vector3 operator-() const {
		Vector3 aux = *this;
		aux.x = -x;
		aux.y = -y;
		aux.z = -z;
	}

	Vector3 operator+(const Vector3& b) const {
		Vector3 aux;
		aux.x = this->x + b.x;
		aux.y = this->y + b.y;
		aux.z = this->z + b.z;
	}

	Vector3 operator-(const Vector3& b) const {
		Vector3 aux;
		aux.x = this->x - b.x;
		aux.y = this->y - b.y;
		aux.z = this->z - b.z;
	}

	/* Vector3 * scalar */
	Vector3 operator*(const float& b) const {
		Vector3 aux;
		aux.x = this->x * b;
		aux.y = this->y * b;
		aux.z = this->z * b;
	}

	Vector3 operator/(const float& b) const {
		Vector3 aux;
		aux.x = this->x / b;
		aux.y = this->y / b;
		aux.z = this->z / b;
	}

	/* Vector3 * Vector3 */
	Vector3 operator*(const Vector3& b) const {
		Vector3 aux;
		aux.x = this->x * b.x;
		aux.y = this->y * b.y;
		aux.z = this->z * b.z;
	}

	Vector3 operator/(const Vector3& b) const {
		Vector3 aux;
		aux.x = this->x / b.x;
		aux.y = this->y / b.y;
		aux.z = this->z / b.z;
	}

	/* Dot product */
	static float Dot(const Vector3& a, const Vector3& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	/* Cross product */
	static Vector3 Cross(const Vector3& a, const Vector3& b) {
		Vector3 resul;
		resul.x = a.y * b.z - a.z * b.y;
		resul.y = a.z * b.x - a.x * b.z;
		resul.z = a.x * b.y - a.y * b.x;
		return resul;
	}
};

