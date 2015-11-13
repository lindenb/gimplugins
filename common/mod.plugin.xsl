<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="text"/>


<xsl:variable name="pluginname" select="/plugin/@name"/>

<xsl:variable name="plugintitle">
<xsl:call-template name="titleize">
  <xsl:with-param name="s" select="$pluginname"/>
</xsl:call-template>
</xsl:variable>

<xsl:variable name="abstractpluginname" select="concat('Abstract',$plugintitle)"/>
<xsl:variable name="pluginparam" select="concat($plugintitle,'Prefs')"/>

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



<xsl:template name="titleize">
  <xsl:param name="s"/>
  <xsl:value-of select="concat(translate(substring($s,1,1),'abcdefghijklmnopqrstuvwxyz','ABCDEFGHIJKLMNOPQRSTUVWXYZ'),substring($s,2))"/>
</xsl:template>

</xsl:stylesheet>
