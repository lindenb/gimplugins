<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="text"/>
<xsl:variable name="pluginname" select="/plugin/@name"/>

<xsl:template match="/">

#ifndef __<xsl:value-of select="$pluginname"/>_HEADER_
#define __<xsl:value-of select="$pluginname"/>_HEADER_
/** BEGIN : AUTO GENERATED CODE **/
<xsl:apply-templates select="plugin"/>


/** END   : AUTO GENERATED CODE **/
#endif
</xsl:template>

<xsl:template match="plugin">


static void      query  (void);
static void      run    (gchar *name, gint nparams, GimpParam *param, gint *nreturn_vals, GimpParam **return_vals);


GimpPlugInInfo PLUG_IN_INFO = {(GimpInitProc)NULL,(GimpQuitProc)NULL,(GimpQueryProc)query,(GimpRunProc)run};


MAIN ()

static void query (void) {
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



/** structure holding preferences */
typedef struct <xsl:value-of select="$pluginname"/>Vals_t
	{
	<xsl:apply-templates select="//param" mode="field"/>
	gboolean preview;
	} <xsl:value-of select="$pluginname"/>Vals;

/** plugin class */
class <xsl:value-of select="$pluginname"/> : public AbstractPlugin&lt;<xsl:value-of select="$pluginname"/>Vals&gt;
	{
	public:
		static <xsl:value-of select="$pluginname"/>Vals PREFS;
		<xsl:value-of select="$pluginname"/>()
			{
			}
		virtual <xsl:value-of select="$pluginname"/>Vals* prefs();
		virtual const char* name() const;
		virtual gboolean dialog(XDrawable drawable);
		virtual void run (XDrawable drawable, XPreview preview);
		
	};


// run plug-in is the heart of the plug-in program
// the parameters are name, inputparameters and a pointer to output parameters.
static void run (
     gchar   *name,
     gint     nparams,
     GimpParam  *param,
     gint    *nreturn_vals,
     GimpParam **return_vals)
	{
	<xsl:value-of select="$pluginname"/> instance;
	instance.run1(name,nparams,param,nreturn_vals,return_vals);
	}




<xsl:value-of select="$pluginname"/>Vals
<xsl:value-of select="$pluginname"/>::PREFS={
<xsl:apply-templates select="//param" mode="instance"/>
1 /* preview */
};

<xsl:value-of select="$pluginname"/>Vals* <xsl:value-of select="$pluginname"/>::prefs()
	{
	return &amp;(<xsl:value-of select="$pluginname"/>::PREFS);
	}

const char* <xsl:value-of select="$pluginname"/>::name() const
	{
	return "<xsl:value-of select="$pluginname"/>";
	}

gboolean <xsl:value-of select="$pluginname"/>::dialog(XDrawable drawable	)
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
          
    ::gimp_window_set_transient (GTK_WINDOW (dialog));
	::gtk_widget_show (dialog);
	
	<xsl:if test="count(//preview)=1">
	/** create preview */
	GtkWidget* <xsl:value-of select="generate-id(//preview[1])"/> = ::gimp_drawable_preview_new (
			drawable.drawable(),
			&amp;(<xsl:value-of select="$pluginname"/>::PREFS.preview)
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

<xsl:template match="param[@type='int']">
		/* container for <xsl:value-of select="@name"/> */
		GtkWidget* <xsl:value-of select="generate-id(.)"/> = gtk_hbox_new (FALSE, 0);
		gtk_widget_show (<xsl:value-of select="generate-id(.)"/>);
		
		/* label for <xsl:value-of select="@name"/> */
		<xsl:value-of select="concat('lbl',generate-id(.))"/> = gtk_label_new("<xsl:apply-templates select="." mode="label"/>");
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
	
		/* adjustment for <xsl:value-of select="@name"/> */
        GtkWidget* <xsl:value-of select="concat('adj',generate-id(.))"/> = ::gtk_adjustment_new (
        	(gfloat)<xsl:value-of select="@default"/>,
        	<xsl:choose>
        		<xsl:when test="@min">(gfloat)<xsl:value-of select="@min"/></xsl:when>
        		<xsl:otherwise>(gfloat)0</xsl:otherwise>
        	</xsl:choose>, 
        	<xsl:choose>
        		<xsl:when test="@max">(gfloat)<xsl:value-of select="@max"/></xsl:when>
        		<xsl:otherwise>(gfloat)100</xsl:otherwise>
        	</xsl:choose>, 
        	<xsl:choose>
        		<xsl:when test="@step">(gfloat)<xsl:value-of select="@step"/></xsl:when>
        		<xsl:otherwise>(gfloat)1</xsl:otherwise>
        	</xsl:choose>, 
        	<xsl:choose>
        		<xsl:when test="@page">(gfloat)<xsl:value-of select="@page"/></xsl:when>
        		<xsl:otherwise>(gfloat)100</xsl:otherwise>
        	</xsl:choose>, 
        	<xsl:choose>
        		<xsl:when test="@page-size">(gfloat)<xsl:value-of select="@page-size"/></xsl:when>
        		<xsl:otherwise>(gfloat)100</xsl:otherwise>
        	</xsl:choose>
        	);
        
        /* spin for <xsl:value-of select="@name"/> */
        GtkWidget* <xsl:value-of select="concat('spin',generate-id(.))"/> = ::gtk_spin_button_new(GTK_ADJUSTMENT (<xsl:value-of select="concat('adj',generate-id(.))"/>), 1 /* climb_rate */,  0 /* digits * /);
        ::gtk_widget_show (<xsl:value-of select="concat('spin',generate-id(.))"/>);
        ::gtk_box_pack_start (GTK_BOX (<xsl:value-of select="generate-id(.)"/>), <xsl:value-of select="concat('spin',generate-id(.))"/>, FALSE, FALSE, 6);
        ::gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (<xsl:value-of select="concat('spin',generate-id(.))"/>), TRUE);

		/* connect spin and adjustement */
        g_signal_connect (
        	<xsl:value-of select="concat('adj',generate-id(.))"/>,
			"value_changed",
             G_CALLBACK (gimp_int_adjustment_update),
             &amp;(<xsl:value-of select="$pluginname"/>vals.<xsl:value-of select="@name"/>)
             );
		 g_signal_connect_swapped (
		  	<xsl:value-of select="concat('adj',generate-id(.))"/>,
		  	"value_changed",
			 G_CALLBACK (gimp_preview_invalidate),
			 preview
			 );
</xsl:template>


<xsl:template match="param[@type='boolean' or @type='bool']">

		  GtkWidget* <xsl:value-of select="generate-id(.)"/> = gtk_check_button_new_with_label("<xsl:apply-templates select="." mode="label"/>");
		  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (<xsl:value-of select="generate-id(.)"/>), <xsl:value-of select="$pluginname"/>vals.<xsl:value-of select="@name"/>);
		  gtk_widget_show (<xsl:value-of select="generate-id(.)"/>);

		  g_signal_connect (
		  	<xsl:value-of select="generate-id(.)"/>,
		  	"toggled",
			G_CALLBACK (gimp_toggle_button_update),
			 &amp;(<xsl:value-of select="$pluginname"/>vals.<xsl:value-of select="@name"/>)
			 );
		<xsl:if test="count(//preview)=1">
		  g_signal_connect_swapped (
		  	<xsl:value-of select="generate-id(.)"/>,
		  	"toggled",
			 G_CALLBACK (gimp_preview_invalidate),
			 preview
			 );
		</xsl:if> 
		
		/* tooltip for <xsl:value-of select="@name"/> */
		GtkTooltips* <xsl:value-of select="concat('tooltip',generate-id(.))"/> = gtk_tooltips_new ();
		gtk_tooltips_set_tip(
			<xsl:value-of select="concat('tooltip',generate-id(.))"/>,
			<xsl:value-of select="generate-id(.)"/>,
			"<xsl:apply-templates select="." mode="description"/>",
			NULL);		
		

</xsl:template>

<xsl:template match="preview">
//preview already created
::gtk_widget_show (<xsl:value-of select="generate-id(.)"/>);
</xsl:template>

<xsl:template match="param" mode="field">
/* <xsl:apply-templates select="." mode="label"/> : <xsl:apply-templates select="." mode="description"/> */
	<xsl:choose>
		<xsl:when test="@type='int'">gint</xsl:when>
		<xsl:when test="@type='bool' or @type='boolean'">gboolean</xsl:when>
		<xsl:otherwise><xsl:value-of select="@type"/></xsl:otherwise>
	</xsl:choose>
<xsl:text>	</xsl:text>
<xsl:value-of select="@name"/>
<xsl:text>;
</xsl:text>
</xsl:template>

<xsl:template match="*" mode="label">
<xsl:choose>
	<xsl:when test="not(@label)"><xsl:value-of select="@name"/></xsl:when>
	<xsl:otherwise><xsl:value-of select="@label"/></xsl:otherwise>
</xsl:choose>
</xsl:template>


<xsl:template match="*" mode="description">
<xsl:choose>
	<xsl:when test="not(@description)"><xsl:apply-templates select="." mode="label"/></xsl:when>
	<xsl:otherwise><xsl:value-of select="@description"/></xsl:otherwise>
</xsl:choose>
</xsl:template>

<xsl:template match="param" mode="instance">
<xsl:choose>
	<xsl:when test="@type='int' and @default"><xsl:value-of select="@default"/></xsl:when>
	<xsl:when test="@type='int'">0</xsl:when>
	<xsl:when test="(@type='bool' or @type='boolean') and @default"><xsl:value-of select="@default"/></xsl:when>
	<xsl:when test="(@type='bool' or @type='boolean')">FALSE</xsl:when>
	<xsl:otherwise><xsl:message terminate="yes">boum</xsl:message></xsl:otherwise>
</xsl:choose>
<xsl:text>, /* </xsl:text>
<xsl:value-of select="@name"/>
<xsl:text> */
</xsl:text>

</xsl:template>


</xsl:stylesheet>
