<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" xmlns="http://www.w3.org/1999/xhtml">
<xsl:output method="text" encoding="utf-8" />
<xsl:include href="General.xsl"/>

<xsl:template match="/controller">

<xsl:call-template name="header">
	<xsl:with-param name="feature" select="'ExternalInterrupt'"/>
	<xsl:with-param name="definition" select="'EXTERNAL_INTERRUPT'"/>
</xsl:call-template>

/**********************************************************************************************************************\

	<xsl:value-of select="@name"/> external interrupts declarations.
	This file is generated. Do not modify manually !!!

\**********************************************************************************************************************/

<xsl:variable name="feature" select="features/feature[@name='ExternalInterrupt']"/>
<xsl:if test="$feature">
<xsl:for-each select="$feature/external_interrupt">#define INT<xsl:value-of select="@nr"/>_ns	ExternalInterrupt
</xsl:for-each>
<xsl:for-each select="$feature/pin_change_interrupt">#define PCINT<xsl:value-of select="@nr"/>_ns	ExternalInterrupt
</xsl:for-each>

<xsl:for-each select="$feature/external_interrupt">
#define INT<xsl:value-of select="@nr"/>_struct	ExternalInterrupt::Interrupt<xsl:value-of select="@nr"/>
</xsl:for-each>
<xsl:for-each select="$feature/pin_change_interrupt">
#define PCINT<xsl:value-of select="@nr"/>_struct	ExternalInterrupt::PinChangeInterrupt<xsl:value-of select="@nr"/>
</xsl:for-each>

namespace AVRCpp
{		
	namespace ExternalInterrupt
	{
<xsl:for-each select="$feature/external_interrupt">	
		struct Interrupt<xsl:value-of select="@nr"/> : Internal::InterruptBase &lt;
			InputPin<xsl:value-of select="pin/@pin"/>&lt;Port<xsl:value-of select="pin/@port"/>&gt;,				/* InputPin */
			<xsl:choose>
	<xsl:when test="@detecton='sync'">Event</xsl:when>
	<xsl:when test="@detecton='async'">AsyncEvent</xsl:when>
	<xsl:when test="@detecton='simple_async'">SimpleAsyncEvent</xsl:when>	
</xsl:choose>,						/* EventEnum */
			Bits&lt;_<xsl:value-of select="register[@name='control']/@register"/>, <xsl:choose>
<xsl:when test="@detecton='simple_async'">_ISC<xsl:value-of select="@nr"/></xsl:when>
<xsl:otherwise>_ISC<xsl:value-of select="@nr"/>0 | _ISC<xsl:value-of select="@nr"/>1</xsl:otherwise>
</xsl:choose>&gt;,	/* EventBits */
			Bits&lt;_<xsl:value-of select="register[@name='interrupt_control']/@register"/>, _INT<xsl:value-of select="@nr"/>&gt;,			/* InterruptEnableBit */
			Bits&lt;_<xsl:value-of select="register[@name='interrupt_flag']/@register"/>, _INTF<xsl:value-of select="@nr"/>&gt;,			/* InterruptFlagBit */
			<xsl:value-of select="@shift"/> &gt;								/* eventShift */
			
		{ __INTERRUPT_HANDLER_SUPPORT__ }; // Interrupt<xsl:value-of select="@nr"/>

</xsl:for-each>

<xsl:for-each select="$feature/pin_change_interrupt">
		struct PinChangeInterrupt<xsl:value-of select="@nr"/> : Internal::PinChangeInterruptBase &lt; 
			_<xsl:value-of select="register[@name='mask']/@register"/>,							/* MaskRegister */
			Bits&lt;_<xsl:value-of select="register[@name='interrupt_control']/@register"/>, _PCIE<xsl:value-of select="@nr"/>&gt;,				/* InterruptEnableBit */ 
			Bits&lt;_<xsl:value-of select="register[@name='interrupt_flag']/@register"/>, _PCIF<xsl:value-of select="@nr"/>&gt; &gt;				/* InterruptFlagBit */

		{ __INTERRUPT_HANDLER_SUPPORT__ }; // PinChangeInterrupt<xsl:value-of select="@nr"/>

</xsl:for-each>	

	} // namespace ExternalInterrupts

} // namespace AVRCpp
</xsl:if>

<xsl:call-template name="footer">
	<xsl:with-param name="feature" select="'ExternalInterrupt'"/>
	<xsl:with-param name="definition" select="'EXTERNAL_INTERRUPT'"/>
</xsl:call-template>

</xsl:template>

</xsl:stylesheet>
