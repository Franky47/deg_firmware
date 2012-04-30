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
    
    if (mCurrentState == Attack) {
        
        processAttack();
        
    }
    else if (mCurrentState == Decay) {
        
        processDecay();
        
    }
    else if (mCurrentState == Release) {
        
        processRelease();
        
    }
    else {
        
        // Error
        fassertfalse;
        
    }
    
}


void EnvelopeGenerator::processAttack()
{
    
    const uint32_t remaining_time = mAttackTime - getTime();
    
    if (mShape == Linear) {
        
        
        
    }
    else if (mShape == Exponential) {
        
        
        
    }
    else {
        
        // \todo: Logarithmic response
        
    }
    
    if (remaining_time - mSampleTime <= 0) {
        
        // End of attack phase
        
        if (mGateState == true) {
            
            mCurrentState = Decay;
            
        }
        else {
            
            // Gate removed before end of attack
            // Go to release state directly.
            mCurrentState = Release;
            
        }
        
        reset();
        
    }
    
}


void EnvelopeGenerator::processDecay()
{
    
    const uint32_t remaining_time = mDecayTime - getTime();
    
    if (mShape == Linear) {
        
        
        
    }
    else if (mShape == Exponential) {
        
        
        
    }
    else {
        
        // \todo: Logarithmic response
        
    }
    
    if (remaining_time - mSampleTime <= 0) {
        
        // End of decay phase
        
        if (mGateState == true) {
            
            mCurrentState = Sustain;
            
            stop();
            
        }
        else {
            
            // Gate removed before end of decay
            // Go to release state directly.
            mCurrentState = Release;
            
            reset();
            
        }
        
    }
    
}


void EnvelopeGenerator::processRelease()
{
    
    const uint32_t remaining_time = mReleaseTime - getTime();
    
    if (mShape == Linear) {
        
        
        
    }
    else if (mShape == Exponential) {
        
        
        
    }
    else {
        
        // \todo: Logarithmic response
        
    }
    
    if (remaining_time - mSampleTime <= 0) {
        
        // End of release phase
        
        mCurrentState = Idle;
        
        stop();
        
    }
    
}

