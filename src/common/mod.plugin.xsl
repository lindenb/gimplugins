<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="text"/>
<xsl:variable name="basepluginname" select="/plugin/@name"/>
<xsl:variable name="pluginname" select="concat('abstract_',$basepluginname)"/>

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


</xsl:stylesheet>
