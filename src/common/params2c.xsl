<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="text"/>
<xsl:param name="pluginname">My</xsl:param>
<xsl:template match="/">
<xsl:apply-templates select="params"/>
</xsl:template>

<xsl:template match="params">
typedef struct
	{
	<xsl:apply-templates select="//param" mode="field"/>
	gboolean preview;
	} <xsl:value-of select="$pluginname"/>Vals;

static <xsl:value-of select="$pluginname"/>Vals
	{
	<xsl:apply-templates select="//param" mode="instance"/>
	1 /* preview */
	} <xsl:value-of select="$pluginname"/>vals;

static gboolean dialog(GimpDrawable drawable	)
	{
	gimp_ui_init ("<xsl:value-of select="$pluginname"/>", FALSE);
	GtkWidget* dialog = ::gimp_dialog_new (
		"<xsl:value-of select="$pluginname"/>",
		"<xsl:value-of select="$pluginname"/>",
                NULL, 0,
                gimp_standard_help_func,
		"plug-in-<xsl:value-of select="$pluginname"/>",
                                  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                  GTK_STOCK_OK,     GTK_RESPONSE_OK,
                                  NULL);
	::gtk_widget_show (dialog);
	GtkWidget* <xsl:value-of select="generate-id()"/> = ::gtk_vbox_new(FALSE,6);
        ::gtk_container_add (GTK_CONTAINER (GTK_DIALOG (dialog)->vbox), <xsl:value-of select="generate-id()"/>);
        ::gtk_widget_show (<xsl:value-of select="generate-id()"/>);

	<xsl:apply-templates select="*"/>

        gboolean run = (gimp_dialog_run (GIMP_DIALOG (dialog)) == GTK_RESPONSE_OK);
        ::gtk_widget_destroy (dialog);
	return run;
	}

</xsl:template>

<xsl:template match="frame">
        GtkWidget* <xsl:value-of select="generate-id(.)"/> = gtk_frame_new (NULL);
        gtk_widget_show (<xsl:value-of select="generate-id(.)"/>);
        gtk_box_pack_start (GTK_BOX (<xsl:value-of select="generate-id(..)"/>), <xsl:value-of select="generate-id(.)"/>, TRUE, TRUE, 0);
        gtk_container_set_border_width (GTK_CONTAINER (frame), 6);

	frame_label = gtk_label_new ("Modify radius");
        ::gtk_widget_show (frame_label);
        ::gtk_frame_set_label_widget (GTK_FRAME (frame), frame_label);
        ::gtk_label_set_use_markup (GTK_LABEL (frame_label), TRUE);
</xsl:template>

<xsl:template match="param[@type='spin']">
        spinbutton_adj = gtk_adjustment_new (3, 1, 16, 1, 5, 5);
        spinbutton = gtk_spin_button_new (GTK_ADJUSTMENT (spinbutton_adj), 1, 0);
        gtk_widget_show (spinbutton);
        gtk_box_pack_start (GTK_BOX (main_hbox), spinbutton, FALSE, FALSE, 6);
        gtk_spin_button_set_numeric (GTK_SPIN_BUTTON (spinbutton), TRUE);

        frame_label = gtk_label_new ("Modify radius");
        gtk_widget_show (frame_label);
        gtk_frame_set_label_widget (GTK_F        spinbutton_adj = gtk_adjustment_new (3, 1, 16, 1, 5, 5);
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

</xsl:template>


</xsl:stylesheet>
