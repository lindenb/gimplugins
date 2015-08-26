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

#endif
