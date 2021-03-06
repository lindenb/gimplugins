/* copied from http://www.home.unix-ag.org/simon/gimp/guadec2002/gimp-plugin/html/demo_stripes.c */
#include <libgimp/gimp.h>
#include <libgimp/gimpcompat.h>

/* Declare local functions.
 */
static void      query  (void);
static void      run    (gchar     *name,
			 gint       nparams,
			 GimpParam    *param,
			 gint      *nreturn_vals,
			 GimpParam   **return_vals);

static void      stripes_demo         (GimpDrawable *drawable);

// Null Null corresponds to the two of the gimptool function that is option
// which are init and close which are for the beginning and ending of the gimp-plug-in
GimpPlugInInfo PLUG_IN_INFO =
{
  (GimpInitProc)NULL,  /* init_proc  */
  (GimpQuitProc)NULL,  /* quit_proc  */
  (GimpQueryProc)query, /* query_proc */
  (GimpRunProc)run   /* run_proc   */
};


MAIN ()

// query function called each time the plug-in changes
static void query (void)
{
// contains three things - the parameter type, its name, and a string describing the parameter
  static GimpParamDef args[] =
  {
    { GIMP_PDB_INT32, "run_mode", "Interactive, non-interactive" },
    { GIMP_PDB_IMAGE, "image", "Input image" },
    { GIMP_PDB_DRAWABLE, "drawable", "Input drawable" }
  };
  static gint nargs = sizeof (args) / sizeof (args[0]);

/*
 the install procedure would have the procedure name
 a description of what it is, the location of the plug-in,
 the type of images the plug-in would handle
 the input and output parameters of the plug-in.
*/
  gimp_install_procedure ("plug_in_stripes_demo",
			  "Demonstration plugin - creates stripes",
			  "This simple plug-in creates diagonal stripes.",
			  "Simon Budig  <simon@gimp.org>",
			  "Simon Budig",
			  "2001",
			  "<Image>/Filters/Colors/Stripes Demo",
			  "RGB*",
			  GIMP_PLUGIN,
			  nargs, 0,
			  args, NULL);
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
  GimpDrawable *drawable;
  GimpRunModeType run_mode;
  GimpPDBStatusType status = GIMP_PDB_SUCCESS;

  gint32 image_ID;

  run_mode = param[0].data.d_int32;

  /*  Get the specified drawable  */
  drawable = gimp_drawable_get (param[2].data.d_drawable);
  image_ID = param[1].data.d_image;

  /*  Make sure that the drawable is RGB color  */
  if (gimp_drawable_is_rgb (drawable->drawable_id))
    {
      gimp_progress_init ("Creating Stripes...");
      gimp_tile_cache_ntiles (2 * (drawable->width / gimp_tile_width () + 1));
      stripes_demo (drawable);

      if (run_mode != GIMP_RUN_NONINTERACTIVE)
	gimp_displays_flush ();
    }
  else
    {
      status = GIMP_PDB_EXECUTION_ERROR;
    }

  *nreturn_vals = 1;
  *return_vals = values;

  values[0].type = GIMP_PDB_STATUS;
  values[0].data.d_status = status;

  gimp_drawable_detach (drawable);
}


static void
stripes_demo (GimpDrawable *drawable)
{
  GimpPixelRgn src_rgn, dest_rgn;
  guchar *src, *s;
  guchar *dest, *d;
  gint    progress, max_progress;
  gint    has_alpha, red, green, blue, alpha;
  gint    x1, y1, x2, y2;
  gint    x, y;
  gpointer pr;

  /* Get selection area */
  gimp_drawable_mask_bounds (drawable->drawable_id, &x1, &y1, &x2, &y2);
  has_alpha = gimp_drawable_has_alpha (drawable->drawable_id);

  red = 0; green = 1; blue = 2;

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
	      d[0] = (src_rgn.x + x + src_rgn.y + y) % 256;
	      d[1] = s[1];
	      d[2] = (- src_rgn.x - x + src_rgn.y + y) % 256;
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

  /*  update the region  */
  gimp_drawable_flush (drawable);
  gimp_drawable_merge_shadow (drawable->drawable_id, TRUE);
  gimp_drawable_update (drawable->drawable_id, x1, y1, (x2 - x1), (y2 - y1));
}

