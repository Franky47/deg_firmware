/*!
 *  @file       EnvelopeGenerator.cpp
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

#include "EnvelopeGenerator.h"
#include "fassert.h"
#include "Clock.h"
#include "Math.h"

// Values in microseconds
#define min_attack          100
#define min_decay           100
#define min_release         100

#define max_attack          10000000
#define max_decay           10000000
#define max_release         10000000

#define min_sustain         0
#define max_sustain         16383



/*==============================================================================
 Construction and Destruction
 ==============================================================================*/

/*! \brief Default constructor. */
EnvelopeGenerator::EnvelopeGenerator() 
    : Clock()
    , mCurrentState(Idle)
    , mAttackTime(min_attack)
    , mDecayTime(min_decay)
    , mSustainLevel(max_sustain)
    , mReleaseTime(min_release)
    , mPositivePolarity(true)
    , mShape(Linear)
    , mEnvelopeLevel(0)
{
    
}


/*==============================================================================
 Control
 ==============================================================================*/

void EnvelopeGenerator::gateOn()
{
    
    mGateState = true;
    
    trigger();
    
}


void EnvelopeGenerator::gateOff()
{
    
    if (mCurrentState != Attack) {
      
        mCurrentState = Release;
        
        reset();
        
    }
    // else:
    // If Gate goes off when still in Attack:
    // - Keep Attack going up to the end
    // - Go to release state after that.
    
    mGateState = false;
    
}


void EnvelopeGenerator::trigger()
{
    
    mCurrentState = Attack;
    
    reset();
    
}


void EnvelopeGenerator::tick()
{
    
    if (mIsRunning) {
    
        mMicrosecondsCounter += mSampleTime;
        
    }
    
#if COMPFLAG_SYNCHRONOUS
    
    doProcess();
    
#endif
    
}


/*==============================================================================
 Settings
 ==============================================================================*/

void EnvelopeGenerator::setAttack(uint32_t inValue)
{
    
    mAttackTime = map(inValue,
                      0x00000000,
                      0x001FFFFF,
                      (uint32_t)min_attack,
                      (uint32_t)max_attack);
    
}


void EnvelopeGenerator::setDecay(uint32_t inValue)
{
    
    mDecayTime = map(inValue,
                     0x00000000,
                     0x001FFFFF,
                     (uint32_t)min_decay,
                     (uint32_t)max_decay);
    
}


void EnvelopeGenerator::setSustain(uint16_t inValue)
{
    
    mSustainLevel = inValue;
    
}


void EnvelopeGenerator::setRelease(uint32_t inValue)
{
    
    mReleaseTime = map(inValue,
                       0x00000000,
                       0x001FFFFF,
                       (uint32_t)min_release,
                       (uint32_t)max_release);
    
}


/*==============================================================================
 Algorithm
 ==============================================================================*/

void EnvelopeGenerator::doProcess()
{
  
    if (mCurrentState == Idle ||
        mCurrentState == Sustain) 
    {
        
        // Nothing to do.
        return;
        
    }
    
    uint32_t current_time = 0;
    const uint32_t elapsed_time = getElapsedTime(&current_time);
    
    uint32_t base_time    = 0;
    uint16_t target_level = 0;
    
    if (mCurrentState == Attack) {
        
        base_time    = mAttackTime;
        target_level = max_sustain;
        
    }
    else if (mCurrentState == Decay) {
        
        base_time    = mDecayTime;
        target_level = mSustainLevel;
        
    }
    else if (mCurrentState == Release) {
        
        base_time    = mReleaseTime;
        target_level = 0;
        
    }
    else {
        
        // Error
        fassertfalse;
        
    }
    
    if (current_time >= base_time) {
        
        // End of current phase
        
        mEnvelopeLevel = target_level;
        
        switch (mCurrentState) 
        {
            case Attack:
                mCurrentState = mGateState ? Decay : Release;
                reset();
                break;
                
            case Decay:
                mCurrentState = mGateState ? Sustain : Release;
                reset();
                break;
                
            case Release:
                mCurrentState = Idle;
                stop();
                break;
                
            default:
                fassertfalse;
                break;
        }
        
        return;
        
    }
    
    if (mShape == Linear) {
        
        const int32_t denominator = base_time - current_time + elapsed_time;
        const int32_t numerator   = elapsed_time * (target_level - mEnvelopeLevel);
        
        mEnvelopeLevel += (numerator / denominator);
        
    }
    else if (mShape == Exponential) {
        
        // \todo Add wave generation
        
    }
    else {
        
        // \todo Add wave generation
        
    }
    
}

