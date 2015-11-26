#ifndef MYGEOMETRY_HH
#define MYGEOMETRY_HH
#include <iostream>

template<typename T>
struct Point
	{
	T x;
	T y;
	Point():x(0),y(0){}
	Point(T x,T y):x(x),y(y){}
	double distance(T x2,T y2) const
		{
		return std::sqrt(std::pow(x2-x,2)+std::pow(y2-y,2));
		}
	double distance(const Point<T>& o) const
		{
		return distance(o.x,o.y);
		}
	};


template<typename T>
std::ostream& operator << (std::ostream&  os,const Point<T> &o)
{
os << "Point(" << o.x << "," << o.y << ")" ;
return os;
} 

typedef Point<double> PointD;


template<typename T>
struct Dimension
	{
	T width;
	T height;
	Dimension():width(0),height(0){}
	Dimension(T width,T height):width(width),height(height){}
	bool contains(T ax,T ay) const
		{
		if(ax > width ) return false;
		if(ay > height ) return false;
		return true;
		}
	bool contains(const Point<T>& pt) const
		{
		return contains(pt.x,pt.y);
		}
	};

typedef Dimension<double> DimensionD;

template<typename T>
std::ostream& operator << (std::ostream&  os,const Dimension<T>& o)
{
os << "Dim(" << o.width << "," << o.height << ")" ;
return os;
}


template<typename T>
struct Rect
	{
	T x;
	T y;
	T width;
	T height;
	T centerX() const { return (T)(x+width/2.0); }
	T centerY() const { return (T)(y+height/2.0); }
	T maxX() const { return x+width;}
	T maxY() const { return y+height;}
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
	bool contains(T ax,T ay) const
		{
		if(ax < x ) return false;
		if(ay < y ) return false;
		if(ax > maxX() ) return false;
		if(ay > maxY() ) return false;
		return true;
		}
	bool contains(const Point<T>& pt) const
		{
		return contains(pt.x,pt.y);
		}
	};

typedef Rect<double> RectD;

template<typename T>
std::ostream& operator << (std::ostream&  os,const Rect<T>& o)
{
os << "Rect(" << o.x << "," << o.y << "," << o.width <<"," << o.height << ")" ;
return os;
} 


#endif
