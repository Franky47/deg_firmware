/*!
 *  @file		IRQ.cpp
 *  Project		DEG
 *	@brief		Interrupt handling routines
 *  @author		Francois Best 
 *	@date		29/04/12
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


#include "IRQ.h"
#include "Globs.h"
#include "PinMapping.h"
#include "Gate.h"
#include "Output.h"
#include "Clock.h"

ISR(IRQ_A) 
{
    
    Output::handleEnvA();
    
}


ISR(IRQ_B)
{
	
    Output::handleEnvB();
    
}


/*! \brief Interrupt handler for the 16 bit counter Overflow point.
 
 This marks the end of a PWM period, and also serves as a clock tick for
 counting the time inside the main algorithm.
 */
ISR(IRQ_OVF)
{
    
    gEnvA.tick();
    
    gEnvB.tick();
    
    Output::handleOverflow();
    
}


/*! \brief Interrupt handler for Gate input.
 
 This is called when the state of the Gate pin changed.
 */
ISR(IRQ_GATE)
{
	
	if (GATE_READ & (1 << GATE_PIN)) {
      
        // High level
        
#if GATE_POLARITY
        
        Gate::handleGateOn();
        
#else
      
        Gate::handleGateOff();
        
#endif
        
    }
    else {
        
        // Low level

#if GATE_POLARITY
        
        Gate::handleGateOff();
        
#else
        
        Gate::handleGateOn();
        
#endif
        
    }
    
}

