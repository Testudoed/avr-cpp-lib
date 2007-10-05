<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" xmlns="http://www.w3.org/1999/xhtml">
<xsl:output method="text" encoding="utf-8"/>
<xsl:include href="General.xsl"/>

<xsl:template match="/controller">

<xsl:call-template name="header">
	<xsl:with-param name="feature" select="'SPI'"/>
</xsl:call-template>

/**********************************************************************************************************************\

	<xsl:value-of select="@name"/> serial peripheral interace (SPI) declarations.
	This file is generated. Do not modify manually !!!

\**********************************************************************************************************************/

<xsl:variable name="SPI" select="features/feature[@name='SPI']"/>
<xsl:if test="$SPI">

#define SPI_STC_ns		SPI
#define SPI_STC_struct	SPI::SPI0::TransmissionCompleteInterrupt

namespace AVRCpp
{
	namespace SPI
	{
		// Functions from this namespace should not be called by user
		namespace Internal
		{
			bool <![CDATA[&]]>Cancel();

		}; // namespace Internal

		struct SPI0 : Internal::SPIBase &lt;
				_SPCR,							/* ControlRegister */
				_SPSR,							/* StatusRegister */
				_SPDR,							/* DataRegister */				
				Pin<xsl:value-of select="$SPI/pin[@name='SS']/@nr"/>&lt;Port<xsl:value-of select="$SPI/pin[@name='SS']/@port"/>&gt;,		/* SlaveSelectPin */	
				Pin<xsl:value-of select="$SPI/pin[@name='SCK']/@nr"/>&lt;Port<xsl:value-of select="$SPI/pin[@name='SCK']/@port"/>&gt;,		/* ClockPin */
				Pin<xsl:value-of select="$SPI/pin[@name='MOSI']/@nr"/>&lt;Port<xsl:value-of select="$SPI/pin[@name='MOSI']/@port"/>&gt;,		/* MasterOutPin */
				Pin<xsl:value-of select="$SPI/pin[@name='MISO']/@nr"/>&lt;Port<xsl:value-of select="$SPI/pin[@name='MISO']/@port"/>&gt; &gt;		/* MasterInPin */			
		{
			struct TransmissionCompleteInterrupt : BasicInterrupt&lt;Bits&lt;_SPCR, _SPIE&gt; &gt; { __INTERRUPT_HANDLER_SUPPORT__ };

		}; // struct SPI0

	} // namespace SPI

} // namespace AVRCpp
</xsl:if>

<xsl:call-template name="footer">
	<xsl:with-param name="feature" select="'IO'"/>
</xsl:call-template>

</xsl:template>

</xsl:stylesheet>
