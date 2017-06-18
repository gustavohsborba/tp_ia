#include "Vector.h"
#include <cmath>

float degToRad(float deg) {
	return deg * pi / 180.0f;
}

float radToDeg(float rad) {
	return rad * 180.0f / pi;
}

Vector::Vector() {
	x = 0;
	y = 0;
	z = 0;
}

Vector::Vector(float xi, float yi, float zi) {
	x = xi;
	y = yi;
	z = zi;
}

float Vector::magnitude2() {
    return x*x + y*y + z*z;
}

float Vector::magnitude() {
	return sqrtf(magnitude2());
}

void  Vector::normalize() {
	float m = (float) sqrt(x*x + y*y + z*z);
	if(m <= tol) m = 1;
	x /= m;
	y /= m;
	z /= m;	

	if (fabs(x) < tol) x = 0.0f;
	if (fabs(y) < tol) y = 0.0f;
	if (fabs(z) < tol) z = 0.0f;
}

void  Vector::reverse() {
	x = -x;
	y = -y;
	z = -z;
}

Vector& Vector::operator+=(Vector u) {
	x += u.x;
	y += u.y;
	z += u.z;
	return *this;
}

Vector& Vector::operator-=(Vector u) {
	x -= u.x;
	y -= u.y;
	z -= u.z;
	return *this;
}

Vector& Vector::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vector& Vector::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

Vector Vector::operator-() {
	return Vector(-x, -y, -z);
}


Vector operator+(Vector u, Vector v) {
	return Vector(u.x + v.x, u.y + v.y, u.z + v.z);
}

Vector operator-(Vector u, Vector v) {
	return Vector(u.x - v.x, u.y - v.y, u.z - v.z);
}

// Vector cross product (u cross v)
Vector operator^(Vector u, Vector v) {
	return Vector(	u.y*v.z - u.z*v.y,
					-u.x*v.z + u.z*v.x,
					u.x*v.y - u.y*v.x );
}

// Vector dot product
float operator*(Vector u, Vector v) {
	return (u.x*v.x + u.y*v.y + u.z*v.z);
}

Vector operator*(float s, Vector u) {
	return Vector(u.x*s, u.y*s, u.z*s);
}

Vector operator*(Vector u, float s) {
	return Vector(u.x*s, u.y*s, u.z*s);
}

Vector operator/(Vector u, float s) {
	return Vector(u.x/s, u.y/s, u.z/s);
}

Vector minWrapVector(Vector p, Vector q, float maxW, float maxH) {
    static const float px[9] = {1,1,0,-1,-1,-1,0,1,0};
    static const float py[9] = {0,1,1,1,0,-1,-1,-1,0};
    int bestI = 8;
    float smallest = (p - q).magnitude2();
    for(int i=0; i<8; ++i) {
        float option = (p - Vector(q.x + px[i] * maxW, q.y + py[i] * maxH, 0)).magnitude2();
        if(option < smallest) {
            bestI = i;
            smallest = option;
        }
    }
    return p - Vector(q.x+px[bestI]*maxW, q.y+py[bestI]*maxH,0);
}

Vector VRotate2D(float angle, Vector u) {
    float x, y;

    x = u.x * cosf(degToRad(-angle)) + u.y * sinf(degToRad(-angle));
    y = -u.x * sinf(degToRad(-angle)) + u.y * cosf(degToRad(-angle));

    return Vector(x, y, 0);
}