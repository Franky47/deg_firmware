/*!
 *  @file       fassert.h
 *  Project     DEG
 *  @brief      Assertion macros for embedded systems
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


#ifndef _FSE_ASSERT_H_
#define _FSE_ASSERT_H_

#include "ConfFile.h"

#if DEBUG

#if DEBUG_SERIAL

#include "Serial.h"

#define log_assertion \
{ \
	Serial.print("Assertion failure in "); \
	Serial.print(__FILE__); \
	Serial.print(", line "); \
	Serial.print(__LINE__); \
	Serial.print("\n"); \
}

#else

#include "PinMapping.h"

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define tuned_delay(delay) \
{ \
uint8_t tmp=0; \
\
asm volatile("sbiw    %0, 0x01 \n\t" \
			 "ldi %1, 0xFF \n\t" \
			 "cpi %A0, 0xFF \n\t" \
			 "cpc %B0, %1 \n\t" \
			 "brne .-10 \n\t" \
			 : "+r" (delay), "+a" (tmp) \
			 : "0" (delay) \
			 ); \
}

#define DEBUG_CHAR(c) \
{ \
	byte old_sreg = SREG; \
	cli(); \
	uint16_t delay = 54; \
	for (byte mask=0x01;mask;mask<<=1) { \
		\
		if (c & mask) { \
			DEBUG_PORT |=  (1<<DEBUG_PIN); \
		} \
		else { \
			DEBUG_PORT &= ~(1<<DEBUG_PIN); \
		} \
		tuned_delay(delay)\
	} \
	DEBUG_PORT |= (1<<DEBUG_PIN); \
	\
	SREG = old_sreg;\
}

#define log_assertion \
{ \
	cli();\
	const char line[6] = TOSTRING(__LINE__);\
	for (byte i=0;i<6;i++) { \
		\
		if (line[i] == 0) break; /*End of data*/ \
		\
		DEBUG_CHAR(line[i]);\
	} \
}

#endif // DEBUG_SERIAL


#define fassertfalse { log_assertion; for(;;) { } }
#define fassert(cond) { if (!(cond)) { fassertfalse; } }

#else // !DEBUG

#define log_assertion
#define fassertfalse
#define fassert(cond) { if (!(cond)) { fassertfalse; } }

#endif // DEBUG

#endif // _FSE_ASSERT_H_
