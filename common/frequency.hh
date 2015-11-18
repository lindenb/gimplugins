#ifndef FREQUENCY_HH
#define FREQUENCY_HH

#include <cmath>
#include <vector>

class Frequency
	{
	private:
		struct Circle
			{
			double r;
			double angle;
			double radspeed;
			};
		std::vector<Circle> circles;
		double  maxr;
	public:
		Frequency(double r,double radspeed):maxr(r)
			{
			Circle c;
			c.r=r;
			c.angle=0;
			c.radspeed=radspeed;
			circles.push_back(c);
			}
		Frequency& add(double r,double radspeed)
			{
			maxr+=r;
			Circle c;
			c.r=r;
			c.angle=0;
			c.radspeed=radspeed;
			circles.push_back(c);
			return *this;
			}
		double rnd()
			{
			double cx=0;
			double cy=0;
			for(std::vector<Circle>::size_type i=0;
				i< circles.size();
				++i)
				{
				Circle& c = circles.at(i);
				c.angle += c.radspeed;
				cx = cx + cos(c.angle)*c.r;
				cy = cy + sin(c.angle)*c.r;
				}
			return (maxr-cx)/(2.0*maxr);
			}
	};

#endif
