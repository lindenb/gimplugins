/* copied from http://www.home.unix-ag.org/simon/gimp/guadec2002/gimp-plugin/html/demo_stripes.c */
#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include <libgimpwidgets/gimpwidgets.h>
#include "common/random.hh"
#include "common/abstractplugin.hh"
#include "common/wiggle.hh"
using namespace std;







#include "gribouillis.h"



void gribouillis::run(XDrawable xdrawable,XPreview preview)
{
  
  GimpPixelRgn src_rgn, dest_rgn;
  guchar *src, *s;
  guchar *dest, *d;
  gint    progress, max_progress;
  gint    has_alpha, alpha;
  gint    x1, y1, x2, y2;
  guint    x, y;
  gpointer pr;
  //gint  red, green, blue
  GimpDrawable *drawable = xdrawable.drawable();
	
	
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
    has_alpha = xdrawable.has_alpha();
	}
  //red = 0; green = 1; blue = 2;

  alpha = (has_alpha) ? drawable->bpp - 1 : drawable->bpp;

  /* Initialize progress */
  progress = 0;
  max_progress = (x2 - x1) * (y2 - y1);

  /* substitute pixel vales */
  gimp_pixel_rgn_init (&src_rgn, drawable,
		       x1, y1, (x2 - x1), (y2 - y1), FALSE, FALSE);
  gimp_pixel_rgn_init (&dest_rgn, drawable,
		       x1, y1, (x2 - x1), (y2 - y1), preview.nil(), TRUE);

  for (pr = gimp_pixel_rgns_register (2, &src_rgn, &dest_rgn);
       pr != NULL;
       pr = gimp_pixel_rgns_process (pr))
    {
      src = src_rgn.data;
      dest = dest_rgn.data;


      for (y = 0; y < src_rgn.h; y++)
	{
	  s = src;
	  d = dest;

	  for (x = 0; x < src_rgn.w; x++)
	    {
	      d[0] = s[0];
	      d[1] = s[1];
	      d[2] = s[2];
	      if (has_alpha)
		d[alpha] = s[alpha];

	      s += src_rgn.bpp;
	      d += dest_rgn.bpp;
	    }

	  src += src_rgn.rowstride;
	  dest += dest_rgn.rowstride;
		}
		      
	  cairo_surface_t* s= ::cairo_image_surface_create_for_data (
				dest_rgn.data,
                 CAIRO_FORMAT_ARGB32,
                 dest_rgn.w,
                 dest_rgn.h,
                 dest_rgn.rowstride
                 );
	cairo_status_t status= cairo_surface_status (s);
	if(status!=CAIRO_STATUS_SUCCESS)
		{
		cerr <<  cairo_status_to_string(status) << endl;
		}

	 XCairo* ctx=new XCairo(s);
   	 ctx->rgb(100,100,0);
     ctx->move_to(0,0);
	ctx->line_to(100,100);
	ctx->stroke();
   	 ctx->rgb(0,200,0);
	ctx->rectangle(1,1,100,100);
	ctx->fill();
	ctx->flush();
	 delete ctx;
cerr << "ok " << dest_rgn.w << "/" << dest_rgn.h << endl;

      /* Update progress */
      progress += src_rgn.w * src_rgn.h;
	 if (! preview )
	 	{
        ::gimp_progress_update ((double) progress / (double) max_progress);
        }
    }
  if (preview)
  	{
  	preview.draw_region (&dest_rgn);
  	}
  else
  	  {
	  /*  update the region  */
	  xdrawable.flush();
	  xdrawable.merge_shadow(TRUE);
	  xdrawable.update(x1, y1, (x2 - x1), (y2 - y1));
	  }

}

