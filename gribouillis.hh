#include <vector>
#include "gribouillis.tab.hh"
#include "common/range.hh"
#include "common/wiggle.hh"
#include "common/trigonometry.hh"

using namespace std;

class gribouillis:public abstract_gribouillis
	{ 
	public:
		
		gribouillis() {}
		virtual ~gribouillis() {}
		virtual void  paint(XCairo* ctx,gint image_width,gint image_height);
	};


void gribouillis::paint(XCairo* ctx,gint image_width,gint image_height)
{

  long occurences = (long)(((image_width)*(image_height))*prefs()->proba);
 
	
  	for(long i=0;i< occurences;++i)
  		{
  		double g = rand.rnd(
  			std::max(0.0,std::min(prefs()->mingray,prefs()->maxgray)),
  			std::min(1.0,std::max(prefs()->mingray,prefs()->maxgray))
  			)
  			;
  		
  		double a = rand.rnd(
  			std::max(0.0,std::min(prefs()->minalpha,prefs()->maxalpha)),
  			std::min(1.0,std::max(prefs()->minalpha,prefs()->maxalpha))
  			)
  			;

  		 double cx= rand.rnd( image_width);
  		 double cy= rand.rnd( image_height);
  		 double r = rand.rnd(
		 		std::min(prefs()->minradius,prefs()->maxradius),
  				std::max(prefs()->minradius,prefs()->maxradius)
		 		);
		
		 
	  	ctx->gray(g,a);
		if( prefs()->wiggle && r>0)
			{
			double angle = 0.0;
			double shift_angle = rand.rnd(PI2);
			bool first=true;
			while(angle< PI2)
				{
				double radius  = abs(r + rand.sign() * rand.rnd(prefs()->precision));
				if(radius<=0) radius=0.0001;
				double px = cx + cos(angle + shift_angle)*radius;
				double py = cy + sin(angle + shift_angle)*radius;
				if(first)
					{
					ctx->move_to(px,py);
					}
				else
					{
					ctx->line_to(px,py);
					}
				double stroke_len= rand.rnd( prefs()->strokelen + rand.rnd(prefs()->precision));
				if(stroke_len<=0) stroke_len=1;
				
				/* arc = angle * radius */
				angle += (stroke_len / radius);
				
				first=false;
				}
			ctx->fill();
			}
		else
			{
			ctx->circle( cx, cy, r );
			ctx->fill();
			}
		
		}
	}
#ifdef STANDALONE
int main(int argc,char** argv)
	{
	gribouillis app;
	return app.main(argc,argv);
	}
#endif

