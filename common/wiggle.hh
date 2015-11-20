#ifndef WIGGLE_HH
#define WIGGLE_HH
#include <vector>
#include <climits>
#include <cmath>
#include "common/random.hh"
#include "common/minmax.hh"

/* Does a sketchy appearance influence drawing behavior 
J Meyer, BB Bederson - hcil2.cs.umd.edu
*/

class Wiggle
	{
	private:
		int B;//256
		int BM;//255
		int N;//4096
		int* p;
		double* g1;
		double noiseAmp;
		bool wiggly;
	public:
		double factor;
		int steps;
		Random* rand;
		Wiggle(Random* rand):B(256),BM(255),N(0x1000),noiseAmp(.05),wiggly(true),factor(0.01),steps(10),rand(rand)
			{
			int i=0, j, k;
			p  = new int[B + B + 2];
			g1 = new double[B + B + 2];
	
			for (i = 0 ; i < B ; i++) {
				p[i] = i;
				g1[i] = (double)((rand->nextInt(SHRT_MAX) % (B + B)) - B) / B;
				}
			while (--i>=0) {
				k = p[i];
				p[i] = p[j = (rand->nextInt(SHRT_MAX) % B )];
				p[j] = k;
				}
	
			for (i = 0 ; i < B + 2 ; i++) {
				p[B + i] = p[i];
				g1[B + i] = g1[i];
				}

			}
		~Wiggle()
			{
			delete[] p;
			delete[] g1;
			}
		
	private:
		double lerp(double t, double a, double b)
		 	{
			 return(a+t*(b-a));
			 }
		double noise1(double arg)
			{
			   int bx0, bx1;
			   double rx0, rx1, sx, t, u, v;

			   t = arg + N;
			   bx0 = ((int)t)&BM;
			   bx1 = (bx0+1)&BM;
			   rx0 = t - (int) t;
			   rx1 = rx0 -1;

			   sx = rx0*rx0*(3.0 - 2.0 * rx0);
			   u = rx0 * g1[p[bx0]];
			   v = rx1 * g1[p[bx1]];

			   return (lerp(sx, u, v));
			}
	public:
		void drawWiggle(std::vector<double>& pts,float ax, float ay, float bx, float by)
			{
			double p=this->factor;
			/** distance horizontal */
			float width = bx - ax;
			//distence vertical
			float height = by - ay;
	
			float distanceCube = width * width + height * height;
			double freq = .1;

			//int lastX = 0, lastY = 0;
		    
			if (distanceCube > 0) {
			double len = std::sqrt(distanceCube) * 2;
		  	    int w = (int)(len*(this->noiseAmp));
				if (w > 50) w = 50;
			double wx = (((-height * w) / len));
			double wy = (((width * w) / len));
		
		
				float x1 = ax, y1 = ay;
				pts.push_back(x1);
				pts.push_back(y1);
				for (int i = 1; i < this->steps; i++)
					{
					double t = i/((double)this->steps);
				    double n = noise1(p);
				    p += freq;
			
				    int x2 = (int)(lerp(t, ax, bx) + n * wx);
					int y2 = (int)(lerp(t, ay, by) + n * wy);
					if (wiggly) {
						t = (i-1)/((double)this->steps);
						n = noise1(p-123.0); p += freq;
						if (n < 0) n = -n;
						t = t-n;
						if (t < -0.01) t=-0.01;
						x1 = (int)(lerp(t, ax, bx) + n * wx);
						y1 = (int)(lerp(t, ay, by) + n * wy);
					}
					pts.push_back(x2);
					pts.push_back(y2);
					x1 = x2;
					y1 = y2;		
				}
				pts.push_back(bx);
				pts.push_back(by);
			}
		}
	};


class WiggleLine
	{
	private:
		double distance(double ax, double ay, double bx, double by) const
			{
			return distance(ax-bx,ay-by);
			}
		double distance(double dx,double dy) const
			{
			return std::sqrt(std::pow(dx,2)+std::pow(dy,2));
			}
	public:
		MinMax line_length;
		MinMax radius;
		AbstractRandom* rand;
		WiggleLine(AbstractRandom* rand):line_length(80,100),radius(5),rand(rand)
			{
			
			}
		void line(std::vector<double>& pts,double x0, double y0, double x1, double y1)
			{
			pts.push_back(x0);
			pts.push_back(y0);
			for(;;)
				{
				double dx = x1-x0;
				double dy = y1-y0;
				double dx_dy = distance(dx,dy);
				if(dx_dy<=0.1) return;
				double L = line_length.rnd(rand);
				if(L<0) L=1;
				if( L >= dx_dy)
					{
					break;
					}
				else
					{
					double angle = rand->rnd(PI2);
					double r = radius.rnd(rand); 
					if(r*2.0 >= L) r=0;
					double x2 = x0 + (dx/dx_dy)*L + r*cos(angle);
					double y2 = y0 + (dy/dx_dy)*L + r*sin(angle);
					x0 = x2;
					y0 = y2;
					//std::cerr << x0 << " " << y0 << std::endl;
					pts.push_back(x0);
					pts.push_back(y0);
					}
				}
			pts.push_back(x1);
			pts.push_back(y1);
			}
	};

#endif
