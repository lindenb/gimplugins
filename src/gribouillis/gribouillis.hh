#include <vector>
#include "gribouillis.tab.hh"
#include "common/random.hh"
#include "common/range.hh"
#include "common/wiggle.hh"
#include "common/trigonometry.hh"

using namespace std;

class gribouillis:public abstract_gribouillis
	{ 
	public:
		Random rand;
		gribouillis() {}
		virtual ~gribouillis() {}
		virtual void  run(XDrawable xdrawable,XPreview preview);
	};


void gribouillis::run(XDrawable xdrawable,XPreview preview)
{
 
  gint    x1, y1, x2, y2;
	
 if (!preview.nil())
  	{
  	x1 = preview.x();
  	y1 = preview.y();
  	x2 = x1 + preview.width();
    y2 = y1 + preview.height();
  	}
  else
  	{
    /* Get selection area */
    xdrawable.mask_bounds( &x1, &y1, &x2, &y2);
   
	}
  
  long seed=std::time(NULL);
  long occurences = (long)(((x2-x1)*(y2-y1))*prefs()->proba);
  
  XTileIterator1 iter(xdrawable,preview);
  long n=0;
  while(iter.ok())
  	{
  	rand.reset(seed);


  	XCairo* ctx = iter.cairo();
	
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

  		 double cx= rand.rnd(x2-x1);
  		 double cy= rand.rnd(y2-y1);
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
		
		if(!preview.nil())
			{
			preview.draw_region (iter.destination());
			}
		++n;
		}

	
	
  	++iter;
    }
  if (preview.nil())
  	  {
	  /*  update the region  */
	  xdrawable.flush();
	  xdrawable.merge_shadow(TRUE);
	  xdrawable.update(x1, y1, (x2 - x1), (y2 - y1));
	  }
	}


