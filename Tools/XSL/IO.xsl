<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" xmlns="http://www.w3.org/1999/xhtml">
<xsl:output method="text" encoding="utf-8"/>
<xsl:include href="General.xsl"/>

<xsl:template match="/controller">

<xsl:call-template name="header">
	<xsl:with-param name="feature" select="'IO'"/>
	<xsl:with-param name="definition" select="'IO'"/>
</xsl:call-template>

/**********************************************************************************************************************\

	<xsl:value-of select="@name"/> registers, bits and ports declarations.
	This file is generated. Do not modify manually !!!

\**********************************************************************************************************************/

// Registers
namespace AVRCpp
{<xsl:for-each select="registers/register">
	__DECLARE_<xsl:value-of select="@bits"/>BIT_REGISTER__(<xsl:value-of select="@name"/>);</xsl:for-each>

} // namespace AVRCpp
<xsl:for-each select="registers/register">
<xsl:variable name="register" select="."/>
<xsl:if test="count($register/bits/bit) > 0">

// <xsl:value-of select="$register/@name"/>
	<xsl:for-each select="$register/bits/bit">
#define _<xsl:analyze-string select="@name" flags="i" regex="PORT(..)">
			<xsl:matching-substring>P<xsl:value-of select="regex-group(1)" /></xsl:matching-substring>
			<xsl:non-matching-substring><xsl:value-of select="." /></xsl:non-matching-substring>
		</xsl:analyze-string><xsl:value-of select="concat('	', @address)"/>
	</xsl:for-each>
</xsl:if>
</xsl:for-each>


// General ports
namespace AVRCpp
{<xsl:for-each select="ports/port"><xsl:variable name="port" select="."/>
	__DECLARE_PORT__(<xsl:value-of select="@char"/>);</xsl:for-each>
	
} // namespace AVRCpp

<xsl:call-template name="footer">
	<xsl:with-param name="feature" select="'IO'"/>
	<xsl:with-param name="definition" select="'IO'"/>
</xsl:call-template>

</xsl:template>

</xsl:stylesheet>
