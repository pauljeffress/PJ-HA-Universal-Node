//
// ======= sendserialtoslave()
//

#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef SERIALSLAVE

void sendserialtoslave(uint8_t sendsertype, uint8_t sendparam) {
  // This is called whenever the Master/Mega needs to send the Uno/slave some new info. 
  // e.g. tell it to update it's LEDs perhaps.
  // Send a two byte serial msg to Uno.
  // The two bytes are proceeded by  a frame header of "PJ" or 0x50, 0x4A.
  // As the serial connection is full duplex, we just send when we want to. 

  CONSOLE_PORT.println("===== Sending Serial Frame ======");
  CONSOLE_PORT.println("PJ");
  CONSOLE_PORT.println(sendsertype);
  CONSOLE_PORT.println(sendparam);
  CONSOLE_PORT.println("=================================");

  Serial1.write(0x50);  
  Serial1.write(0x4A);
  Serial1.write(sendsertype);
  Serial1.write(sendparam);
  
} // END - sendserialtoslave()
#endif //SERIALSLAVE