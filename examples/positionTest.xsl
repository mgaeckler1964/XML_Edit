<?xml version="1.0" encoding="iso-8859-1" ?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

	<xsl:output method="html" indent="yes" encoding="iso-8859-1" />

	<xsl:template match='/|*'>
		<xsl:apply-templates />
	</xsl:template>

	<xsl:template match="Liste">
	<html><body>
	  <xsl:for-each select="*">
	  <p>
	    <xsl:value-of select="position()" />
	    <xsl:text>: </xsl:text>
	    <xsl:value-of select="." />
	  </p>
	  </xsl:for-each>
	</body></html>
	</xsl:template>
</xsl:stylesheet>

