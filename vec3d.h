#include <math.h>
#include <iostream>

class vec3d {
	public:
		// this variables
		double x;
		double y;
		double z;

		// initializations
		vec3d() : x(0), y(0), z(0) {};
		vec3d(double x, double y, double z) : x(x), y(y), z(z) {};

		// function definitions
		vec3d add(vec3d otherVec);
		vec3d subtract(vec3d otherVec);
		double magnitude();
		vec3d normalize();
		double dot(vec3d otherVec);
		vec3d cross(vec3d otherVec);
		vec3d scalarMul(double scalar);
		vec3d scalarDiv(double scalar);
		vec3d scalarAdd(double scalar);
		vec3d scalarSubtract(double scalar);
};

// add this vector to another (element-wise adding)
vec3d vec3d::add(vec3d otherVec) {
	vec3d outVec;

	outVec.x = this->x + otherVec.x;
	outVec.y = this->y + otherVec.y;
	outVec.z = this->z + otherVec.z;

	return outVec;
}

// add scalar to each entry of vector
vec3d vec3d::scalarAdd(double scalar) {
	vec3d outVec;

	outVec.x = this->x + scalar;
	outVec.y = this->y + scalar;
	outVec.z = this->z + scalar;

	return outVec;
}

// subtract scalar from each entry of vector
vec3d vec3d::scalarSubtract(double scalar) {
	vec3d outVec;

	outVec.x = this->x - scalar;
	outVec.y = this->y - scalar;
	outVec.z = this->z - scalar;

	return outVec;
}

// subtract another vector from this vector (element-wise subtraction)
vec3d vec3d::subtract(vec3d otherVec) {
	vec3d outVec;

	outVec.x = this->x - otherVec.x;
	outVec.y = this->y - otherVec.y;
	outVec.z = this->z - otherVec.z;

	return outVec;
}

// calculate magnitude of this vector (sqrt(x^2 + y^2 + z^2))
double vec3d::magnitude() {
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}

// calculate this unit vector (normalize)
vec3d vec3d::normalize() {
	double mag = magnitude();
	vec3d normVec(this->x / mag, this->y / mag, this->z / mag);
	return normVec;
}

// compute dot product between this vector and another
double vec3d::dot(vec3d otherVec) {
	return this->x * otherVec.x + this->y * otherVec.y + this->z * otherVec.z;
}

vec3d vec3d::cross(vec3d otherVec) {
	vec3d outVec;

	outVec.x = this->y * otherVec.z - this->z * otherVec.y;
	outVec.y = this->z * otherVec.x - this->x * otherVec.z;
	outVec.z = this->x * otherVec.y - this->y * otherVec.x;

	return outVec;
}

// multiply this vector by a scalar 
vec3d vec3d::scalarMul(double scalar) {
	vec3d outVec;

	outVec.x = x * scalar;
	outVec.y = y * scalar;
	outVec.z = z * scalar;

	return outVec;
}

// divide this vector by a scalar
vec3d vec3d::scalarDiv(double scalar) {
	vec3d outVec;

	outVec.x = x / scalar;
	outVec.y = y / scalar;
	outVec.z = z / scalar;

	return outVec;
}

// when the operator << is used on a vec3d, output 
// NOTE can also change operators for other stuff (i.e. do a vec3d + vec3d instead of vec3d.add(vec3d))
inline std::ostream& operator<<(std::ostream &out, const vec3d &v) {
	return out << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}

// aliases for points and colors
using point3d = vec3d;
using color = vec3d;
 
