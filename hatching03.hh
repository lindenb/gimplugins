#include <vector>
#include <map>
#include "hatching03.tab.hh"
#include "common/wiggle.hh"
#include "common/geometry.hh"
#include "common/trigonometry.hh"

using namespace std;


#define CLASSNAME Hatching03

class CLASSNAME:public AbstractHatching03
	{ 
	public:
		
		CLASSNAME() {}
		virtual ~CLASSNAME() {}
		virtual void  paint(XCairo* ctx,gint image_width,gint image_height);
	};


void CLASSNAME::paint(XCairo* ctx,gint image_width,gint image_height)
	{
	PointD center(
		prefs()->centerx * image_width,
		prefs()->centery * image_height
		);
		cerr << center.x << " " << center.y << endl;
		cerr << prefs()->centerx << " " << prefs()->centery << endl;
	Wiggle wiggle(&rand);
	map<Radian,double>* rad2radius=0;
	map<Radian,double>* next_rad2radius=0;
	int max_iterations=prefs()->iterations,iteration=0;
	MinMax stroke_size(
		prefs()->minstrokesize,
		prefs()->maxstrokesize
		);
	MinMax gray(
		prefs()->mingray,
		prefs()->maxgray
		);
	MinMax distance_between_strokes(
		prefs()->mindiststroke,
		prefs()->maxdiststroke
		);
	MinMax distance_between_circles(
		prefs()->mindistcircle,
		prefs()->maxdistcircle
		);
	MinMax stroke_width(
		prefs()->minstrokewidth,
		prefs()->maxstrokewidth
		);
	int user_precision=1;
	ctx->line_width(1);
	ctx->rgb(0, 0, 0);
	double radius=0;
	double prev_radius=0;
	
	for(;;)
		{
		++iteration;
		next_rad2radius = new map<Radian,double>;
		
		radius += stroke_size.rnd(&rand);
		//double perimeter = PI2*radius;
		double rad_between_strokes =   distance_between_strokes(iteration/(double)max_iterations)/radius;
		
		Radian angle(PI2 * rand.rnd());
		int n_strokes = (int)(PI2/rad_between_strokes); 
		int sens=1;
		//cerr << "iter:"<< iteration << " "<<n_strokes << " r=" << radius << " p=" << perimeter << " r2=" << rad_between_strokes << endl;
		for(int n=0; n< n_strokes; ++n)
			{
			double a_radius = prev_radius+rand.nextInt(-user_precision,user_precision);
			if( rad2radius!=0)
				{
				a_radius=0;
				double m=0;
				map<Radian,double>::iterator r= rad2radius->lower_bound(angle);
				while(r!=rad2radius->end())
					{
					if( r->first() > angle()+rad_between_strokes) break;
					a_radius+=r->second;
				++m;
					++r;
					}
				a_radius/=m;
				}
			
			
			double ax=
				center.x + angle.cos()*(a_radius)+rand.nextInt(-user_precision,user_precision);
			
			double ay=
				center.y + angle.sin()*(a_radius)+rand.nextInt(-user_precision,user_precision);
			
			Radian b_angle ( modulo_pi2(angle() + rand.rnd()*0.001 * sens) ) ;
			
			
			
			double b_radius = radius+rand.nextInt(-1,fabs(30.0*b_angle.sin()));
			
			double bx=
				center.x + b_angle.cos()*(b_radius)+rand.nextInt(-user_precision,user_precision);
			
			double by=
				center.y + b_angle.sin()*(b_radius)+rand.nextInt(-user_precision,user_precision);
			
			double b_distance = sqrt(pow(bx-center.x,2)+pow(by-center.y,2));
			//cerr << b_angle << " " << b_distance << endl;
			next_rad2radius->insert(make_pair<Radian,double>(b_angle,b_distance + 
					distance_between_circles(iteration/(double)max_iterations)));
			
			
			if(iteration>1)
				{
				vector<double> pts;
				wiggle.drawWiggle(pts,ax,  ay,  bx,  by);
				MinMax r(1.0,0.5);
				MinMax lw(1.0,0.8);
				for(size_t i=0;i+1< pts.size();i+=2)
					{
					double gray_v = gray(iteration/(double)max_iterations);
					ctx->rgba(gray_v,gray_v,gray_v,r(i/(double)(pts.size()/2)));
					if(i==0)
						{
						ctx->move_to(pts[0],pts[1]);
						}
					else
						{
						ctx->line_to(pts[i],pts[i+1]);
						ctx->line_width(stroke_width(iteration/(double)max_iterations)*lw(i/(double)(pts.size()/2)));
						ctx->stroke();
						ctx->move_to(pts[i],pts[i+1]);
						}
					}
				}
			angle += rad_between_strokes;
			angle = modulo_pi2(angle());
			}
		if(rad2radius!=0) delete rad2radius;
		rad2radius = next_rad2radius;
		
		
		if(radius>image_width) break;
		
		radius+=max(2.0,distance_between_circles(iteration/(double)max_iterations));
		prev_radius=radius;
		}
	
	if(rad2radius!=0) delete rad2radius;
	}


#ifdef STANDALONE
int main(int argc,char** argv)
	{
	CLASSNAME app;
	return app.main(argc,argv);
	}
#endif

