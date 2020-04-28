/*
 * Serial2 generator for FeatherM0
 * 
 * If I need to use my FeatherM0 specific custom functions to build Serial2
 * i.e on a Node that needs one Serial for USB/Serial Monitor, one for esp-link
 * monitoring and finally, one for an RS232 link to another device. Example is
 * my Node06, where Serial2 is for the esp-link
 * 
 * See my Evernote;
 *  - Deck Lights Controller Node06 - Convert to Feather M0RFM69HCW and RadioHead
 */

#include "a.h" // My global defines and extern variables to help multi file compilation.


#ifdef GENERATE_SERIAL2

// bunch of #include and #defines that support the below are in a.IrqHandler
// there are is also some code in setup(); to get this stuff initialised.

Uart Serial2 (&sercom1, SERIAL2_RXPIN, SERIAL2_TXPIN, SERCOM_RX_PAD_0, UART_TX_PAD_2);

void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}


#endif  // of the FEATHERM0 or FEATHERM0RFM69