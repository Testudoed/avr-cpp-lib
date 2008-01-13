<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0" xmlns="http://www.w3.org/1999/xhtml">
<xsl:output method="text" encoding="utf-8"/>
<xsl:include href="General.xsl"/>

<xsl:template match="/controller">

<xsl:call-template name="header">
	<xsl:with-param name="feature" select="'Watchdog'"/>
	<xsl:with-param name="definition" select="'WATCHDOG'"/>
</xsl:call-template>

/**********************************************************************************************************************\

	<xsl:value-of select="@name"/> watchdog (WDT) declarations.
	This file is generated. Do not modify manually !!!

\**********************************************************************************************************************/

<xsl:variable name="feature" select="features/feature[@name='Watchdog']"/>
<xsl:if test="$feature">
<xsl:if test="$feature/@enhanced">
#define WDT_TC_ns		WDT
#define WDT_TC_struct	Watchdog::TimerInterrupt
</xsl:if>
namespace AVRCpp
{
	<xsl:choose><xsl:when test="$feature/@enhanced">
	struct Watchdog : Internal::EnhancedWatchdogBase &lt;
			_<xsl:value-of select="$feature/register[@name='mcu_status']/@register"/>,					/* MCUStatusRegister */
			_<xsl:value-of select="$feature/register[@name='control']/@register"/> &gt;				/* WatchdogTimerControlRegister */			 
	{			
		struct TimerInterrupt : BasicInterrupt&lt;Bits&lt;_<xsl:value-of select="$feature/interrupt[@name='timer']/@register"/>, _<xsl:value-of select="$feature/interrupt[@name='timer']/@bit"/>&gt; &gt; { __INTERRUPT_HANDLER_SUPPORT__ };
		
	}; // struct Watchdog
	</xsl:when>	
	<xsl:otherwise>
	struct Watchdog : Internal::WatchdogBase &lt;
			_<xsl:value-of select="$feature/register[@name='mcu_status']/@register"/>,					/* MCUStatusRegister */
			_<xsl:value-of select="$feature/register[@name='control']/@register"/> &gt;				/* WatchdogTimerControlRegister */			 
	{	
	
	}; // struct Watchdog
	</xsl:otherwise></xsl:choose>
} // namespace AVRCpp
</xsl:if>

<xsl:call-template name="footer">
	<xsl:with-param name="feature" select="'Watchdog'"/>
	<xsl:with-param name="definition" select="'WATCHDOG'"/>
</xsl:call-template>

</xsl:template>

</xsl:stylesheet>
