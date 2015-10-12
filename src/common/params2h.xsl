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
#ifdef STANDALONE
	gint image_width;
	gint image_height;
#else
	gboolean preview;
#endif
	} <xsl:value-of select="$pluginname"/>Vals;

/** abstract plugin class */
class <xsl:value-of select="$pluginname"/> : public <xsl:choose>
		<xsl:when test="@extend"><xsl:value-of select="@extend"/></xsl:when>
		<xsl:otherwise>AbstractCairoPlugin&lt;<xsl:value-of select="$pluginname"/>Vals&gt;</xsl:otherwise>
	</xsl:choose>
	{
	public:
		static <xsl:value-of select="$pluginname"/>Vals PREFS;
		<xsl:value-of select="$pluginname"/>()
			{
			}
		virtual ~<xsl:value-of select="$pluginname"/>()
			{
			}
		virtual <xsl:value-of select="$pluginname"/>Vals* prefs();
		virtual const char* name() const;
		virtual void paint(XCairo* cairo,gint width,gint height)=0;
		#ifndef STANDALONE
		virtual gboolean dialog(XDrawable drawable);
		#else
		virtual void usage(std::ostream&amp; out);
		int main(int argc,char** argv);
		#endif
		
		
	};


</xsl:template>



</xsl:stylesheet>
