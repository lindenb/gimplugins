<?xml version='1.0' ?>
<xsl:stylesheet
	xmlns:xsl='http://www.w3.org/1999/XSL/Transform'
	version='1.0'
	>
<xsl:output method="xml" indent="no" />

<xsl:template match="/">
<xsl:apply-templates/>
</xsl:template>

<xsl:template match="@*">
<xsl:copy select="."/>
</xsl:template>


<xsl:template match="frame[@id='line-width']">
	<minmax label="Line Width" type="double" name="linewidth" min="0" max="20" beg="0.1" end="1.5" />
</xsl:template>

<xsl:template match="frame[@id='alpha']">
	<minmax label="Alpha" type="double" name="alpha" min="0" max="1.00" beg="0.7" end="1.0" />
</xsl:template>

<xsl:template match="minmax">
	<frame >
		<xsl:attribute name="label">
			<xsl:apply-templates select="." mode="label"/>
		</xsl:attribute>
		<vbox>
			<param>
				<xsl:attribute name="name"><xsl:value-of select="concat('min',@name)"/></xsl:attribute>
				<xsl:attribute name="type"><xsl:value-of select="@type"/></xsl:attribute>
				<xsl:attribute name="label">Min. <xsl:apply-templates select="." mode="label"/></xsl:attribute>
				<xsl:attribute name="default">
					<xsl:choose>
						<xsl:when test="@begin"><xsl:value-of select="@begin"/></xsl:when>
						<xsl:otherwise><xsl:value-of select="@min"/></xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
				<xsl:attribute name="min"><xsl:value-of select="@min"/></xsl:attribute>
				<xsl:attribute name="max"><xsl:value-of select="@max"/></xsl:attribute>
			</param>
			<param>
				<xsl:attribute name="name"><xsl:value-of select="concat('max',@name)"/></xsl:attribute>
				<xsl:attribute name="type"><xsl:value-of select="@type"/></xsl:attribute>
				<xsl:attribute name="label">Max. <xsl:apply-templates select="." mode="label"/></xsl:attribute>
				<xsl:attribute name="default">
					<xsl:choose>
						<xsl:when test="@end"><xsl:value-of select="@end"/></xsl:when>
						<xsl:otherwise><xsl:value-of select="@max"/></xsl:otherwise>
					</xsl:choose>
				</xsl:attribute>
				<xsl:attribute name="min"><xsl:value-of select="@min"/></xsl:attribute>
				<xsl:attribute name="max"><xsl:value-of select="@max"/></xsl:attribute>
			</param>
		</vbox>
	</frame>
</xsl:template>

<xsl:template match="*">
<xsl:copy select=".">
<xsl:apply-templates select="@*"/>
<xsl:apply-templates select="*|text()"/>
</xsl:copy>
</xsl:template>

<xsl:template match="*" mode="label">
	<xsl:choose>
		<xsl:when test="@label"><xsl:value-of select="@label"/></xsl:when>
		<xsl:otherwise><xsl:value-of select="@name"/></xsl:otherwise>
	</xsl:choose>
</xsl:template>



</xsl:stylesheet>

