/*!
 *  @file		Runtime.cpp
 *  Project		DEG
 *	@brief		Runtime functions
 *  @author		Francois Best
 *	@date		02/06/2010
 *  @license	CC-BY-NC-SA Forty Seven Effects - 2012
 *
 * THE WORK (AS DEFINED BELOW) IS PROVIDED UNDER THE TERMS 
 * OF THIS CREATIVE COMMONS PUBLIC LICENSE ("CCPL" OR "LICENSE").
 * THE WORK IS PROTECTED BY COPYRIGHT AND/OR OTHER APPLICABLE LAW. 
 * ANY USE OF THE WORK OTHER THAN AS AUTHORIZED UNDER THIS LICENSE 
 * OR COPYRIGHT LAW IS PROHIBITED.
 * 
 * BY EXERCISING ANY RIGHTS TO THE WORK PROVIDED HERE, YOU ACCEPT 
 * AND AGREE TO BE BOUND BY THE TERMS OF THIS LICENSE. 
 * TO THE EXTENT THIS LICENSE MAY BE CONSIDERED TO BE A CONTRACT, 
 * THE LICENSOR GRANTS YOU THE RIGHTS CONTAINED HERE IN CONSIDERATION 
 * OF YOUR ACCEPTANCE OF SUCH TERMS AND CONDITIONS.
 * http://creativecommons.org/licenses/by-nc-sa/3.0/
 */


#include "Runtime.h"
#include "Globs.h"
#include <avr/pgmspace.h>
#include "fassert.h"

#if COMPFLAG_DEBUG
#include "Serial.h"
#endif

#include "Gate.h"
#include "Output.h"
#include "Clock.h"

#if COMPFLAG_SPI
#include "SPI.h"
#endif




#if DEBUG_SERIAL

void printLong(const uint32_t inLong)
{
	
	Serial.printNumber((byte)(inLong>>24),16); 
	Serial << " ";
	Serial.printNumber((byte)(inLong>>16),16); 
	Serial << " ";
	Serial.printNumber((byte)(inLong>>8),16); 
	Serial << " ";
	Serial.printNumber((byte)inLong,16);  
}

#endif // DEBUG_SERIAL


void setup()
{
	
#if defined(__AVR_ATtiny84__)
	
	// Set the CPU clock divider to 1 (full speed)
	CLKPR = 0x80;
	CLKPR = 0x00;
	
	// This is in case the fuses have not been set up correctly.
	// Set them with the following command:
	// avrdude -U lfuse:w:0xff:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
	// See this useful tool: http://www.engbedded.com/fusecalc/
	
#endif
	
	
	// ------------------------------------------------------------------
	// PWM Output module
	{
      
        Output::configure();
        
    }
	
    
	// ------------------------------------------------------------------
	// Gate pin
	{
        
        Gate::configure();
	   
    }
    
    
	// ------------------------------------------------------------------
	// SPI Communications
	{
		
#if COMPFLAG_SPI
		
		SPI::configure();
		
#endif
		
	}
	
    
    
    // ------------------------------------------------------------------
	// Clock
	{
        
        Clock::configure();
        
    }
    
	
	// ------------------------------------------------------------------
	// Initialise globs
	
    
    
	
	// Launch main timer
	TCCR1B |= 0x01;
	
#if DEBUG_SERIAL
	Serial.begin(38400);
	Serial << "MCO Ready" << endl; 
#endif
	
}


void loop()
{
	
	// ------------------------------------------------------------------
	// Read SPI input
	{	
		
#if COMPFLAG_SPI 
		
		SPI::read();
		
#endif // COMPFLAG_SPI
		
	}
	
    
    // ------------------------------------------------------------------
	// Asynchronous processing
	{
        
#if !(COMPFLAG_SYNCHRONOUS)
        
        gEnvA.doProcess();
        
        gEnvB.doProcess();
        
#endif
        
	}
    
}

