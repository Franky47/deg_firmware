/*!
 *  @file		SPI.cpp
 *  Project		DEG
 *	@brief		SPI Driver for DEG
 *  @author		Francois Best 
 *	@date		28/11/10
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

#include "SPI.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Globs.h"
#include "fassert.h"
#include "IRQ.h"

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



namespace SPI {
	
	// Variables
	
#if SPI_USE_BUFFER
	
	byte buffer[SPI_BUFFER_SIZE] = { 0x00 };
	byte * write_ptr = buffer;
	byte * read_ptr  = buffer;
	
#endif
	
	namespace Message {
		
		byte data[SPI_MAX_MSG_SIZE] = { 0x00 };
		byte index  = 0;
		byte length = 0;
		byte running_status = Invalid;
		
	};
	
	
	volatile byte StaticNote_coarse = 0;
	volatile byte StaticNote_fine   = 0;
	
	
	void SetConsigne(uint16_t inVal) {
		// Limite max de la consigne: 127 * 100 + 99
		if (inVal < 12800) {
			StaticNote_coarse = (inVal -inVal%100)/100;
			StaticNote_fine = inVal%100;
		}
	}
	void SetConsigne(byte inSemi,byte inCent) {
		if (inSemi < 128 && inCent < 100) {
			StaticNote_coarse = inSemi;
			StaticNote_fine = inCent;
		}
	}
	
	
	void configure()
    {
		
		/*
		 The following code examples show how to initialize the SPI as a Master and how to perform a simple transmission. 
		 DDR_SPI in the examples must be replaced by the actual Data Direction Register controlling the SPI pins. 
		 DD_MOSI, DD_MISO and DD_SCK must be replaced by the actual data direction bits for these pins. 
		 E.g. if MOSI is placed on pin PB5, replace DD_MOSI with DDB5 and DDR_SPI with DDRB.
		 */
		
		// Initialise memory
		
#if SPI_USE_BUFFER
		
		for (byte i=0;i<SPI_BUFFER_SIZE;i++) {
			
			buffer[i] = 0x00;
			
		}
		
		read_ptr = buffer;
		write_ptr = buffer;
		
#endif
		
		// ----------------------------------------------
		// Initialise Message
		
		for (byte i=0;i<SPI_MAX_MSG_SIZE;i++) {
			
			Message::data[i] = 0x00;
			
		}
		
		Message::index = 0;
		Message::length = 0;
		Message::running_status = FineNote;
		
		StaticNote_coarse = 60; // C4
		StaticNote_fine   = 0;
		
		
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
	void read()
	{
		
#if SPI_USE_BUFFER
		
		parse_buffer();
		
#endif	
		
	}
	
	
#if SPI_USE_BUFFER
	
	void parse_buffer()
	{
				
		while (available() > 0) {
			
			const byte extracted = extract_byte_from_buffer();
			
			parse(extracted);
			
		}
		
	}
	
	// Private
	byte available() {
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
	byte extract_byte_from_buffer() {
		
		if (read_ptr == write_ptr) return 0xFF;
		byte out = *read_ptr;
		read_ptr++;
		
		if (read_ptr >= buffer+SPI_BUFFER_SIZE) read_ptr = buffer;
		
		return out;
		
	}
	void add_byte_to_buffer(byte inByte) {
		
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
	void parse(const byte inByte)
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
			if (inByte >= CoarseNote) {
				
				/*fassert(inByte == CoarseNote   ||
						inByte == FineNote     ||
						inByte == GlobalDetune ||
						inByte == PortamentoValue);*/
				
				// Status byte, update running status.
				Message::data[0] = inByte;
				Message::running_status = inByte;
				Message::index = 1;
				
				// Update expected length
				if (inByte >= EnableModulation) {
					
					// One byte message, direct handling.
					handleMessage();
					
					Message::index = 0;
					
				}
				else if (inByte == CoarseNote || 
						 inByte == PortamentoMode)
				{
				
					Message::length = 2;
					
				}
				else {
					
					Message::length = 3;
					
				}

				
				Message::length = (inByte == CoarseNote) ? 2 : 3;
				
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
	
	void handleMessage()
	{
		
		switch (Message::data[0]) {
				
			case CoarseNote:
			{
				StaticNote_coarse = Message::data[1];
				StaticNote_fine = 0;
				
#if COMPFLAG_PORTAMENTO
				Portamento::counter = 0;
#endif
				
#if DEBUG_SERIAL
				Serial << "C";
#endif
			}
				break;
				
			case FineNote:
			{
				StaticNote_coarse = Message::data[1];
				StaticNote_fine = Message::data[2];
				
#if COMPFLAG_PORTAMENTO
				Portamento::counter = 0;
#endif
				
#if DEBUG_SERIAL
				Serial << "F";
#endif
			}
				break;
				
			case GlobalDetune:
			{
				 // \todo vérifier les shift signed
				gGlobalDetune = ((unsigned)Message::data[1] << 7) | Message::data[2];
				
#if DEBUG_SERIAL
				Serial << "D";
#endif
			}
				break;
				
#if COMPFLAG_PORTAMENTO
			case PortamentoValue:
			{
				Portamento::size = ((unsigned)Message::data[1] << 7) | Message::data[2];
				
				// Precaution ?
				Portamento::counter = 0;
			}
				break;
				
			case PortamentoMode:
			{
				Portamento::flags = Message::data[1];
				
				// Precaution ?
				Portamento::counter = 0;
			}
				break;
				
#endif // COMPFLAG_PORTAMENTO
			
#if COMPFLAG_ANALOGIN
			case EnableModulation:
				
				gModulationActivated = true;
				
				break;
				
			case DisableModulation:
				
				gModulationActivated = false;
				
				break;
				
			case ToggleModulation:
				
				gModulationActivated = !gModulationActivated;
				
				break;
	
#if COMPFLAG_FILTER
				
			case EnableFiltering:
				
				gModulationFilterActivated = true;
				
				break;
				
			case DisableFiltering:
				
				gModulationFilterActivated = false;
				
				break;
				
			case ToggleFiltering:
				
				gModulationFilterActivated = !gModulationFilterActivated;
				
				break;	
				
#endif // COMPFLAG_FILTER
				
#endif // COMPFLAG_ANALOGIN
				
#if COMPFLAG_SYNC
				
			case EnableSync:
				
				Sync::enable();
				
				break;
				
			case DisableSync:
				
				Sync::disable();
				
				break;
				
			case ToggleSync:
				
				Sync::toggle();
				
				break;	
				
#endif // COMPFLAG_SYNC	
			
			case EnableOutput:
				
				gActive = true;
				
				gRefreshFrequency = true;
				recomputeFrequency();
				
				break;
			
			case DisableOutput:
				
				gActive = false;
				
				TCCR1B = 0x00;
				
				break;
			
			case ToggleOutput:
				
				gActive = !gActive;
				
				if (gActive) {
					
					gRefreshFrequency = true;
					recomputeFrequency();
					
				}
				else {
					
					TCCR1B = 0x00;
					
				}
				
				break;
				
			default:
				// Parse Error
#if DEBUG_SERIAL
				Serial << "*";
#endif
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
	
};



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

