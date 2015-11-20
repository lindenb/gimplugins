<?xml version='1.0' ?>
<xsl:stylesheet
	xmlns:xsl='http://www.w3.org/1999/XSL/Transform'
	xmlns="http://www.w3.org/1999/xhtml"
	version='1.0'
	>
<xsl:output method="xml" indent="no" />

<xsl:template match="/">
<xsl:apply-templates/>
</xsl:template>

<xsl:template match="@*">
<xsl:copy select="."/>
</xsl:template>


<xsl:template match="*">
<xsl:copy select=".">
<xsl:apply-templates select="@*"/>
<xsl:apply-templates select="*|text()"/>
</xsl:copy>
</xsl:template>

</xsl:stylesheet>

