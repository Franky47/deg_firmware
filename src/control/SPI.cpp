/*!
 *  @file       SPI.cpp
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

#include "SPI.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Globs.h"
#include "fassert.h"
#include "IRQ.h"
#include "Gate.h"

#if COMPFLAG_SPI


/*
 SPI Driver Service Manual:
 
 Lorsqu'un byte est reçu dans le registre SPI, l'IRQ correspondant se lance pour le sauver dans le buffer.
 Le buffer est pollé par read(), qui lance le parseur pour tenter de récupérer des messages.
 Lorsque le parseur détecte un message, il lance directement l'action associée (pas de retour au Runtime pour redirection).
 
 */

#if DEBUG_SERIAL
#include "Serial.h"
#endif



#if defined (__AVR_ATmega644P__)

#define SPI_PORT_DDR	DDRB
#define SPI_SS			DDB4
#define SPI_MOSI		DDB5
#define SPI_MISO		DDB6
#define SPI_SCK			DDB7

#define SPI_DATA_VECT	SPI_STC_vect
#define SPI_DATA_REG	SPDR

#elif defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)

#define SPI_PORT_DDR	DDRA
#define SPI_SS			DDA3
#define SPI_MOSI		DDA6
#define SPI_MISO		DDA5
#define SPI_SCK			DDA4

#define SPI_DATA_VECT	USI_OVF_vect
#define SPI_DATA_REG	USIBR	// Buffered register, use USIDR for unbuffered read.

#define SPI_SS_VECT		PCINT0_vect
#define SPI_SS_READ		PINA

#else
#error Unsupported target MCU
#endif



// Variables

#if SPI_USE_BUFFER

byte SPI::buffer[SPI_BUFFER_SIZE] = { 0x00 };
byte* SPI::write_ptr = buffer;
byte* SPI::read_ptr  = buffer;

#endif

byte SPI::Message::data[SPI_MAX_MSG_SIZE] = { 0x00 };
byte SPI::Message::index  = 0;
byte SPI::Message::length = 0;
byte SPI::Message::running_status = SPI::Invalid;



void SPI::configure()
{
    
    // Initialise memory
    
#if SPI_USE_BUFFER
    
    for (byte i=0;i<SPI_BUFFER_SIZE;i++) {
        
        buffer[i] = 0x00;
        
    }
    
    read_ptr  = buffer;
    write_ptr = buffer;
    
#endif
    
    
    // ----------------------------------------------
    // Initialise Message
    
    for (byte i=0;i<SPI_MAX_MSG_SIZE;i++) {
        
        Message::data[i] = 0x00;
        
    }
    
    Message::index  = 0;
    Message::length = 0;
    Message::running_status = Invalid;
    
    
    
    // ----------------------------------------------
    // Hardware configuration
    
    SPI_PORT_DDR &= ~(1 << SPI_SCK);	// SCK:		Input
    SPI_PORT_DDR &= ~(1 << SPI_MOSI);	// MOSI:	Input
    SPI_PORT_DDR |=  (1 << SPI_MISO);	// MISO:	Output
    SPI_PORT_DDR &= ~(1 << SPI_SS);		// SS:		Input
    
#if defined (__AVR_ATmega644P__)
    
    // Enable SPI + Interrupt + MSB First + Slave operation + Mode 0 (Polarity 0 Phase 0) 
    SPCR = (1<<SPE) | (1<<SPIE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA);
    
#elif defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)
	
    // Enable 3-wire mode + Interrupt + Clock external, positive edge
    USICR = (1 << USIOIE) | (0 << USIWM1) | (1 << USIWM0) | (1 << USICS1) | (0 << USICS0);
    
    // We also need to setup SS as a pin change interrupt
    GIMSK  |= (1 << PCIE0);		// Enable Pin Change interrupt for port A (PCI 0)
    PCMSK0 |= (1 << SPI_SS);	// Enable Interrupt for pin SS
    
#else
#error Unsupported target MCU
#endif
    
}


/*! \brief Launch the parser on the incoming buffer.
 
 This method has an effect only if buffer parsing is
 activated (set SPI_USE_BUFFER to 1).
 Otherwise this does nothing, and the data is parsed
 directly byte by byte at reception.
 */
void SPI::read()
{
    
#if SPI_USE_BUFFER
    
    parse_buffer();
    
#endif	
    
}


#if SPI_USE_BUFFER

void SPI::parse_buffer()
{
    
    while (available() > 0) {
        
        const byte extracted = extract_byte_from_buffer();
        
        parse(extracted);
        
    }
    
}

// Private
byte SPI::available()
{
    if (read_ptr < write_ptr) {
        // Cas normal
        return write_ptr - read_ptr;
    }
    else if (read_ptr == write_ptr) {
        return 0;
    }
    else {
        // read_ptr > write_ptr
        return (SPI_BUFFER_SIZE-(read_ptr-write_ptr));
    }
}


byte SPI::extract_byte_from_buffer()
{
    
    if (read_ptr == write_ptr) return 0xFF;
    byte out = *read_ptr;
    read_ptr++;
    
    if (read_ptr >= buffer+SPI_BUFFER_SIZE) read_ptr = buffer;
    
    return out;
    
}


void SPI::add_byte_to_buffer(byte inByte)
{
    
    *(write_ptr) = inByte;
    write_ptr++;
    
    if (write_ptr >= buffer+SPI_BUFFER_SIZE) write_ptr = buffer;
    
    if (write_ptr == read_ptr) {
        // Overflow
        read_ptr++;
        if (read_ptr >= buffer+SPI_BUFFER_SIZE) read_ptr = buffer;
    }
    
}


#endif // SPI_USE_BUFFER

/*! \brief 1-byte parser.
 
 If buffer is enabled, this is called for each byte of
 the parser. Otherwise, it will be called directly upon
 reception of a byte in the SPI interrupt routine.
 */
void SPI::parse(const byte inByte)
{
    
    if (Message::index == 0) {
        
        // Start a new message.
        /*
         #if DEBUG_SERIAL
         Serial << "Starting message" << endl;
         Serial << "Parsing byte: ";
         Serial.printNumber(inByte,16);
         Serial << endl;
         #endif
         */
        if (inByte >= 0x80) {
            
            // Status byte, update running status.
            Message::data[0] = inByte;
            Message::running_status = inByte;
            Message::index = 1;
            
            // Update expected length
            
            Message::length = 1 + getNumDataBytes(inByte);
            
            if (Message::length == 1) {
              
                // One byte message, direct handling.
                handleMessage();
                
                Message::index = 0;
                
            }
            
            return;
            
        }
        else {
            
            // Data byte -> Running status.
            fassert(Message::running_status != Invalid);
            
            Message::data[0] = Message::running_status;
            Message::data[1] = inByte;
            Message::index   = 2;
            
        }
        
    }
    else {
        
        /*
         #if DEBUG_SERIAL
         Serial << "Parsing byte: ";
         Serial.printNumber(inByte,16);
         Serial << " (appending)" << endl;
         #endif
         */		
        // Continue recomposing the message.
        Message::data[Message::index++] = inByte;
        
    }
    
    if (Message::index == Message::length) {
        /*
         #if DEBUG_SERIAL
         Serial << "Message complete" << endl;
         for (int i=0;i<Message::length;++i) {
         
         Serial.printNumber(Message::data[i],16);
         
         }
         Serial << endl;
         #endif
         */		
        // Complete
        handleMessage();
        
        // Reset index
        Message::index = 0;
        
    }
    
}


void SPI::handleMessage()
{
    
#if COMPFLAG_ENV_A || COMPFLAG_ENV_B
    
    // \refactor #if block added to satisfy unused variable.
    // Remove it if using another modulation source.
    
    const bool message_dest_A = !(Message::data[0] & 0x40);
    
#endif 
    
    const byte message_type = Message::data[0] & 0x3F;
    
    switch (message_type) {
        
        case trigger:
            Gate::handleTrigger();
            break;
            
        case gateOn:
            Gate::handleGateOn();
            break;
            
        case gateOff:
            Gate::handleGateOff();
            break;

#if COMPFLAG_ENV_A || COMPFLAG_ENV_B
            
        case setAttack:
        {

            const uint16_t data = ((uint16_t)Message::data[1] << 7) | Message::data[2];
            
#if COMPFLAG_ENV_A
            
            if (message_dest_A)     gEnvA.setAttack(data);
            
#else
            if (0) { }
#endif
            
#if COMPFLAG_ENV_B
            
            else                    gEnvB.setAttack(data);
            
#endif
            
        }
            break;
            
        case setDecay:
        {
            const uint16_t data = ((uint16_t)Message::data[1] << 7) | Message::data[2];
            
#if COMPFLAG_ENV_A
            
            if (message_dest_A)     gEnvA.setDecay(data);
            
#else
            if (0) { }
#endif
            
#if COMPFLAG_ENV_B
            
            else                    gEnvB.setDecay(data);
            
#endif
        }
            break;
            
        case setSustain:
        {
            const uint16_t data = ((uint16_t)Message::data[1] << 7) | Message::data[2];
            
#if COMPFLAG_ENV_A
            
            if (message_dest_A)     gEnvA.setSustain(data);
            
#else
            if (0) { }
#endif
            
#if COMPFLAG_ENV_B
            
            else                    gEnvB.setSustain(data);
            
#endif
        }
            break;
            
        case setRelease:
        {
            const uint16_t data = ((uint16_t)Message::data[1] << 7) | Message::data[2];
            
#if COMPFLAG_ENV_A
            
            if (message_dest_A)     gEnvA.setRelease(data);
            
#else
            if (0) { }
#endif
            
#if COMPFLAG_ENV_B
            
            else                    gEnvB.setRelease(data);
            
#endif
        }
            break;
        
#endif // COMPFLAG_ENV_A || COMPFLAG_ENV_B
            
        case Invalid:
        default:
            // Parse error
            fassertfalse;
            break;
    }
    
#if DEBUG_SERIAL
    // Dump message
    Serial.printNumber(Message::data[1],10);
    Serial.print(" ");
    Serial.printNumber(Message::data[2],10);
    Serial << endl;
#endif
    
}


byte SPI::getNumDataBytes(byte inMessageType)
{
    
    switch (inMessageType) 
    {
        case setAttack:
        case setDecay:
        case setSustain:
        case setRelease:
            return 2;
            break;
        
        case trigger:
        case gateOn:
        case gateOff:
        case setFastMode:
        case setSlowMode:
        case toggleFastSlow:
        case setPolarityPositive:
        case setPolarityNegative:
        case togglePolarity:
            return 0;
            break;
    
        case setShape:
            return 1;
            break;
        
        case Invalid:
        default:
            fassertfalse;
            return 0;
            break;
    }
    
}


// ---------------------------------------------
// Interrupt routine definition (chip-specific)

ISR(SPI_DATA_VECT) 
{	
	/*
     #if DEBUG_SERIAL
     Serial << "Received byte 0x";
     Serial.printNumber(SPI_DATA_REG,16);
     Serial << endl;
     #endif
     */
	
#if SPI_USE_BUFFER
	
	SPI::add_byte_to_buffer(SPI_DATA_REG);
	
#else
	
	//sei(); // ?
	
	SPI::parse(SPI_DATA_REG);
	
#endif
	
}


#if defined (__AVR_ATtiny44__) || defined (__AVR_ATtiny84__)

ISR(SPI_SS_VECT)
{
	
	// The SS pin has been toggled.
	// Read its new state.
	
	const bool ss_state = SPI_SS_READ & (1 << SPI_SS);
	
	if (ss_state == true) {
#if DEBUG
		DEBUG_CHAR('1');
#endif		
		// High level: Chip unselected.
		// Put SPI to sleep.
		SPI_PORT_DDR &= ~(1 << SPI_MISO);	// MISO: Input (tri-state)
		
		USICR &= ~(1<<USIWM0);				// Disable SPI
	}
	else {
#if DEBUG
		DEBUG_CHAR('0');
#endif	
		// Low level: chip selected.
		SPI_PORT_DDR |=  (1 << SPI_MISO);	// MISO: Output
		
		USICR |= (1<<USIWM0);				// Enable SPI
		
	}
	
}

#endif // ATtiny target


#endif // COMPFLAG_SPI

