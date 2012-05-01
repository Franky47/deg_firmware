/*!
 *  @file       EnvelopeGenerator.h
 *  Project     DEG
 *  @brief      Envelope generation algorithm
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

#ifndef _FSE_ENVELOPE_GENERATOR_H_
#define _FSE_ENVELOPE_GENERATOR_H_

#include "Clock.h"
#include "ConfFile.h"


/*! \class EnvelopeGenerator 
 \brief 
 */
class EnvelopeGenerator : public Clock
{
    
public:
    
#if COMPFLAG_SHAPING
    
    enum eShape 
    {
        Invalid = 0,
        Linear,
        Exponential,
        Logarithmic,
        eShape_count
    };
    
#endif
    
    /*==========================================================================
     Construction and Destruction
     ==========================================================================*/
    
    EnvelopeGenerator();
    
    
    /*==========================================================================
     Control
     ==========================================================================*/
    
    void gateOn();
    
    void gateOff();
    
    void trigger();
    
    
    virtual void tick();
    
    
    /*==========================================================================
     Settings
     ==========================================================================*/
    
    void setAttack(uint32_t inValue);
    
    void setDecay(uint32_t inValue);
    
    void setSustain(uint16_t inValue);
    
    void setRelease(uint32_t inValue);
    
    
#if COMPFLAG_POLARITY
    
    void setPolarity(bool inPositive)
    {
        mPositivePolarity = inPositive;
    }
    
    void togglePolarity()
    {
        mPositivePolarity = !mPositivePolarity;
    }
    
    
    bool getPolarity() const
    {
        return mPositivePolarity;
    }
    
#endif // COMPFLAG_POLARITY
    
    
#if COMPFLAG_SHAPING
    
    void setShape(eShape inShape)
    {
        mShape = inShape;
    }
    
#endif // COMPFLAG_SHAPING
    
    
    /*==========================================================================
     Algorithm
     ==========================================================================*/
    
    void doProcess();
    
    uint16_t getOutputLevel() const
    {
        return mEnvelopeLevel;
    }
    
    
private:
    
    enum eState 
    {
        Idle,
        Attack,
        Decay,
        Sustain,
        Release
    };
    
    eState                          mCurrentState;
    
    bool                            mGateState;
    
    uint32_t                        mAttackTime;
    uint32_t                        mDecayTime;
    uint16_t                        mSustainLevel;
    uint32_t                        mReleaseTime;
    
#if COMPFLAG_POLARITY
    
    bool                            mPositivePolarity;
    
#endif
    
    
#if COMPFLAG_SHAPING
    
    eShape                          mShape;
    
#endif
    
    uint16_t                        mEnvelopeLevel;
    
};

#endif //_FSE_ENVELOPE_GENERATOR_H_
