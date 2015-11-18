#ifndef WAVE_HH
#define WAVE_HH

#include <cmath>
#include <vector>
#include "common/random.hh"

class Wave : public AbstractRandom 
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
		Wave(double r,double radspeed):AbstractRandom(),maxr(r)
			{
			Circle c;
			c.r=(r<0?-r:r);
			c.angle=0;
			c.radspeed=radspeed;
			circles.push_back(c);
			}
		Wave& add(double r,double radspeed)
			{
			maxr+=r;
			Circle c;
			c.r=(r<0?-r:r);
			c.angle=0;
			c.radspeed=radspeed;
			circles.push_back(c);
			return *this;
			}
		
		/** https://twitter.com/klmr/status/667005762565181440 */
		using AbstractRandom::rnd ;
		virtual double rnd()
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
