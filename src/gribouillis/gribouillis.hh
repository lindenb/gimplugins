
#include "gribouillis.tab.hh"
#include "common/random.hh"
#include "common/wiggle.hh"
using namespace std;










void gribouillis::run(XDrawable xdrawable,XPreview preview)
{
 
  gint    x1, y1, x2, y2;

	
	
  if(preview)
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
  
  DEBUG(preview << x1 << "," << y1 << "," << x2<<"," << y2);
  
  long seed=std::time(NULL);
  long occurences = (long)((xdrawable.width()*xdrawable.height())*prefs()->proba);
  occurences=50;
  XTileIterator1 iter(xdrawable,preview);
  long n=0;
  while(iter.ok())
  	{
  	Random rand(seed);


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
		 //DEBUG("g"<<g << " a:" << a << " c:" << cx<<","<<cy<<"," << r);
		 
	  	ctx->gray(g,a);
		ctx->circle( cx, cy, r );
		ctx->fill();
		++n;
		}
	ctx->flush();
	
  	++iter;
    }
  if (!preview.nil())
  	{
  	DEBUG("draw region " << n);
  	preview.draw_region (iter.destination());
  	}
  else
  	  {
	  /*  update the region  */
	  xdrawable.flush();
	  xdrawable.merge_shadow(TRUE);
	  xdrawable.update(x1, y1, (x2 - x1), (y2 - y1));
	  }
	}


