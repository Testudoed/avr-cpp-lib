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
<xsl:for-each select="$feature/interrupt">#define INT<xsl:value-of select="@nr"/>_ns	ExternalInterrupt
</xsl:for-each>
<xsl:for-each select="$feature/pin_change_interrupt">#define PCINT<xsl:value-of select="@nr"/>_ns	ExternalInterrupt
</xsl:for-each>

<xsl:for-each select="$feature/interrupt">
#define INT<xsl:value-of select="@nr"/>_struct	ExternalInterrupt::Interrupt<xsl:value-of select="@nr"/>
</xsl:for-each>
<xsl:for-each select="$feature/pin_change_interrupt">
#define PCINT<xsl:value-of select="@nr"/>_struct	ExternalInterrupt::PinChangeInterrupt<xsl:value-of select="@nr"/>
</xsl:for-each>

namespace AVRCpp
{		
	namespace ExternalInterrupt
	{
<xsl:for-each select="$feature/interrupt">	
		struct Interrupt<xsl:value-of select="@nr"/> : Internal::InterruptBase &lt;
			InputPin<xsl:value-of select="pin/@nr"/>&lt;Port<xsl:value-of select="pin/@port"/>&gt;,				/* InputPin */
			<xsl:choose>
	<xsl:when test="@detecton='sync'">Event</xsl:when>
	<xsl:when test="@detecton='async'">AsyncEvent</xsl:when>
	<xsl:when test="@detecton='simple_async'">SimpleAsync</xsl:when>	
</xsl:choose>,						/* EventEnum */
			Bits&lt;_EICR<xsl:value-of select="@register"/>, _ISC<xsl:value-of select="@nr"/>0 | _ISC<xsl:value-of select="@nr"/>1&gt;,	/* EventBits */
			Bits&lt;_EIMSK, _INT<xsl:value-of select="@nr"/>&gt;,			/* InterruptEnableBit */
			Bits&lt;_EIFR, _INTF<xsl:value-of select="@nr"/>&gt;,			/* InterruptFlagBit */
			<xsl:value-of select="@shift"/> &gt;								/* eventShift */
			
		{ __INTERRUPT_HANDLER_SUPPORT__ }; // Interrupt<xsl:value-of select="@nr"/>	

</xsl:for-each>

<xsl:for-each select="$feature/pin_change_interrupt">
		struct PinChangeInterrupt<xsl:value-of select="@nr"/> : Internal::PinChangeInterruptBase &lt; 
			_PCMSK<xsl:value-of select="@register"/>,							/* MaskRegister */
			Bits&lt;_PCICR, _PCIE<xsl:value-of select="@nr"/>&gt;,				/* InterruptEnableBit */ 
			Bits&lt;_PCIFR, _PCIF<xsl:value-of select="@nr"/>&gt; &gt;				/* InterruptFlagBit */

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
