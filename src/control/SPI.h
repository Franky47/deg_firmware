/*!
 *  @file       SPI.h
 *  Project     DEG
 *  @brief      SPI Driver for DEG
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

#ifndef _FSE_DEVICE_SPI_H_
#define _FSE_DEVICE_SPI_H_

#include "Types.h"
#include "ConfFile.h"

#if COMPFLAG_SPI

namespace SPI {

    /*==========================================================================
     Message types
     ==========================================================================*/
    
    /*! \brief SPI messages
     
     SPI Messages have the following structure:
     
     [status (1 byte)] [data (N bytes)]
     
     Status bytes have their MSB set to 1, data bytes have it set to 0.
     
     Bit 7 in status byte determines which envelope is being controlled:
     
     - if bit 7 is cleared (0): The message targets envelope A.
     - if bit 7 is set     (1): The message targets envelope B.
     
     The remainding bits (6 to 0) are used to encode the message type, using
     this enumeration.
     
     status byte = 0x80 | (env_B << 7) | enum
     
     */
    enum eSPIMessage 
    {
        Invalid = -1,
        
        trigger,
        gateOn,
        gateOff,
        
        // Parameters
        setAttack,          // 14 bit data value (range-independant)
        setDecay,           // 14 bit data value (range-independant)
        setSustain,         // 14 bit data value
        setRelease,         // 14 bit data value (range-independant)
        
        // Settings
        setFastMode,        // Range: 100us to 1s
        setSlowMode,        // Range: 1ms to 10s
        toggleFastSlow,
        
        setShape,           // Exponential or Linear
        
        setPolarityPositive,
        setPolarityNegative,
        togglePolarity,
        
        eSPIMessage_count
    };
    
    
    /*==========================================================================
     Initialisation
     ==========================================================================*/
    
    void configure();

    
    /*==========================================================================
     Message reception
     ==========================================================================*/
    
    void read();    // Poll SPI buffer for incoming data

    void handleMessage();

    byte getNumDataBytes(byte inMessageType);
    
    
    // Members
#if SPI_USE_BUFFER

    void parse_buffer();

    // Private
    byte extract_byte_from_buffer();
    void add_byte_to_buffer(byte inByte);
    byte available();

    extern byte buffer[SPI_BUFFER_SIZE];
    extern byte * read_ptr;
    extern byte * write_ptr;


#endif

    void parse(const byte inByte);


    /*==========================================================================
     SPI Message data structure
     ==========================================================================*/
    
    namespace Message {
        
        extern byte data[SPI_MAX_MSG_SIZE];
        extern byte index;
        extern byte length;
        extern byte running_status;
        
    };
    
    
}; // namespace SPI

#endif // COMPFLAG_SPI

#endif // _FSE_DEVICE_SPI_H_
