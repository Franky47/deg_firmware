/*!
 *  @file       Gate.cpp
 *  Project     DEG
 *  @brief      Gate handling
 *  @author     Francois Best 
 *  @date       29/04/12
 *  @license    CC-BY-NC-SA Forty Seven Effects - 2012
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


#include "Gate.h"
#include "PinMapping.h"
#include "Globs.h"

/*==============================================================================
 Initialisation
 ==============================================================================*/

void Gate::configure()
{
    
    GATE_DDR &= ~(1 << GATE_PIN);       // Set as input
    
#if defined (__AVR_ATmega644P__)
    
    EICRA |= (0<<ISC01) | (1<<ISC00);   // Trigger on both edges of Gate
    
#elif defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)
    
    MCUCR |= (0<<ISC01) | (1<<ISC00);   // Trigger on both edges of Gate
    
#else
#error Unsupported target MCU
#endif
    
    enable();
    
}


/*==============================================================================
 Activation / Deactivation
 ==============================================================================*/

void Gate::enable()
{
    
#if defined (__AVR_ATmega644P__)
    
    EIMSK |= (1 << INT0);
    
#elif defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)
    
    GIMSK |= (1 << INT0);
    
#else
#error Unsupported target MCU
#endif
    
}


void Gate::disable()
{
    
#if defined (__AVR_ATmega644P__)
    
    EIMSK &= ~(1 << INT0);
    
#elif defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)
    
    GIMSK &= ~(1 << INT0);
    
#else
#error Unsupported target MCU
#endif
    
}


void Gate::toggle()
{
    
#if defined (__AVR_ATmega644P__)
    
    EIMSK ^= (1 << INT0);
    
#elif defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)
    
    GIMSK ^= (1 << INT0);
    
#else
#error Unsupported target MCU
#endif
    
}


/*==============================================================================
 Interrupt callbacks
 ==============================================================================*/

void Gate::handleGateOn()
{
    
#if COMPFLAG_ENV_A
    
    gEnvA.gateOn();
    
#endif
    
#if COMPFLAG_ENV_B
    
    gEnvB.gateOn();
    
#endif
    
}


void Gate::handleGateOff()
{
    
#if COMPFLAG_ENV_A
    
    gEnvA.gateOff();
    
#endif
    
#if COMPFLAG_ENV_B
    
    gEnvB.gateOff();
    
#endif
    
}


void Gate::handleTrigger()
{
    
#if COMPFLAG_TRIGGER
    
#if COMPFLAG_ENV_A
    
    gEnvA.trigger();
    
#endif // COMPFLAG_ENV_A
    
#if COMPFLAG_ENV_B
    
    gEnvB.trigger();
    
#endif // COMPFLAG_ENV_B
    
#endif // COMPFLAG_TRIGGER
    
}
