#include "common/xgimp.hh"
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

/* Declare local functions.
 */
static void      query  (void);
static void      run    (gchar     *name,
			 gint       nparams,
			 GimpParam    *param,
			 gint      *nreturn_vals,
			 GimpParam   **return_vals);



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
    { GIMP_PDB_INT32, (gchar*)"run_mode", (gchar*)"Interactive, non-interactive" },
    { GIMP_PDB_IMAGE, (gchar*)"image", (gchar*)"Input image" },
    { GIMP_PDB_DRAWABLE, (gchar*)"drawable", (gchar*)"Input drawable" }
  };


/*
 the install procedure would have the procedure name
 a description of what it is, the location of the plug-in,
 the type of images the plug-in would handle
 the input and output parameters of the plug-in.
*/
  gimp_install_procedure (
		  	  "plug_in_stripes_demo_cpp",
			  "Demonstration plugin - creates stripes",
			  "This simple plug-in creates diagonal stripes.",
			  "Pierre Lindenbaum",
			  "Pierre Lindenbaum",
			  "2015",
			  "<Image>/Filters/Colors/Hatching 1",
			  "RGB*",
			  GIMP_PLUGIN,
			  G_N_ELEMENTS(args), 0,
			  args, NULL);
}


      
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
  const GimpRunModeType run_mode = (GimpRunModeType)param[0].data.d_int32;

  /*  Get the specified drawable  */
  XDrawable drawable( gimp_drawable_get (param[2].data.d_drawable));
  

  /*  Make sure that the drawable is RGB color  */
  if (drawable.is_rgb())
    {
      MyPlugin myplugin;
      gimp_progress_init ("Creating Stripes...");
      gimp_tile_cache_ntiles (2 * (drawable.width() / gimp_tile_width () + 1));
      myplugin.run(drawable);
      /*
	switch(run_mode)
		{
		case GIMP_RUN_INTERACTIVE:
			gimp_get_data("plugin-in-hellocpp",&bvals);
			if(!show_dialog(drawable)) return;
			gimp_displays_flush ();
			break;
		  case GIMP_RUN_NONINTERACTIVE:
		    if (nparams != 4)
		      status = GIMP_PDB_CALLING_ERROR;
		    if (status == GIMP_PDB_SUCCESS)
		      bvals.radius = param[3].data.d_int32;
		    break;
		  case GIMP_RUN_WITH_LAST_VALS:
		    gimp_get_data ("plug-in-hellocpp", &bvals);
		    break;
		  default:
		    break;
		}
	*/
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
 gimp_displays_flush ();
 drawable.detach();  
}


MyPlugin::MyPlugin():AbstractPlugin()
{

}
MyPlugin::~MyPlugin()
	{
	}


void MyPlugin::run(XDrawable drawable)
{
  guchar *dest, *d;
  XBound bounds = drawable.mask_bounds();
  gint    x, y;
  gpointer pr;
char *ob=gimp_context_get_brush();
gimp_context_set_brush_size(1);
char* b = ::gimp_brush_duplicate(ob);
gimp_context_set_brush(b);
gimp_context_set_brush_size(1);

  gimp_image_undo_group_start(drawable.image());
  for(int k=0;k<100;++k)
  	{
  	double x3 = random.rnd()*bounds.width() + bounds.x1;
  	double y3 = random.rnd()*bounds.height() + bounds.y1;
  	 gdouble  array2[4]={x3,y3,x3,y3};
  	 gimp_paintbrush_default( drawable.id(),2,array2);
  	}
  gimp_image_undo_group_end(drawable.image());
gimp_context_set_brush(ob);
gimp_brush_delete(b);
  drawable.flush();
}

