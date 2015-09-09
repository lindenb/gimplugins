<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:import href="mod.plugin.xsl"/>
<xsl:output method="text"/>



<xsl:template match="/">

#ifndef __<xsl:value-of select="$pluginname"/>_HEADER_
#define __<xsl:value-of select="$pluginname"/>_HEADER_
/** BEGIN : AUTO GENERATED CODE **/

#include &lt;libgimp/gimp.h&gt;
#include &lt;libgimp/gimpui.h&gt;
#include &lt;libgimpwidgets/gimpwidgets.h&gt;
#include "common/abstractplugin.hh"

<xsl:apply-templates select="plugin"/>

/** END   : AUTO GENERATED CODE **/
#endif
</xsl:template>

<xsl:template match="plugin">


/** structure holding preferences */
typedef struct <xsl:value-of select="$pluginname"/>Vals_t
	{
	<xsl:apply-templates select="//param" mode="field"/>
	gboolean preview;
	} <xsl:value-of select="$pluginname"/>Vals;

/** plugin class */
class <xsl:value-of select="$pluginname"/> : public <xsl:choose>
		<xsl:when test="@extend"><xsl:value-of select="@extend"/></xsl:when>
		<xsl:otherwise>AbstractPlugin&lt;<xsl:value-of select="$pluginname"/>Vals&gt;</xsl:otherwise>
	</xsl:choose>
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


</xsl:template>

<xsl:template match="param" mode="field">
/* <xsl:apply-templates select="." mode="label"/> : <xsl:apply-templates select="." mode="description"/> */
	<xsl:choose>
		<xsl:when test="@type='int' or @type='gint'">gint</xsl:when>
		<xsl:when test="@type='bool' or @type='boolean'">gboolean</xsl:when>
		<xsl:when test="@type='double' or @type='gdouble'">gdouble</xsl:when>
		<xsl:when test="@type='float' or @type='gfloat'">gfloat</xsl:when>
		<xsl:otherwise><xsl:value-of select="@type"/></xsl:otherwise>
	</xsl:choose>
<xsl:text>	</xsl:text>
<xsl:value-of select="@name"/>
<xsl:text>;
</xsl:text>
</xsl:template>


</xsl:stylesheet>
