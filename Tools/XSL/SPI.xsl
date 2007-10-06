<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" xmlns="http://www.w3.org/1999/xhtml">
<xsl:output method="text" encoding="utf-8"/>
<xsl:include href="General.xsl"/>

<xsl:template match="/controller">

<xsl:call-template name="header">
	<xsl:with-param name="feature" select="'SPI'"/>
	<xsl:with-param name="definition" select="'SPI'"/>
</xsl:call-template>

/**********************************************************************************************************************\

	<xsl:value-of select="@name"/> serial peripheral interace (SPI) declarations.
	This file is generated. Do not modify manually !!!

\**********************************************************************************************************************/

<xsl:variable name="feature" select="features/feature[@name='SPI']"/>
<xsl:if test="$feature">
#define SPI_STC_ns		SPI
<xsl:for-each select="$feature/spi">#define SPI_STC_struct	SPI::SPI<xsl:value-of select="@nr"/>::TransmissionCompleteInterrupt
</xsl:for-each>
namespace AVRCpp
{
	namespace SPI
	{
		// Functions from this namespace should not be called by user
		namespace Internal
		{
			bool <![CDATA[&]]>Cancel();

		}; // namespace Internal
<xsl:for-each select="$feature/spi">
		struct SPI<xsl:value-of select="@nr"/> : Internal::SPIBase &lt;
				_SPCR,					/* ControlRegister */
				_SPSR,					/* StatusRegister */
				_SPDR,					/* DataRegister */				
				Pin<xsl:value-of select="pin[@name='SS']/@pin"/>&lt;Port<xsl:value-of select="pin[@name='SS']/@port"/>&gt;,			/* SlaveSelectPin */	
				Pin<xsl:value-of select="pin[@name='SCK']/@pin"/>&lt;Port<xsl:value-of select="pin[@name='SCK']/@port"/>&gt;,			/* ClockPin */
				Pin<xsl:value-of select="pin[@name='MOSI']/@pin"/>&lt;Port<xsl:value-of select="pin[@name='MOSI']/@port"/>&gt;,			/* MasterOutPin */
				Pin<xsl:value-of select="pin[@name='MISO']/@pin"/>&lt;Port<xsl:value-of select="pin[@name='MISO']/@port"/>&gt; &gt;			/* MasterInPin */			
		{
			struct TransmissionCompleteInterrupt : BasicInterrupt&lt;Bits&lt;_SPCR, _SPIE&gt; &gt; { __INTERRUPT_HANDLER_SUPPORT__ };

		}; // struct SPI<xsl:value-of select="@nr"/>

</xsl:for-each>

	} // namespace SPI

} // namespace AVRCpp
</xsl:if>

<xsl:call-template name="footer">
	<xsl:with-param name="feature" select="'SPI'"/>
	<xsl:with-param name="definition" select="'SPI'"/>
</xsl:call-template>

</xsl:template>

</xsl:stylesheet>
