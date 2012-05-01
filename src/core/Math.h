/*!
 *  @file       Math.h
 *  Project     DEG
 *  @brief      Simple math tools
 *  @author     Francois Best
 *  @date       30/04/12
 *
 *  This file is mostly based on Processing/Arduino API.
 *  Therefore, all credit goes to the original owners.
 *
 */

#ifndef _AVR_CORE_MATH_H_
#define _AVR_CORE_MATH_H_

#include "Types.h"

#define USE_MACRO               0

#if USE_MACRO

#define map(x,in_min,in_max,out_min,out_max) \
((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min)

#else

inline uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif

#endif // _AVR_CORE_MATH_H_
