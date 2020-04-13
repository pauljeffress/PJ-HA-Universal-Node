//
// ================ LCDGEN Functions ===================
// 
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef LCDGENERIC

//
// ================ writeLCDGEN()
// Write a string to theGeneric LCD, based on current DevXX setiings
// This is called after an instruction (i.e. Dev324 = 1) received by this node.
//
void writeLCDGEN()
{
  #ifdef DEBUGPJ2
    CONSOLE_PORT.println("S: writeLCDGEN");
  #endif

  #ifdef LCDGENERIC_SFGLCD  // items specific to the SparkFun LCD-09351
    lcdGenStr[lcdGenStrLen] = '\0'; // ensure the lcdGenStr buffer has a terminator so when I print it below I don't print
                                    // the whole buffer, which may contain rubish chars beyond what was placed in there
                                    // by an MQTT WRITE.
    lcdGen.setX(lcdGenXPos * 6);  // this particular LCD takes X coord in num pixels, and one char is 6 pixels wide.
    lcdGen.setY(lcdGenYPos * 8);  // this particular LCD takes Y coord in num pixels, and one char is 8 pixels high.
    lcdGen.printStr(lcdGenStr);
    
  #endif // LCDGENERIC_SFGLCD

  #ifdef LCDGENERIC_BLAH  // items specific to the blah LCD
  #endif // LCDGENERIC_BLAH
   
}  // END - writeLCDGEN()

void localwriteLCDGEN(int xpos, int ypos, char* theStr)
{
  // xpos - X position to start writting the character string, top left = 0
  // ypos - Y position to start writting the character string, top left = 0
  // theStr - the string to display at the above coordinates.
  
  #ifdef DEBUGPJ2
    CONSOLE_PORT.println("S: localwriteLCDGEN");
    CONSOLE_PORT.print(">");CONSOLE_PORT.print("theStr");CONSOLE_PORT.println("<");
  #endif

  #ifdef LCDGENERIC_SFGLCD  // items specific to the SparkFun LCD-09351
    lcdGenStr[lcdGenStrLen] = '\0'; // ensure the lcdGenStr buffer has a terminator so when I print it below I don't print
                                    // the whole buffer, which may contain rubish chars beyond what was placed in there
                                    // by an MQTT WRITE.
    lcdGen.setX(xpos * 6);  // this particular LCD takes X coord in num pixels, and one char is 6 pixels wide.
    lcdGen.setY(ypos * 8);  // this particular LCD takes Y coord in num pixels, and one char is 8 pixels high.
    lcdGen.printStr(theStr);
    delay(100); // allow LCD to finish writting. I found I needed this else got screen corruption.
  #endif // LCDGENERIC_SFGLCD

  #ifdef LCDGENERIC_BLAH  // items specific to the blah LCD
  #endif // LCDGENERIC_BLAH
   
}  // END - localwriteLCDGEN()

#endif // LCDGEN


