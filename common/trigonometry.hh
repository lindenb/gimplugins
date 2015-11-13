#ifndef MY_TRYGONOMETRY
#define MY_TRYGONOMETRY
#include <cmath>
#define PI (3.14159265359)
#define PI2 (2.0*PI)

inline double modulo_pi2(double angle)
	{
	while( angle > PI2 ) angle -= PI2;
	while( angle < 0 ) angle += PI2;
	return angle;
	}

struct Radian
	{
	double v;
	Radian(double v):v(v)
		{
		}
	Radian(const Radian& cp):v(cp.v)
		{
		}
	double operator()()const
		{
		return v;
		}
	Radian& operator += (double d)
		{
		v+=d;
		return *this;
		}
	Radian& operator += (const Radian& r)
		{
		v+= r.v;
		return *this;
		}
		
	Radian& operator = (double nv)
		{
		v = nv;
		return *this;
		}
		
	bool operator < (const Radian& r) const
		{
		return v < r.v;
		}
	double sin() const
		{
		return std::sin(v);
		}
	double cos() const
		{
		return std::cos(v);
		}
	};

#endif
