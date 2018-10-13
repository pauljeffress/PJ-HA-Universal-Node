//
// ================ LCDNEXTION Functions ===================
// 
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef LCDNEXTION

//
// ================ writeLCDNEXTION_instruction()
// Write a string to the NEXTIONLCD into the scroll_inst text box.
//
void writeLCDNEXTION_instruction(char* theStr)
{
  // theStr - the string to display at the above coordinates.
  
  #ifdef DEBUGPJ
    Serial.println("S: writeLCDNEXTION");
    Serial.print(">");Serial.print("theStr");Serial.println("<");
  #endif

  strcpy(NextionLCD_buffer, theStr);
  scroll_inst.setText(NextionLCD_buffer);
    
}  // END - writeLCDNEXTION_instruction()

#endif // LCDNEXTION


