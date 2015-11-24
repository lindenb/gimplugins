#ifndef MINMAX_HH
#define MINMAX_HH
#include "common/random.hh"

struct MinMax
	{
	double m;
	double M;
	MinMax():m(0.0),M(1.0)
		{
		}
	MinMax(double M):m(0.0),M(M)
		{
		}
	MinMax(double m,double M):m(m),M(M)
		{
		}
	void reset(double min,double max)
		{
		this->m = min;
		this->M = max;
		}
	void reset(double max)
		{
		this->m = 0;
		this->M = max;
		}
	double min() const { return m<M?m:M;}
	double max() const { return m<M?M:m;}
	double distance() const
		{
		return max() - min();
		}
	double lerp(double f) const
		{
		return min() + distance()* f;
		}	
	double range(double v) const
		{
		if(v<min()) return min();
		if(v>max()) return max();
		return v;
		}
	double rnd(AbstractRandom* rand) const
		 {
		 return lerp(rand->rnd());
		 }
	double operator()(double v)
		{
		return lerp(v);
		}
	};

#endif


