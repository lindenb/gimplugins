/* copied from http://www.home.unix-ag.org/simon/gimp/guadec2002/gimp-plugin/html/demo_stripes.c */
#include <libgimp/gimp.h>
#include <libgimp/gimpui.h>
#include <libgimpwidgets/gimpwidgets.h>
#include "common/random.hh"
#include "common/abstractplugin.hh"
#include "common/wiggle.hh"
using namespace std;


class MyPlugin : public AbstractPlugin
	{
	private:
		Random random;
	public:
		MyPlugin();
		virtual ~MyPlugin();
		virtual void run(XDrawable drawable);
	};

typedef struct
      {
        gint radius;
      } MyBlurVals;


      /* Set up default values for options */
      static MyBlurVals bvals =
      {
        3  /* radius */
      };

static void      query  (void);
static void      run    (gchar *name, gint nparams, GimpParam *param, gint *nreturn_vals, GimpParam **return_vals);


GimpPlugInInfo PLUG_IN_INFO = {(GimpInitProc)NULL,(GimpQuitProc)NULL,(GimpQueryProc)query,(GimpRunProc)run};


MAIN ()

DEFAULT_GIMP_PLUGIN_QUERY(query,"Gribouillis","Gribouillis")

      
static gboolean
      show_dialog (GimpDrawable *drawable)
      {
        GtkWidget *dialog;
        GtkWidget *main_vbox;
        GtkWidget *main_hbox;
        GtkWidget *frame;
        GtkWidget *radius_label;
        GtkWidget *alignment;
        GtkWidget *spinbutton;
        GtkObject *spinbutton_adj;
        GtkWidget *frame_label;
        gboolean   run;

        gimp_ui_init ("myblur", FALSE);

        dialog = gimp_dialog_new ("My blur", //titlle
        			"myblur",//role
                                  NULL, //parent
                                  (GtkDialogFlags)0,//flag
                                  gimp_standard_help_func,
                                  "plug-in-myblur",
                                  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                  GTK_STOCK_OK,     GTK_RESPONSE_OK,
                                  NULL);

        main_vbox = gtk_vbox_new (FALSE, 6);
        gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), main_vbox);
        gtk_widget_show (main_vbox);

        frame = gtk_frame_new (NULL);
        gtk_widget_show (frame);
        gtk_box_pack_start (GTK_BOX (main_vbox), frame, TRUE, TRUE, 0);
        gtk_container_set_border_width (GTK_CONTAINER (frame), 6);

        alignment = gtk_alignment_new (0.5, 0.5, 1, 1);
        gtk_widget_show (alignment);
        gtk_container_add (GTK_CONTAINER (frame), alignment);
        gtk_alignment_set_padding (GTK_ALIGNMENT (alignment), 6, 6, 6, 6);

        main_hbox = gtk_hbox_new (FALSE, 0);
        gtk_widget_show (main_hbox);
        gtk_container_add (GTK_CONTAINER (alignment), main_hbox);

        radius_label = gtk_label_new_with_mnemonic ("_Radius:");
        gtk_widget_show (radius_label);
        gtk_box_pack_start (GTK_BOX (main_hbox), radius_label, FALSE, FALSE, 6);
        gtk_label_set_justify (GTK_LABEL (radius_label), GTK_JUSTIFY_RIGHT);

        spinbutton_adj = gtk_adjustment_new (3, 1, 16, 1, 5, 5);
        spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adj), 1, 0);
        gtk_widget_show (spinbutton);
        gtk_box_pack_start (GTK_BOX (main_hbox), spinbutton, FALSE, FALSE, 6);
        gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton), TRUE);

        frame_label = gtk_label_new ("Modify radius");
        gtk_widget_show (frame_label);
        gtk_frame_set_label_widget (GTK_FRAME (frame), frame_label);
        gtk_label_set_use_markup (GTK_LABEL (frame_label), TRUE);

        g_signal_connect (spinbutton_adj, "value_changed",
                          G_CALLBACK (gimp_int_adjustment_update),
                          &bvals.radius);
        gtk_widget_show (dialog);

        run = (gimp_dialog_run (GIMP_DIALOG (dialog)) == GTK_RESPONSE_OK);

        gtk_widget_destroy (dialog);

        return run;
      }

// run plug-in is the heart of the plug-in program
// the parameters are name, inputparameters and a pointer to output parameters.
static void run (
     gchar   *name,
     gint     nparams,
     GimpParam  *param,
     gint    *nreturn_vals,
     GimpParam **return_vals)
{
  static GimpParam values[1];
  GimpPDBStatusType status = GIMP_PDB_SUCCESS;
  //gint32 image_ID = param[1].data.d_image;
  //const GimpRunModeType run_mode = (GimpRunModeType)param[0].data.d_int32;

  /*  Get the specified drawable  */
  XDrawable drawable(gimp_drawable_get (param[2].data.d_drawable));
  

  /*  Make sure that the drawable is RGB color  */
  if (drawable.is_rgb ())
    {
    MyPlugin myplugin;
    ::gimp_progress_init ("Creating Stripes...");
    ::gimp_tile_cache_ntiles (2 * (drawable.width() / gimp_tile_width () + 1));
	myplugin.run(drawable);
	::gimp_displays_flush ();
    }
  else
    {
      status = GIMP_PDB_EXECUTION_ERROR;
    }

  *nreturn_vals = 1;
  *return_vals = values;

  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;
 gimp_displays_flush ();
 drawable.gimp_detach();
  
}


MyPlugin::MyPlugin():AbstractPlugin()
{

}
MyPlugin::~MyPlugin()
	{
	}


void MyPlugin::run(XDrawable xdrawable)
{
  
  GimpPixelRgn src_rgn, dest_rgn;
  guchar *src, *s;
  guchar *dest, *d;
  gint    progress, max_progress;
  gint    has_alpha, alpha;
  gint    x1, y1, x2, y2;
  gint    x, y;
  gpointer pr;
  //gint  red, green, blue
  GimpDrawable *drawable = xdrawable.drawable();

  /* Get selection area */
  gimp_drawable_mask_bounds (drawable->drawable_id, &x1, &y1, &x2, &y2);
  has_alpha = gimp_drawable_has_alpha (drawable->drawable_id);

  //red = 0; green = 1; blue = 2;

  alpha = (has_alpha) ? drawable->bpp - 1 : drawable->bpp;

  /* Initialize progress */
  progress = 0;
  max_progress = (x2 - x1) * (y2 - y1);

  /* substitute pixel vales */
  gimp_pixel_rgn_init (&src_rgn, drawable,
		       x1, y1, (x2 - x1), (y2 - y1), FALSE, FALSE);
  gimp_pixel_rgn_init (&dest_rgn, drawable,
		       x1, y1, (x2 - x1), (y2 - y1), TRUE, TRUE);

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

      gimp_progress_update ((double) progress / (double) max_progress);
    }

  /*  update the region  */
  gimp_drawable_flush (drawable);
  gimp_drawable_merge_shadow (drawable->drawable_id, TRUE);
  gimp_drawable_update (drawable->drawable_id, x1, y1, (x2 - x1), (y2 - y1));




#ifdef XXXX
  for(XTileIterator2 iter(drawable);iter.ok();++iter)
  	{
  	cerr << "OK" << iter.x() << endl;
  	XCairo* ctx =iter.cairo();
  	//cerr << ctx->width() << ctx << ctx->height() << endl;
  	ctx->rgb(100,100,0);
	ctx->move_to(0,0);
	ctx->line_to(100,100);
	ctx->stroke();
	delete ctx;
  	}
  drawable.flush();
  drawable.update();
#endif

#ifdef XXXX
  GimpPixelRgn src_rgn, dest_rgn;
  guchar *src, *s;
  guchar *dest, *d;
  gint    progress, max_progress;
  gint    has_alpha, alpha;
  gint    x1, y1, x2, y2;
  gint    x, y;
  gpointer pr;
  GimpDrawable* drawable = xdrawable.drawable();
  /* Get selection area */
  gimp_drawable_mask_bounds (drawable->drawable_id, &x1, &y1, &x2, &y2);
  has_alpha = gimp_drawable_has_alpha (drawable->drawable_id);
 gdouble  array[8]={(gdouble)0,(gdouble)0,(gdouble)x2,(gdouble)y2,(gdouble)0,(gdouble)y2,(gdouble)0,(gdouble)0};
//gimp_paintbrush_default( drawable->drawable_id,4,array);
  alpha = (has_alpha) ? drawable->bpp - 1 : drawable->bpp;

  /* Initialize progress */
  progress = 0;
  max_progress = (x2 - x1) * (y2 - y1);

  /* substitute pixel vales */
  gimp_pixel_rgn_init (&src_rgn, drawable,
		       x1, y1, (x2 - x1), (y2 - y1), FALSE, FALSE);
  gimp_pixel_rgn_init (&dest_rgn, drawable,
		       x1, y1, (x2 - x1), (y2 - y1), TRUE, TRUE);




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
	     if( this->random.rnd()<0.1)
	     {
	      int gray = this->random.nextInt(256);
	      d[0] = gray;
	      d[1] = gray;
	      d[2] = gray;
	      
	      }
	      else
	      {
	      d[0]=s[0];
	      d[1]=s[1];
	      d[2]=s[2];
	      }
	      if (has_alpha)
		d[alpha] = s[alpha];
	      s += src_rgn.bpp;
	      d += dest_rgn.bpp;
	    }

	  src += src_rgn.rowstride;
	  dest += dest_rgn.rowstride;
	}

      /* Update progress */
      progress += src_rgn.w * src_rgn.h;

      gimp_progress_update ((double) progress / (double) max_progress);
    }



//gimp_paintbrush_default( drawable->drawable_id,4,array);



  /*  update the region  */
  gimp_drawable_flush (drawable);
  //gimp_paintbrush_default( drawable->drawable_id,4,array);
  gimp_drawable_merge_shadow (drawable->drawable_id, TRUE);
  //TODO  gimp_context_get_brush ();
  gimp_drawable_update (drawable->drawable_id, x1, y1, (x2 - x1), (y2 - y1));
  gimp_paintbrush_default( drawable->drawable_id,4,array);
  for(int k=0;k<100;++k)
  	{
  	double x3 = random.rnd()*x2;
  	double y3 = random.rnd()*y2;
  	 gdouble  array2[4]={x3,y3,x3,y3};
  	 gimp_paintbrush_default( drawable->drawable_id,2,array2);
  	}
#endif
}

