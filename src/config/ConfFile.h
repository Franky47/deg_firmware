/*!
 *  @file		ConfFile.h
 *  Project		DEG
 *	@brief		Global definitions
 *  @author		Francois Best
 *	@date		11/04/2011
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


#ifndef CONFFILE_H_
#define CONFFILE_H_


#define COMPFLAG_ENV_A          1   // Enable envelope A.
#define COMPFLAG_ENV_B          1   // Enable envelope B.
#define COMPFLAG_TRIGGER        1   // Add setting to allow rettriggering.
#define COMPFLAG_SHAPING        1   // Add setting to generate non-linear curves (eg: exponential).
#define COMPFLAG_SLOWFAST       1   // Add setting to divide speed by 10 for A,D,R.
#define COMPFLAG_POLARITY       1   // Add setting to generate inverted envelope.
#define	COMPFLAG_DEBUG			0	// Serial line for debug (available only on 644P).


#define SPI_BUFFER_SIZE			16
#define SPI_MAX_MSG_SIZE		3
#define SPI_USE_BUFFER          0


#if COMPFLAG_DEBUG

#define DEBUG 1

#if defined (__AVR_ATmega644P__)
#define DEBUG_SERIAL 1
#elif defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)
#define DEBUG_SERIAL 0
#else
#error Unsupported target MCU
#endif

#endif // COMPFLAG_DEBUG


// Checks
#ifndef F_CPU
#error Please define F_CPU to 16000000 (16MHz clock).
#else
#if (F_CPU != 16000000)
#error This code has been designed for running on a 16MHz clock.
#endif
#endif


#endif // CONFFILE_H_

