#ifndef _MYMATH
#define _MYMATH

#include <math.h>

//------------------------------------------------------------------------//
// Misc. Constants
//------------------------------------------------------------------------//

float	const	pi	= 3.14159265f;
float	const	rho = 0.0023769f;	// desity of air at sea level, slugs/ft^3
float	const	tol = 0.000000000000001f;		// float type tolerance 


//------------------------------------------------------------------------//
// Misc. Functions
//------------------------------------------------------------------------//
inline	float	DegreesToRadians(float deg);
inline	float	RadiansToDegrees(float rad);

inline	float	DegreesToRadians(float deg)
{
	return deg * pi / 180.0f;
}

inline	float	RadiansToDegrees(float rad)
{	
	return rad * 180.0f / pi;
}

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

	float Magnitude();
    float Magnitude2();
	void  Normalize();
	void  Reverse();

	Vector& operator+=(Vector u);	// vector addition
	Vector& operator-=(Vector u);	// vector subtraction
	Vector& operator*=(float s);	// scalar multiply
	Vector& operator/=(float s);	// scalar divide

	Vector operator-();
};

inline	Vector operator+(Vector u, Vector v);
inline	Vector operator-(Vector u, Vector v);
inline	Vector operator^(Vector u, Vector v);
inline	float operator*(Vector u, Vector v);
inline	Vector operator*(float s, Vector u);
inline	Vector operator*(Vector u, float s);
inline	Vector operator/(Vector u, float s);
inline	float TripleScalarProduct(Vector u, Vector v, Vector w);

inline Vector::Vector()
{
	x = 0;
	y = 0;
	z = 0;
}

inline Vector::Vector(float xi, float yi, float zi)
{
	x = xi;
	y = yi;
	z = zi;
}

inline float Vector::Magnitude2() {
    return x*x + y*y + z*z;
}

inline	float Vector::Magnitude() {
	return sqrt(Magnitude2());
}

inline	void  Vector::Normalize()
{
	float m = (float) sqrt(x*x + y*y + z*z);
	if(m <= tol) m = 1;
	x /= m;
	y /= m;
	z /= m;	

	if (fabs(x) < tol) x = 0.0f;
	if (fabs(y) < tol) y = 0.0f;
	if (fabs(z) < tol) z = 0.0f;
}

inline	void  Vector::Reverse()
{
	x = -x;
	y = -y;
	z = -z;
}

inline Vector& Vector::operator+=(Vector u)
{
	x += u.x;
	y += u.y;
	z += u.z;
	return *this;
}

inline	Vector& Vector::operator-=(Vector u)
{
	x -= u.x;
	y -= u.y;
	z -= u.z;
	return *this;
}

inline	Vector& Vector::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

inline	Vector& Vector::operator/=(float s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

inline	Vector Vector::operator-()
{
	return Vector(-x, -y, -z);
}


inline	Vector operator+(Vector u, Vector v)
{
	return Vector(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline	Vector operator-(Vector u, Vector v)
{
	return Vector(u.x - v.x, u.y - v.y, u.z - v.z);
}

// Vector cross product (u cross v)
inline	Vector operator^(Vector u, Vector v)
{
	return Vector(	u.y*v.z - u.z*v.y,
					-u.x*v.z + u.z*v.x,
					u.x*v.y - u.y*v.x );
}

// Vector dot product
inline	float operator*(Vector u, Vector v)
{
	return (u.x*v.x + u.y*v.y + u.z*v.z);
}

inline	Vector operator*(float s, Vector u)
{
	return Vector(u.x*s, u.y*s, u.z*s);
}

inline	Vector operator*(Vector u, float s)
{
	return Vector(u.x*s, u.y*s, u.z*s);
}

inline	Vector operator/(Vector u, float s)
{
	return Vector(u.x/s, u.y/s, u.z/s);
}

// triple scalar product (u dot (v cross w))
inline	float TripleScalarProduct(Vector u, Vector v, Vector w)
{
	return float(	(u.x * (v.y*w.z - v.z*w.y)) +
					(u.y * (-v.x*w.z + v.z*w.x)) +
					(u.z * (v.x*w.y - v.y*w.x)) );
	//return u*(v^w);

}

inline Vector minWrapVector(Vector p, Vector q, float maxW, float maxH){
    static const float px[9] = {1,1,0,-1,-1,-1,0,1,0};
    static const float py[9] = {0,1,1,1,0,-1,-1,-1,0};
    int bestI = 8;
    float smallest = (p-q).Magnitude2();
    for(int i=0; i<8; ++i){
        float option = (p - Vector(q.x+px[i]*maxW, q.y+py[i]*maxH,0)).Magnitude2();
        if(option < smallest){
            bestI = i;
            smallest = option;
        }
    }
    return p - Vector(q.x+px[bestI]*maxW, q.y+py[bestI]*maxH,0);
}

inline float minWrapDistance(Vector p, Vector q, float maxW, float maxH){
    return minWrapVector(p,q,maxW, maxH).Magnitude();
}





#endif