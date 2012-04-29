/*!
 *  @file		Gate.h
 *  Project		DEG
 *	@brief		Gate handling
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


#ifndef GATE_H_
#define GATE_H_

namespace Gate 
{
    
    /*==========================================================================
     Initialisation
     ==========================================================================*/
    
    void configure();
    
    
    /*==========================================================================
     Activation / Deactivation
     ==========================================================================*/
    
    void enable();
    
    void disable();
    
    void toggle();
    
    
    /*==========================================================================
     Interrupt callbacks
     ==========================================================================*/
    
    void handleGateOn();
    
    void handleGateOff();
    
}


#endif // GATE_H_
