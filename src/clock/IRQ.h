/*!
 *  @file		IRQ.h
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


#ifndef IRQ_H_
#define IRQ_H_

#include <avr/interrupt.h>


/*==============================================================================
 Interrupt vectors definition
 ==============================================================================*/

#if defined (__AVR_ATmega644P__)

	#define IRQ_A           TIMER1_COMPA_vect
	#define IRQ_B           TIMER1_COMPB_vect
    #define IRQ_OVF         TIMER1_OVF_vect


#elif defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)

	#define IRQ_A           TIM1_COMPA_vect
	#define IRQ_B           TIM1_COMPB_vect
    #define IRQ_OVF         TIM1_OVF_vect

#else
#error Unsupported target MCU
#endif


#ifdef INT0_vect

	#define IRQ_GATE        INT0_vect		// Universal vector for both chips

#endif


/*==============================================================================
 Macros validity checks
 ==============================================================================*/

#ifndef IRQ_A
#error	No valid ISR found for A
#endif

#ifndef IRQ_B
#error	No valid ISR found for B
#endif

#ifndef IRQ_OVF
#error No valid ISR found for Overflow
#endif

#ifndef IRQ_GATE
#error No valid ISR found for sync pulse handling
#endif


#endif // IRQ_H_
