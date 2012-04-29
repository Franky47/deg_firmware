/*!
 *  @file       Globs.h
 *  Project     DEG
 *  @brief      Global variables declarations
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


#ifndef GLOBS_H_
#define GLOBS_H_

#include <avr/pgmspace.h>
#include "Types.h"
#include "ConfFile.h"
#include "EnvelopeGenerator.h"

extern EnvelopeGenerator gEnvA;
extern EnvelopeGenerator gEnvB;




// Lookup tables
extern const byte gPrescaleShift[5];					// Lookup table for prescale shifting (1,8,64,256,1024)
extern const uint32_t gClockDivision[12];				// Clock division values for semitones
extern const uint16_t gCentInterpolTable[99];			// Valeurs de multiplication pour le calcul de la division d'horloge dynamique

// Paramètres du timer
extern volatile bool gRefreshFrequency;					// Toggle this to true to update counter on next rising edge of output clock
extern volatile uint16_t gNewCompare;						// With this new compare value
extern volatile byte gNewPrescale;						// And this new prescale value

#endif // GLOBS_H_
