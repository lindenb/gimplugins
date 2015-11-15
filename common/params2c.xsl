<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:import href="mod.plugin.xsl"/>
<xsl:output method="text"/>

<xsl:variable name="instanceclass">
	<xsl:choose>
		<xsl:when test="/plugin/@instanceclass"><xsl:value-of select="/plugin/@instanceclass"/></xsl:when>
		<xsl:otherwise><xsl:value-of select="$plugintitle"/></xsl:otherwise>
	</xsl:choose>
</xsl:variable>

<xsl:template match="/">
#ifdef STANDALONE
#include &lt;unistd.h&gt;
#include &lt;getopt.h&gt;
#endif
#include "<xsl:value-of select="$pluginname"/>.hh"

<xsl:apply-templates select="plugin"/>

</xsl:template>

<xsl:template match="plugin">


<xsl:for-each select="//param[@type='select']">

/** <xsl:apply-templates select="." mode="description"/> */
<xsl:apply-templates select="." mode="enum.name"/><xsl:text> </xsl:text><xsl:apply-templates select="." mode="enum.name"/>IndexFromString(const char* s) {
	<xsl:for-each select="option">
	<xsl:if test="position()&gt;1">else </xsl:if> if(s!=NULL &amp;&amp; strcmp(s,"<xsl:value-of select="@value"/>")==0)
		{
		return <xsl:apply-templates select="." mode="enum.item"/> ;
		}
	</xsl:for-each>
	else
		{
		#ifdef STANDALONE
		cerr &lt;&lt; "Bad value for enum <xsl:value-of select="@name"/>" &lt;&lt; (s==NULL?"(NULL)":s) &lt;&lt; "available are: " &lt;&lt; endl;
		<xsl:for-each select="option">
		cerr &lt;&lt; "\t+ <xsl:value-of select="@value"/>" &lt;&lt; endl;
		</xsl:for-each>
		
		exit(EXIT_FAILURE);
		#else
		return <xsl:apply-templates select="option[@default='true']" mode="enum.item"/>;
		#endif
		}
	};

#ifndef STANDALONE
static void cb_<xsl:value-of select="generate-id(.)"/>changed( GtkComboBox *combo, gpointer data)
{
	
	<xsl:value-of select="$abstractpluginname"/>* me = (<xsl:value-of select="$abstractpluginname"/>*)data;
    /* Obtain currently selected string from combo box */
    gchar *s = gtk_combo_box_get_active_text( combo );
 

 	me->prefs()-><xsl:value-of select="@name"/> = <xsl:apply-templates select="." mode="enum.name"/>IndexFromString(s);

    /* Free string */
    g_free( s );
}
#endif

</xsl:for-each>


#ifndef STANDALONE

static void <xsl:value-of select="$pluginname"/>_query (void) {
  static GimpParamDef args[] = {
    { GIMP_PDB_INT32, (gchar*)"run_mode", (gchar*)"Interactive, non-interactive" },
    { GIMP_PDB_IMAGE, (gchar*)"image", (gchar*)"Input image" },
    { GIMP_PDB_DRAWABLE, (gchar*)"drawable", (gchar*)"Input drawable" }
  };
  ::gimp_install_procedure (
		  "<xsl:apply-templates select="." mode="label"/>" ,
		  "<xsl:apply-templates select="." mode="description"/>",
		  "<xsl:apply-templates select="." mode="description"/>",
		  "Pierre Lindenbaum  &lt;plindenbaum@yahoo.fr&gt;",
		  "Pierre Lindenbaum",
		  __DATE__,
		  "&lt;Image&gt;/Filters/Kakaheska/<xsl:apply-templates select="." mode="label"/>" ,
		  "RGB*",
		  GIMP_PLUGIN,
		  G_N_ELEMENTS(args), 0,
		  args, NULL);
	}



// run plug-in is the heart of the plug-in program
// the parameters are name, inputparameters and a pointer to output parameters.
static void <xsl:value-of select="$pluginname"/>_run (
     gchar   *name,
     gint     nparams,
     GimpParam  *param,
     gint    *nreturn_vals,
     GimpParam **return_vals)
	{
	<xsl:value-of select="$instanceclass"/> instance;
	instance.run1(name,nparams,param,nreturn_vals,return_vals);
	}

#endif

/**  constructor for <xsl:value-of select="$pluginparam"/> */
<xsl:value-of select="$pluginparam"/>::<xsl:value-of select="$pluginparam"/>()
	{
	<xsl:apply-templates select="//param" mode="instance"/>
	#ifdef STANDALONE
	image_width = 1000; /* width */
	image_height = 1000;  /* height */
	#else
	preview= 1;/* preview */
	#endif
	}

<xsl:value-of select="$pluginparam"/>
<xsl:text> </xsl:text>
<xsl:value-of select="$abstractpluginname"/>::PREFS;

/** constructor */
<xsl:value-of select="$abstractpluginname"/>::<xsl:value-of select="$abstractpluginname"/>()
	{
	}

/** destructor */
<xsl:value-of select="$abstractpluginname"/>::~<xsl:value-of select="$abstractpluginname"/>()
	{
	}

#ifdef STANDALONE

void <xsl:value-of select="$abstractpluginname"/>::usage(std::ostream&amp; out)
	{
	out &lt;&lt; "Compilation: " &lt;&lt; __DATE__ &lt;&lt; __TIME__ &lt;&lt; std::endl;
	out &lt;&lt; "Options: " &lt;&lt; std::endl;
	out &lt;&lt; "  -h help, this screen" &lt;&lt; std::endl;
	out &lt;&lt; "  -v version" &lt;&lt; std::endl;
	out &lt;&lt; "  -W|--width (int) image width" &lt;&lt; std::endl;
	out &lt;&lt; "  -H|--height (int) image height" &lt;&lt; std::endl;	
	out &lt;&lt; "  -o|--output (filename) output file. REQUIRED" &lt;&lt; std::endl;	
	<xsl:for-each select="//param">
    out &lt;&lt; "  --<xsl:value-of select="@name"/>" &lt;&lt; std::endl;	
    </xsl:for-each>
	out &lt;&lt; std::endl;
	}


int <xsl:value-of select="$abstractpluginname"/>::main(int argc,char** argv)
	{
	char* save_as_filename = NULL;
           int c;

           while (1) {
               //int this_option_optind = optind ? optind : 1;
               int option_index = 0;
               static struct option long_options[] = {
               	   {"help",     no_argument, 0,  'h' },
               	   {"version",     no_argument, 0,  'v' },
                   {"width",     required_argument, 0,  'W' },
                   {"height",     required_argument, 0,  'H' },
                   {"output",     required_argument, 0,  'o' }<xsl:for-each select="//param">,
                   {"<xsl:value-of select="@name"/>",<xsl:choose>
                   		<xsl:when test="@type='bool'">no_argument</xsl:when>
                   		<xsl:otherwise>required_argument</xsl:otherwise>
                   	</xsl:choose>,0,0}</xsl:for-each>,
                   {0,         0,                 0,  0 }
               		};
               c = getopt_long(argc, argv, "W:H:o:vh", long_options, &amp;option_index);
               if (c == -1) break;
               switch (c) {
               case 0:
               		<xsl:for-each select="//param">
               		if( strcmp("<xsl:value-of select="@name"/>" , long_options[option_index].name ) == 0)
               			{
               			<xsl:choose>
               			<xsl:when test="@type='select'">
               			<xsl:for-each select="option">
               			<xsl:if test="position()&gt;1">else </xsl:if> if(strcmp(optarg,"<xsl:value-of select="@value"/>")==0)
               				{
               				prefs()-><xsl:value-of select="../@name"/> = <xsl:apply-templates select="." mode="enum.item"/> ;
               				}
               			</xsl:for-each>
               			else
               				{
               				cerr &lt;&lt; "Bad value for enum <xsl:value-of select="@name"/>" &lt;&lt; endl;
                   			return EXIT_FAILURE;
               				}
               			</xsl:when>
                   		<xsl:when test="@type='bool'">
                   		prefs()-><xsl:value-of select="@name"/> = !<xsl:value-of select="@default"/>;
                   		</xsl:when>
                   		<xsl:when test="@type='double'">
                   		char* p2;
                   		double d = strtod(optarg,&amp;p2); 
                   		<xsl:if test="@min">
                   		if(d &lt; <xsl:value-of select="@min"/>)
                   			{
                   			cerr &lt;&lt; "<xsl:value-of select="@name"/> should be &gt;=  <xsl:value-of select="@min"/> " &lt;&lt; endl;
                   			return EXIT_FAILURE;
                   			}
                   		</xsl:if>
                   		<xsl:if test="@max">
                   		if(d &gt; <xsl:value-of select="@max"/>)
                   			{
                   			cerr &lt;&lt; "<xsl:value-of select="@name"/> should be &lt;=  <xsl:value-of select="@max"/> " &lt;&lt; endl;
                   			return EXIT_FAILURE;
                   			}
                   		</xsl:if>
                   		prefs()-><xsl:value-of select="@name"/> = d;
                   		</xsl:when>
                   		<xsl:otherwise><xsl:message terminate="yes">getopt:<xsl:value-of select="@type"/></xsl:message></xsl:otherwise>
                   		</xsl:choose>
               			}
               		</xsl:for-each>
                   break;
               case 'W':
               		prefs()->image_width = atoi(optarg);
					if( prefs()->image_width &lt;=0 )
						{
						 std::cerr &lt;&lt; "bad width" &lt;&lt; std::endl;
						 return EXIT_FAILURE;
						}
                   break;
               case 'H':
               		prefs()->image_height = atoi(optarg);
					if( prefs()->image_height &lt;=0 )
						{
						 std::cerr &lt;&lt; "bad height" &lt;&lt; std::endl;
						 return EXIT_FAILURE;
						}
                   break;
               case 'o':
               		save_as_filename = optarg;
               		break;
               case 'v':
                    std::cout &lt;&lt; "VERSION" &lt;&lt; std::endl;
                    return EXIT_SUCCESS;
               		break;
               case 'h':
                    usage(std::cout);
                    return EXIT_SUCCESS;
               		break;
			   }	
			}
	if( save_as_filename == NULL)
		{
		 std::cerr &lt;&lt; "undefined filename" &lt;&lt; std::endl;
		 return EXIT_FAILURE;
		}
	
	XCairo xcairo(prefs()->image_width,prefs()->image_height);
	paint(&amp;xcairo,prefs()->image_width,prefs()->image_height);
	xcairo.png(save_as_filename);
	return EXIT_SUCCESS;
	}



#endif


<xsl:value-of select="$pluginparam"/>* <xsl:value-of select="$abstractpluginname"/>::prefs()
	{
	return &amp;(<xsl:value-of select="$abstractpluginname"/>::PREFS);
	}


const char* <xsl:value-of select="$abstractpluginname"/>::name() const
	{
	return "<xsl:value-of select="$pluginname"/>";
	}

#ifndef STANDALONE
static void _preview_callback(GimpDrawable *drawable, GimpPreview  *preview)
	{
	if(drawable==NULL) return;
	if(preview==NULL) return;
	<xsl:value-of select="$instanceclass"/> instance;
	XDrawable xdrawable(drawable);
	XPreview p(preview);
	instance.run(xdrawable,p);
	}

gboolean <xsl:value-of select="$abstractpluginname"/>::dialog(XDrawable drawable	)
	{
	gimp_ui_init ("<xsl:value-of select="$pluginname"/>", FALSE);
	GtkWidget* dialog = ::gimp_dialog_new (
		"<xsl:value-of select="$pluginname"/>",/* title */
		"<xsl:value-of select="$pluginname"/>",/* role */
                (GtkWidget*)NULL,
                (GtkDialogFlags)0,
                gimp_standard_help_func,
		"plug-in-<xsl:value-of select="$pluginname"/>",
          GTK_STOCK_CANCEL,
          GTK_RESPONSE_CANCEL,
          GTK_STOCK_OK,
          GTK_RESPONSE_OK,
          NULL
          );
    GtkWidget* preview  = NULL;
    ::gimp_window_set_transient (GTK_WINDOW (dialog));
	::gtk_widget_show (dialog);
	
	<xsl:if test="count(.//preview)=1">
	<xsl:variable name="previewid"><xsl:value-of select="generate-id(.//preview[1])"/></xsl:variable>
	/** create preview */
	prefs()->preview=TRUE;
	GtkWidget* <xsl:value-of select="$previewid"/> = ::gimp_drawable_preview_new (
			drawable.drawable(),
			&amp;(prefs()->preview) /* unused anyway */
			);
	preview = <xsl:value-of select="$previewid"/>;
	::g_signal_connect_swapped(
		preview,
		"invalidated",
         G_CALLBACK (_preview_callback),
         drawable.drawable()
         );
	</xsl:if>
	
	<xsl:apply-templates select="*"/>
	<xsl:for-each select="*">
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox),<xsl:value-of select="generate-id(.)"/>);
	</xsl:for-each>
	
    gboolean run = (::gimp_dialog_run (GIMP_DIALOG (dialog)) == GTK_RESPONSE_OK);
    ::gtk_widget_destroy (dialog);
	return run;
	}

GimpPlugInInfo PLUG_IN_INFO = {
	(GimpInitProc)NULL,
	(GimpQuitProc)NULL,
	(GimpQueryProc)<xsl:value-of select="$pluginname"/>_query,
	(GimpRunProc)<xsl:value-of select="$pluginname"/>_run
	};


MAIN ()

#endif
</xsl:template>

<xsl:template match="frame">
        GtkWidget* <xsl:value-of select="generate-id(.)"/> = ::gtk_frame_new (NULL);
        ::gtk_widget_show (<xsl:value-of select="generate-id(.)"/>);
        ::gtk_container_set_border_width (GTK_CONTAINER (<xsl:value-of select="generate-id(.)"/>), 6);

		GtkWidget* <xsl:value-of select="concat('lbl',generate-id(.))"/> = ::gtk_label_new ("<xsl:apply-templates select="." mode="label"/>");
        ::gtk_widget_show (<xsl:value-of select="concat('lbl',generate-id(.))"/>);
        ::gtk_frame_set_label_widget (GTK_FRAME (<xsl:value-of select="generate-id(.)"/>), <xsl:value-of select="concat('lbl',generate-id(.))"/>);
        ::gtk_label_set_use_markup (GTK_LABEL (<xsl:value-of select="concat('lbl',generate-id(.))"/>), TRUE);
        
        <xsl:apply-templates select="*"/>
		<xsl:for-each select="*">
		::gtk_container_add (GTK_CONTAINER (<xsl:value-of select="generate-id(..)"/>), <xsl:value-of select="generate-id(.)"/>);
		</xsl:for-each>
</xsl:template>



<xsl:template match="vbox|hbox">
GtkWidget* <xsl:value-of select="generate-id(.)"/> = <xsl:choose>
		<xsl:when test="name(.)='vbox'">gtk_vbox_new</xsl:when>
		<xsl:otherwise>gtk_hbox_new</xsl:otherwise>
	</xsl:choose>(FALSE, 6);
<xsl:apply-templates select="*"/>
<xsl:for-each select="*">
gtk_box_pack_start (GTK_BOX (<xsl:value-of select="generate-id(..)"/>), <xsl:value-of select="generate-id(.)"/>, TRUE, TRUE, 0);
</xsl:for-each>
gtk_widget_show(<xsl:value-of select="generate-id(.)"/>);
</xsl:template>




<xsl:template match="param[@type='int' or @type='gint' or @type='double'  or @type='gdouble']">
		/* container for <xsl:value-of select="@name"/> */
		GtkWidget* <xsl:value-of select="generate-id(.)"/> = gtk_hbox_new (FALSE, 0);
		gtk_widget_show (<xsl:value-of select="generate-id(.)"/>);
		
		/* label for <xsl:value-of select="@name"/> */
		GtkWidget* <xsl:value-of select="concat('lbl',generate-id(.))"/> = gtk_label_new("<xsl:apply-templates select="." mode="label"/>");
        ::gtk_widget_show (<xsl:value-of select="concat('lbl',generate-id(.))"/> );
        ::gtk_box_pack_start (GTK_BOX (<xsl:value-of select="generate-id(.)"/>), <xsl:value-of select="concat('lbl',generate-id(.))"/> , FALSE, FALSE, 6);
        ::gtk_label_set_justify (GTK_LABEL (<xsl:value-of select="concat('lbl',generate-id(.))"/> ), GTK_JUSTIFY_RIGHT);

		/* tooltip for <xsl:value-of select="@name"/> */
		GtkTooltips* <xsl:value-of select="concat('tooltip',generate-id(.))"/> = gtk_tooltips_new ();
		gtk_tooltips_set_tip(
			<xsl:value-of select="concat('tooltip',generate-id(.))"/>,
			<xsl:value-of select="concat('lbl',generate-id(.))"/>,
			"<xsl:apply-templates select="." mode="description"/>",
			NULL);
		
		gdouble <xsl:value-of select="concat('min_',generate-id(.))"/> = <xsl:choose>
        		<xsl:when test="@min">(gdouble)<xsl:value-of select="@min"/></xsl:when>
        		<xsl:otherwise>(gdouble)(<xsl:value-of select="@default"/> -1)</xsl:otherwise>
        	</xsl:choose>;
        gdouble <xsl:value-of select="concat('max_',generate-id(.))"/> = <xsl:choose>
        		<xsl:when test="@max">(gdouble)<xsl:value-of select="@max"/></xsl:when>
        		<xsl:otherwise>(gdouble)(<xsl:value-of select="@default"/> +1)</xsl:otherwise>
        	</xsl:choose>;
		
		/* adjustment for <xsl:value-of select="@name"/> */
        GtkObject* <xsl:value-of select="concat('adj',generate-id(.))"/> = ::gtk_adjustment_new (
        	(gdouble)<xsl:value-of select="@default"/>,/* value */
        	<xsl:value-of select="concat('min_',generate-id(.))"/>, /* min */
        	<xsl:value-of select="concat('max_',generate-id(.))"/>, /* max */
        	<xsl:choose>
        		<xsl:when test="@step">(gdouble)<xsl:value-of select="@step"/></xsl:when>
        		<xsl:otherwise>(gdouble)((<xsl:value-of select="concat('max_',generate-id(.))"/> - <xsl:value-of select="concat('min_',generate-id(.))"/> )/1000.0)</xsl:otherwise>
        	</xsl:choose>, /* step_increment */
        	<xsl:choose>
        		<xsl:when test="@page">(gdouble)<xsl:value-of select="@page"/></xsl:when>
        		<xsl:otherwise>(gdouble)((<xsl:value-of select="concat('max_',generate-id(.))"/> - <xsl:value-of select="concat('min_',generate-id(.))"/> )/100.0)</xsl:otherwise>
        	</xsl:choose>,  /* page_increment */
        	0 /* page size is always 0 */
        	);
        
        /* spin for <xsl:value-of select="@name"/> */
        GtkWidget* <xsl:value-of select="concat('spin',generate-id(.))"/> = ::gtk_spin_button_new(GTK_ADJUSTMENT (<xsl:value-of select="concat('adj',generate-id(.))"/>), 1 /* climb_rate */, 
        	<xsl:choose>
        		<xsl:when test="@type='int'  or @type='gint'">0</xsl:when>
        		<xsl:otherwise>4</xsl:otherwise>	
        	</xsl:choose> /* digits */
        	);
        ::gtk_widget_show (<xsl:value-of select="concat('spin',generate-id(.))"/>);
        ::gtk_box_pack_start (GTK_BOX (<xsl:value-of select="generate-id(.)"/>), <xsl:value-of select="concat('spin',generate-id(.))"/>, FALSE, FALSE, 6);
        ::gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (<xsl:value-of select="concat('spin',generate-id(.))"/>), TRUE);

		/* connect spin and adjustement */
        g_signal_connect (
        	<xsl:value-of select="concat('adj',generate-id(.))"/>,
			"value_changed",
			<xsl:choose>
        		<xsl:when test="@type='int'  or @type='gint'"> G_CALLBACK (gimp_int_adjustment_update)</xsl:when>
        		<xsl:when test="@type='double' or @type='gdouble'"> G_CALLBACK (gimp_double_adjustment_update)</xsl:when>
        		<xsl:when test="@type='float' or @type='gfloat'"> G_CALLBACK (gimp_float_adjustment_update)</xsl:when>
        		<xsl:otherwise>BOUM</xsl:otherwise>
        	</xsl:choose>, 
             &amp;(prefs()-><xsl:value-of select="@name"/>)
             );
        if(preview!=NULL)
        	{
			 g_signal_connect_swapped (
			  	<xsl:value-of select="concat('adj',generate-id(.))"/>,
			  	"value_changed",
				 G_CALLBACK (gimp_preview_invalidate),
				 preview
				 );
			}
</xsl:template>



<xsl:template match="param[(@type='int' or @type='gint' or @type='double'  or @type='gdouble') and @slider='true']">
		/* container for <xsl:value-of select="@name"/> */
		GtkWidget* <xsl:value-of select="generate-id(.)"/> = gtk_hbox_new (FALSE, 0);
		gtk_widget_show (<xsl:value-of select="generate-id(.)"/>);
		
		/* label for <xsl:value-of select="@name"/> */
		GtkWidget* <xsl:value-of select="concat('lbl',generate-id(.))"/> = gtk_label_new("<xsl:apply-templates select="." mode="label"/>");
        ::gtk_widget_show (<xsl:value-of select="concat('lbl',generate-id(.))"/> );
        ::gtk_box_pack_start (GTK_BOX (<xsl:value-of select="generate-id(.)"/>), <xsl:value-of select="concat('lbl',generate-id(.))"/> , FALSE, FALSE, 6);
        ::gtk_label_set_justify (GTK_LABEL (<xsl:value-of select="concat('lbl',generate-id(.))"/> ), GTK_JUSTIFY_RIGHT);

		/* tooltip for <xsl:value-of select="@name"/> */
		GtkTooltips* <xsl:value-of select="concat('tooltip',generate-id(.))"/> = gtk_tooltips_new ();
		gtk_tooltips_set_tip(
			<xsl:value-of select="concat('tooltip',generate-id(.))"/>,
			<xsl:value-of select="concat('lbl',generate-id(.))"/>,
			"<xsl:apply-templates select="." mode="description"/>",
			NULL);
		
		<xsl:if test="not(@min)">
			<xsl:message terminate="yes">@min missing</xsl:message>
		</xsl:if>
		<xsl:if test="not(@max)">
			<xsl:message terminate="yes">@max missing</xsl:message>
		</xsl:if>
		<xsl:if test="not(@default)">
			<xsl:message terminate="yes">@default missing</xsl:message>
		</xsl:if>
		<xsl:if test="not(@step)">
			<xsl:message terminate="yes">@step missing</xsl:message>
		</xsl:if>
		<xsl:if test="number(@min)&gt;number(@max)">
			<xsl:message terminate="yes">min&gt;max</xsl:message>
		</xsl:if>

		/* adjustment for <xsl:value-of select="@name"/> */
        GtkObject* <xsl:value-of select="concat('adj',generate-id(.))"/> = ::gtk_adjustment_new (
        	(gdouble)<xsl:value-of select="@default"/>,/* value */
        	(gdouble)<xsl:value-of select="@min"/>, /* min */
        	(gdouble)<xsl:value-of select="@max"/>, /* max */
        	(gdouble)<xsl:value-of select="@step"/>, /* step */
        	<xsl:choose>
        		<xsl:when test="@page">(gdouble)<xsl:value-of select="@page"/></xsl:when>
        		<xsl:otherwise>(gdouble)((<xsl:value-of select="@max"/> - <xsl:value-of select="@min"/> )/100.0)</xsl:otherwise>
        	</xsl:choose>, /* page */
        	0 /* page size is always 0 */
        	);


        /* scale for <xsl:value-of select="@name"/> */
        GtkWidget* <xsl:value-of select="concat('scale',generate-id(.))"/> = ::gtk_hscale_new(
        	GTK_ADJUSTMENT(<xsl:value-of select="concat('adj',generate-id(.))"/>)
        	);
        ::gtk_scale_set_draw_value (GTK_SCALE( <xsl:value-of select="concat('scale',generate-id(.))"/>),TRUE);
        ::gtk_widget_show (<xsl:value-of select="concat('scale',generate-id(.))"/>);
        //:gtk_widget_set_size_request
        
        ::gtk_box_pack_start (
        	GTK_BOX (<xsl:value-of select="generate-id(.)"/>),
        	<xsl:value-of select="concat('scale',generate-id(.))"/>,
        	TRUE, TRUE, 6
        	);
        

		/* connect spin and adjustement */
        g_signal_connect (
        	<xsl:value-of select="concat('adj',generate-id(.))"/>,
			"value_changed",
			<xsl:choose>
        		<xsl:when test="@type='int'  or @type='gint'"> G_CALLBACK (gimp_int_adjustment_update)</xsl:when>
        		<xsl:when test="@type='double' or @type='gdouble'"> G_CALLBACK (gimp_double_adjustment_update)</xsl:when>
        		<xsl:when test="@type='float' or @type='gfloat'"> G_CALLBACK (gimp_float_adjustment_update)</xsl:when>
        		<xsl:otherwise>BOUM</xsl:otherwise>
        	</xsl:choose>, 
             &amp;(prefs()-><xsl:value-of select="@name"/>)
             );
        if(preview!=NULL)
        	{
			 g_signal_connect_swapped (
			  	<xsl:value-of select="concat('adj',generate-id(.))"/>,
			  	"value_changed",
				 G_CALLBACK (gimp_preview_invalidate),
				 preview
				 );
			}
</xsl:template>


<xsl:template match="param[@type='boolean' or @type='bool']">

		  GtkWidget* <xsl:value-of select="generate-id(.)"/> = gtk_check_button_new_with_label("<xsl:apply-templates select="." mode="label"/>");
		  ::gtk_toggle_button_set_active (
		  	GTK_TOGGLE_BUTTON (<xsl:value-of select="generate-id(.)"/>),
		  	prefs()-><xsl:value-of select="@name"/>
		  	);
		  ::gtk_widget_show (<xsl:value-of select="generate-id(.)"/>);

		  g_signal_connect (
		  	<xsl:value-of select="generate-id(.)"/>,
		  	"toggled",
			G_CALLBACK (gimp_toggle_button_update),
			 &amp;(<xsl:value-of select="$abstractpluginname"/>::PREFS.<xsl:value-of select="@name"/>)
			 );
		if(preview != NULL)
			{
			::g_signal_connect_swapped (
			  	<xsl:value-of select="generate-id(.)"/>,
			  	"toggled",
				 G_CALLBACK (gimp_preview_invalidate),
				 preview
				 );
			}
		
		/* tooltip for <xsl:value-of select="@name"/> */
		GtkTooltips* <xsl:value-of select="concat('tooltip',generate-id(.))"/> = gtk_tooltips_new ();
		gtk_tooltips_set_tip(
			<xsl:value-of select="concat('tooltip',generate-id(.))"/>,
			<xsl:value-of select="generate-id(.)"/>,
			"<xsl:apply-templates select="." mode="description"/>",
			NULL);		
		
</xsl:template>

<xsl:template match="param[@type='select']">
/* BEGIN SELECT <xsl:value-of select="@name"/> */
		/* container for <xsl:value-of select="@name"/> */
		GtkWidget* <xsl:value-of select="generate-id(.)"/> = gtk_hbox_new (FALSE, 0);
		gtk_widget_show (<xsl:value-of select="generate-id(.)"/>);
		
		/* label for <xsl:value-of select="@name"/> */
		GtkWidget* <xsl:value-of select="concat('lbl',generate-id(.))"/> = gtk_label_new("<xsl:apply-templates select="." mode="label"/>");
        ::gtk_widget_show (<xsl:value-of select="concat('lbl',generate-id(.))"/> );
        ::gtk_box_pack_start (GTK_BOX (<xsl:value-of select="generate-id(.)"/>), <xsl:value-of select="concat('lbl',generate-id(.))"/> , FALSE, FALSE, 6);
        ::gtk_label_set_justify (GTK_LABEL (<xsl:value-of select="concat('lbl',generate-id(.))"/> ), GTK_JUSTIFY_RIGHT);

		/* tooltip for <xsl:value-of select="@name"/> */
		GtkTooltips* <xsl:value-of select="concat('tooltip',generate-id(.))"/> = gtk_tooltips_new ();
		gtk_tooltips_set_tip(
			<xsl:value-of select="concat('tooltip',generate-id(.))"/>,
			<xsl:value-of select="concat('lbl',generate-id(.))"/>,
			"<xsl:apply-templates select="." mode="description"/>",
			NULL);
			
        

        
        
        GtkListStore* <xsl:value-of select="concat('list',generate-id(.))"/> = gtk_list_store_new(1, G_TYPE_STRING);
        GtkTreeIter <xsl:value-of select="concat('iter',generate-id(.))"/>;
		<xsl:for-each select="option">
		gtk_list_store_append(<xsl:value-of select="concat('list',generate-id(..))"/>, &amp;<xsl:value-of select="concat('iter',generate-id(..))"/>);
    	gtk_list_store_set(<xsl:value-of select="concat('list',generate-id(..))"/>, &amp;<xsl:value-of select="concat('iter',generate-id(..))"/>, 0, "<xsl:value-of select="@value"/>", -1);
       	</xsl:for-each>
       	
       	 /* combo for <xsl:value-of select="@name"/> */
        GtkWidget* <xsl:value-of select="concat('combo',generate-id(.))"/> = ::gtk_combo_box_new_with_model (GTK_TREE_MODEL(<xsl:value-of select="concat('list',generate-id(.))"/>));
        ::gtk_combo_box_set_active(GTK_COMBO_BOX(<xsl:value-of select="concat('combo',generate-id(.))"/>), 0);
        ::gtk_widget_show (<xsl:value-of select="concat('combo',generate-id(.))"/>);
        ::gtk_box_pack_start (GTK_BOX (<xsl:value-of select="generate-id(.)"/>), <xsl:value-of select="concat('combo',generate-id(.))"/>, FALSE, FALSE, 6);
        
        
        GtkCellRenderer *<xsl:value-of select="concat('render',generate-id(.))"/> = gtk_cell_renderer_text_new();
   		::gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(<xsl:value-of select="concat('combo',generate-id(.))"/>), <xsl:value-of select="concat('render',generate-id(.))"/>, TRUE);
    	::gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(<xsl:value-of select="concat('combo',generate-id(.))"/>),<xsl:value-of select="concat('render',generate-id(.))"/>, "text", 0, NULL);
        
        ::g_signal_connect(
        	  G_OBJECT( <xsl:value-of select="concat('combo',generate-id(.))"/> ),
        	  "changed",
              G_CALLBACK( cb_<xsl:value-of select="generate-id(.)"/>changed),
              this
              );
        if(preview != NULL)
			{
			::g_signal_connect_swapped (
			  	 <xsl:value-of select="concat('combo',generate-id(.))"/>,
			  	"changed",
				 G_CALLBACK (gimp_preview_invalidate),
				 preview
				 );
			}
       
        
/* END SELECT <xsl:value-of select="@name"/> */
</xsl:template>

<xsl:template match="label">
  /* create label */
  GtkWidget* <xsl:value-of select="generate-id(.)"/> = gtk_label_new("<xsl:choose>
  	<xsl:when test="@label"><xsl:value-of select="@label"/></xsl:when>
  	<xsl:otherwise><xsl:value-of select="text()"/></xsl:otherwise>
  	</xsl:choose>");
  gtk_widget_show (<xsl:value-of select="generate-id(.)"/>);
</xsl:template>

<xsl:template match="split">
  <xsl:choose>
    <xsl:when test="count(*) = 2">
    	/* split pane; contains two children */
    </xsl:when>
    <xsl:otherwise>
    	<xsl:message terminate="yes">split dont have two children</xsl:message>
    </xsl:otherwise>
  </xsl:choose>
  /* create pane */
  GtkWidget* <xsl:value-of select="generate-id(.)"/> = ::gtk_paned_new(<xsl:choose>
  	<xsl:when test="@orient='vertical' or @orientation='vertical'">GTK_ORIENTATION_VERTICAL</xsl:when>
  	<xsl:otherwise>GTK_ORIENTATION_HORIZONTAL</xsl:otherwise>
  	</xsl:choose>);
  gtk_widget_show (<xsl:value-of select="generate-id(.)"/>);
  
  <xsl:apply-templates select="*"/>
  <xsl:for-each select="*">
  ::gtk_paned_pack<xsl:value-of select="position()"/>(
			GTK_PANED (<xsl:value-of select="generate-id(..)"/>),
			<xsl:value-of select="generate-id(.)"/>,
			TRUE,
			FALSE
			);
  </xsl:for-each>
  
</xsl:template>


<xsl:template match="tabbed|notebook">
<xsl:variable name="notebookid"><xsl:value-of select="generate-id(.)"/></xsl:variable>

GtkWidget* <xsl:value-of select="$notebookid"/> = ::gtk_notebook_new();
::gtk_widget_show (<xsl:value-of select="$notebookid"/>);
GtkWidget* <xsl:value-of select="concat('lbl',$notebookid)"/> = NULL;

<xsl:for-each select="tab">
  <xsl:choose>
    <xsl:when test="count(*) = 1">
    	/* TAB */
    </xsl:when>
    <xsl:otherwise>
    	<xsl:message terminate="yes">tab doesn't have one child</xsl:message>
    </xsl:otherwise>
  </xsl:choose>

<xsl:for-each select="*"> <!-- only one -->
<xsl:value-of select="concat('lbl',$notebookid)"/> = ::gtk_label_new("<xsl:value-of select="../@label"/>");
 <xsl:apply-templates select="."/>
::gtk_notebook_append_page (
	GTK_NOTEBOOK(<xsl:value-of select="$notebookid"/>),
    <xsl:value-of select="generate-id(.)"/>,
    <xsl:value-of select="concat('lbl',$notebookid)"/>
    );
</xsl:for-each>

</xsl:for-each>

</xsl:template>


<xsl:template match="preview">
//preview already created
::gtk_widget_show (<xsl:value-of select="generate-id(.)"/>);
</xsl:template>


<xsl:template match="param" mode="instance">
<xsl:text>		this-></xsl:text>
<xsl:value-of select="@name"/>
<xsl:text> = </xsl:text>
<xsl:choose>
	<xsl:when test="@type='int' and @default"><xsl:value-of select="@default"/></xsl:when>
	<xsl:when test="@type='int'">0</xsl:when>
	<xsl:when test="(@type='bool' or @type='boolean') and @default"><xsl:value-of select="@default"/></xsl:when>
	<xsl:when test="(@type='bool' or @type='boolean')">FALSE</xsl:when>
	<xsl:when test="(@type='double' or @type='gdouble') and @default"><xsl:value-of select="@default"/></xsl:when>
	<xsl:when test="(@type='double' or @type='gdouble')">0.0</xsl:when>
	<xsl:when test="(@type='float' or @type='gfloat') and @default">(gfloat)<xsl:value-of select="@default"/></xsl:when>
	<xsl:when test="(@type='float' or @type='gfloat')">(gfloat)0.0</xsl:when>
	<xsl:when test="(@type='select')">
	<xsl:if test="not(option[@default='true'])">
		<xsl:message terminate="yes">param:instance not default for '<xsl:value-of select="@name"/></xsl:message>
	</xsl:if>
			<xsl:apply-templates select="option[@default='true']" mode="enum.item"/>
	</xsl:when>
	<xsl:otherwise><xsl:message terminate="yes">param:instance boum '<xsl:value-of select="@type"/>/<xsl:value-of select="@name"/>'</xsl:message></xsl:otherwise>
</xsl:choose>
<xsl:text>;
</xsl:text>

</xsl:template>


</xsl:stylesheet>
