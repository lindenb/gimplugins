<?xml version="1.0" encoding="UTF-8" ?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">

<xsl:output method="text"/>
<xsl:variable name="pluginname" select="/plugin/@name"/>


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


</xsl:stylesheet>
