/*!
 *  @file       Globs.cpp
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



#include "Globs.h"
#include "fassert.h"
#include "Types.h"

#if COMPFLAG_ENV_A

EnvelopeGenerator gEnvA;

#endif

#if COMPFLAG_ENV_B

EnvelopeGenerator gEnvB;

#endif 


/*! \brief These are C++ virtual handlers, not defined in avr-libc.
 
 Basic implementation will do the job.
 */
extern "C" {
    
    int __cxa_guard_acquire(__guard *g)
    {
        
        return !*(char *)(g);
    
    }; 
    
    
    void __cxa_guard_release(__guard *g) 
    {
        
        *(char *)g = 1;
    
    }; 
    
    
    void __cxa_guard_abort (__guard *)
    {
    
    }; 
    
    
    void __cxa_pure_virtual() 
    { 
        
        fassertfalse;
        while (1);
        
    }
    
}

