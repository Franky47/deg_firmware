/*!
 *  @file       Output.cpp
 *  Project     DEG
 *  @brief      PWM output module
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


#include "Output.h"
#include "PinMapping.h"
#include "ConfFile.h"
#include "Globs.h"

/*==============================================================================
 Initialisation
 ==============================================================================*/

void Output::configure()
{
    
#if COMPFLAG_ENV_A
    
    ENV_A_DDR |= (1 << ENV_A_PIN);          // Set pin as output
    
    TIMSK1 |= (1 << OCIE1A);                // Enable point A interrupt
    
    OCR1A = 0x0000;                         // Reset envelope value to zero
    
#endif
    
#if COMPFLAG_ENV_B
    
    ENV_B_DDR |= (1 << ENV_B_PIN);          // Set pin as output
    
    TIMSK1 |= (1 << OCIE1B);                // Enable point B interrupt
    
    OCR1B = 0x0000;                         // Reset envelope value to zero
    
#endif
    
    // Timer settings
    
    TCCR1A = 0x00;                          // Normal mode, no PWM, no CTC
    TCCR1B = 0x00;                          // (PWM is handled internally)
    
    TIMSK1 |= (1 << TOIE1);                 // Register overflow interrupt
    
}


/*==============================================================================
 Interrupt callbacks
 ==============================================================================*/

/*! \brief Interupt callback for PWM toggle point A.
 
 */
void Output::handleEnvA()
{
    
#if COMPFLAG_ENV_A
    
    ENV_A_READ |= (1 << ENV_A_PIN); // Writing logical 1 to PIN register toggles output.
    
#endif // COMPFLAG_ENV_A
    
}


/*! \brief Interupt callback for PWM toggle point B.
 
 */
void Output::handleEnvB()
{
    
#if COMPFLAG_ENV_B
    
    ENV_B_READ |= (1 << ENV_B_PIN); // Writing logical 1 to PIN register toggles output.
    
#endif // COMPFLAG_ENV_B
    
}


/*! \brief Interrupt callback for end of PWM wave. 
 
 */
void Output::handleOverflow()
{
    
#if COMPFLAG_ENV_A
    
    OCR1A = gEnvA.getOutputLevel();
    
#if COMPFLAG_POLARITY
    
    if (gEnvA.getPolarity() == true) {
        
        // In positive polarity, start with high level.
        ENV_A_PORT |= (1 << ENV_A_PIN);
        
    }
    else {
        
        // In negative polarity, start with low  level.
        ENV_A_PORT &= ~(1 << ENV_A_PIN);
        
    }
    
    
#else // Positive polarity only.
    
    ENV_A_PORT |= (1 << ENV_A_PIN);
    
#endif // COMPFLAG_POLARITY
    
#endif // COMPFLAG_ENV_A
    
    
#if COMPFLAG_ENV_B
    
    OCR1B = gEnvB.getOutputLevel();
    
#if COMPFLAG_POLARITY
    
    if (gEnvB.getPolarity() == true) {
        
        // In positive polarity, start with high level.
        ENV_B_PORT |= (1 << ENV_B_PIN);
        
    }
    else {
        
        // In negative polarity, start with low  level.
        ENV_B_PORT &= ~(1 << ENV_B_PIN);
        
    }
    
    
#else // Positive polarity only.
    
    ENV_B_PORT |= (1 << ENV_B_PIN);
    
#endif // COMPFLAG_POLARITY
    
#endif // COMPFLAG_ENV_B
    
}
