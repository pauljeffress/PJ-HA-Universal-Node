//
// ================ LCDNEXTION_FPS Functions ===================
// 
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef LCDNEXTION_FPS

//
// ================ writeLCDNEXTION_FPS_instruction()
// Write a string to the NEXTIONLCD_FPS into the scroll_inst text box.
//
void writeLCDNEXTION_FPS_instruction(char* theStr)
{
  // theStr - the string to display at the above coordinates.
  
  #ifdef DEBUGPJ2
    CONSOLE_PORT.println("S: localwriteLCDGEN");
    CONSOLE_PORT.print(">");CONSOLE_PORT.print("theStr");CONSOLE_PORT.println("<");
  #endif

  strcpy(NextionLCD_FPS_buffer, theStr);
  scroll_inst.setText(NextionLCD_FPS_buffer);
    
}  // END - writeLCDNEXTION_FPS_instruction()

#endif // LCDNEXTION_FPS


