#ifndef MYGEOMETRY_HH
#define MYGEOMETRY_HH


template<typename T>
struct Dimension
	{
	T width;
	T height;
	Dimension():width(0),height(0){}
	Dimension(T width,T height):width(width),height(height){}
	};

template<typename T>
struct Point
	{
	T x;
	T y;
	Point():x(0),y(0){}
	Point(T x,T y):x(x),y(y){}
	};
	
typedef Point<double> PointD;

template<typename T>
struct Rect
	{
	T x;
	T y;
	T width;
	T height;
	T centerX() const { return (T)(x+width/2.0); }
	T centerY() const { return (T)(y+height/2.0); }
	Point<T> center() const
		{
		Point<T> c(centerX(),centerY());
		return c;
		}
	Dimension<T> size() const
		{
		Dimension<T> d(width,height);
		return d;
		}
	};



#endif
