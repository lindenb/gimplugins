
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
  Random rand;
  long seed=0L;
  XTileIterator1 iter(xdrawable,preview);
  while(iter.ok())
  	{
  	rand.reset(seed);
  	iter.copy();
  
  	XCairo* ctx = iter.cairo();
  	for(int i=0;i< 1000;++i)
  		{
  		double g = 0.5+rand.rnd(0.5);
	  	 ctx->gray(g);
		 ctx->circle(
		 	rand.rnd(x2-x1),
		 	rand.rnd(y2-y1),
		 	2+rand.rnd(50)
		 	);
		ctx->fill();
		}
  	++iter;
    }
  if (preview)
  	{
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


