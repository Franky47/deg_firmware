/*!
 *  @file		Types.h
 *  Project		DEG
 *	@brief		Type definitions
 *  @author		Francois Best
 *	@date		15/01/10
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

#ifndef _TYPES_H_
#define _TYPES_H_

#include <inttypes.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef uint8_t byte;
//typedef int8_t sbyte;	
//typedef uint16_t word;
	
typedef void (*voidFuncPtr)(void);
typedef void (*intFuncPtr)(int);
	
	
#ifdef __cplusplus
} // extern "C"
#endif
	
	
#endif //_TYPES_H_

