#ifndef WIGGLE_HH
#define WIGGLE_HH
#include <vector>
#include <climits>
#include <cmath>
#include "common/random.hh"
/* Does a sketchy appearance influence drawing behavior 
J Meyer, BB Bederson - hcil2.cs.umd.edu
*/

class Wiggle
	{
	private:
		Random rand;
		int B;//256
		int BM;//255
		int N;//4096
		int* p;
		double* g1;
		double noiseAmp;
		bool wiggly;
	public:
		
		Wiggle():B(256),BM(255),N(0x1000),noiseAmp(.05),wiggly(true)
			{
			int i=0, j, k;
			p  = new int[B + B + 2];
			g1 = new double[B + B + 2];
	
			for (i = 0 ; i < B ; i++) {
				p[i] = i;
				g1[i] = (double)((rand.nextInt(SHRT_MAX) % (B + B)) - B) / B;
				}
			while (--i>=0) {
				k = p[i];
				p[i] = p[j = (rand.nextInt(SHRT_MAX) % B )];
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
		void drawWiggle(std::vector<double>& path,double p, float ax, float ay, float bx, float by)
			{
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
				path.push_back(x1);
				path.push_back(y1);
				for (int i = 1; i < 10; i++)
					{
					double t = i/10.0;
				    double n = noise1(p);
				    p += freq;
			
				    int x2 = (int)(lerp(t, ax, bx) + n * wx);
					int y2 = (int)(lerp(t, ay, by) + n * wy);
					if (wiggly) {
						t = (i-1)/10.0;
						n = noise1(p-123.0); p += freq;
						if (n < 0) n = -n;
						t = t-n;
						if (t < -0.01) t=-0.01;
						x1 = (int)(lerp(t, ax, bx) + n * wx);
						y1 = (int)(lerp(t, ay, by) + n * wy);
					}
					path.push_back(x2);
					path.push_back(y2);
					x1 = x2;
					y1 = y2;		
				}
				
				path.push_back(bx);
				path.push_back(by);
			}
			}
		 
	};


#endif
