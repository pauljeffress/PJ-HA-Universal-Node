/*
 * digitalWriteHighPower()
 * 
 * This is my enhanced version of the standard digitalWrite() function, that allows me to turn on
 * the high strength driver mode that means a SAMD21 can provide max 7mA source current vs the
 * default of 2mA.
 * 
 * See my Evernotes;
 *  - Deck Lights Controller Node06 - Convert to Feather M0RFM69HCW and RadioHead
 *  - SAMD21 port power issue driving LEDs
 */

#include "a.h" // My global defines and extern variables to help multi file compilation.

// #define HIGH_HP         (0x2)   // Already defined in a.h

void digitalWriteHighPower( uint32_t ulPin, uint32_t ulVal)
{
  // based on https://lowpowerlab.com/forum/moteino-m0/m0-gpio-output-power-setting/
  
  if ( g_APinDescription[ulPin].ulPinType == PIO_NOT_A_PIN )
  {
    return ;
  }

  EPortType port = g_APinDescription[ulPin].ulPort;
  uint32_t pin = g_APinDescription[ulPin].ulPin;
  uint32_t pinMask = (1ul << pin);

  if ( (PORT->Group[port].DIRSET.reg & pinMask) == 0 ) {
    PORT->Group[port].PINCFG[pin].bit.PULLEN = ((ulVal == LOW) ? 0 : 1) ;
  }

  switch ( ulVal )
  {
    case LOW:
      PORT->Group[port].OUTCLR.reg = pinMask;
    break ;

    case HIGH_HP:
      PORT->Group[port].OUTSET.reg = pinMask;
      PORT->Group[g_APinDescription[ulPin].ulPort].PINCFG[g_APinDescription[ulPin].ulPin].bit.DRVSTR = 1;
    break ;

    default:
      PORT->Group[port].OUTSET.reg = pinMask;
    break ;
  }

  return ;
}



