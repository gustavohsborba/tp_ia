#ifndef _MYMATH
#define _MYMATH

//------------------------------------------------------------------------//
// Constants
//------------------------------------------------------------------------//

float const pi = 3.14159265f;
float const rho = 0.0023769f;    // desity of air at sea level, slugs/ft^3
float const tol = 0.000000000000001f;        // float type tolerance

//------------------------------------------------------------------------//
// Angle functions
//------------------------------------------------------------------------//
float degToRad(float deg);
float radToDeg(float rad);

//------------------------------------------------------------------------//
// Vector Class and vector functions
//------------------------------------------------------------------------//
class Vector {
public:
    float x;
    float y;
    float z;

    Vector();
    Vector(float xi, float yi, float zi);

    float magnitude();
    float magnitude2();
    void normalize();
    void reverse();

    Vector &operator+=(Vector u);    // vector addition
    Vector &operator-=(Vector u);    // vector subtraction
    Vector &operator*=(float s);    // scalar multiply
    Vector &operator/=(float s);    // scalar divide

    Vector operator-();
};

Vector operator+(Vector u, Vector v);
Vector operator-(Vector u, Vector v);
Vector operator^(Vector u, Vector v);
float operator*(Vector u, Vector v);
Vector operator*(float s, Vector u);
Vector operator*(Vector u, float s);
Vector operator/(Vector u, float s);

Vector minWrapVector(Vector p, Vector q, float maxW, float maxH);
Vector VRotate2D(float angle, Vector u);

Vector getWrapPosition(Vector v, float maxW, float maxH);

#endif