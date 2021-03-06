/*!
 *  @file       PinMapping.h
 *  Project     DEG
 *  @brief      Pin Mapping
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


#ifndef _PIN_MAPPING_H_
#define _PIN_MAPPING_H_

#include <avr/io.h>

#if defined (__AVR_ATmega644P__)

#define ENV_A_PORT      
#define ENV_A_READ      
#define ENV_A_DDR       
#define ENV_A_PIN       

#define ENV_B_PORT      
#define ENV_B_READ      
#define ENV_B_DDR       
#define ENV_B_PIN       

#define GATE_PORT
#define GATE_READ       
#define GATE_DDR
#define GATE_PIN


#elif defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)

/*
 1  VCC
 2  XTAL1
 3  XTAL2
 4  Reset
 5  Gate        (PB2)   -> INT0
 6  Available   (PA7)   -> OC0B
 7  SPI MOSI
 8  SPI MISO
 9  SPI SCK
 10 SPI SS      (PA3)
 11 Debug       (PA2)
 12 Env B       (PA1)
 13 Env A       (PA0)
 14 GND
 */

#define ENV_A_PORT      PORTA
#define ENV_A_READ      PINA
#define ENV_A_DDR       DDRA
#define ENV_A_PIN       DDA0

#define ENV_B_PORT      PORTA
#define ENV_B_READ      PINA
#define ENV_B_DDR       DDRA
#define ENV_B_PIN       DDA1

#define GATE_PORT       PORTB
#define GATE_READ       PINB
#define GATE_DDR        DDRB
#define GATE_PIN        DDB2

#define DEBUG_PORT      PORTA
#define DEBUG_DDR       DDRA
#define DEBUG_PIN       DDA2

#else

#error Unsupported target MCU

#endif

#endif // _PIN_MAPPING_H_
