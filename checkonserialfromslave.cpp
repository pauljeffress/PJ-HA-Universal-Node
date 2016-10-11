/*  Edit Log
 *  ========
 *  14-02-16 - Modified the debug serial print statements.
 *  
 */
#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef SERIALSLAVE
//
// ======== checkonserialfromslave()
//
void checkonserialfromslave(){
  // See if slave Arduino has sent any messages up to us recently.  
  // For example any button presses???
  // The slave merly tells us if a button was tapped (i.e. pressed and released), its up to
  // this Mega (in localactions() ) to then act on it and perhaps change the state of a local
  // actuator, and also send notification that the button has been tapped upstream via MQTT
  // to HA Master etc.
boolean validframe = false;
char firstchar;
char secondchar;
int  recsertype;
int  recparam;

  // see if a valid frame has arrived, and read it in. 
  if (Serial1.available()) 
    {
    Serial.println("Rec'd a char off serial from Uno");
    firstchar = Serial1.read();
    Serial.println(firstchar);
    if (firstchar == 0x50) // i.e. "P" - we are correctly at the start of a 4 byte frame
      {
      delay(1000);
      secondchar = Serial1.read();
      Serial.println(secondchar);
      if (secondchar == 0x4A) // i.e. "J" - we are correctly at the 2nd byte of a 4 byte frame
        {
        recsertype = Serial1.read();
        Serial.println(recsertype);
        if ((recsertype >= 0) && (recsertype <10)) // quick bounds check
          {
          recparam = Serial1.read();
          Serial.println(recparam);
          validframe = true;
          }
        }
      }
    }

  //  if frame was valid then action it
  if (validframe) 
    {
    Serial.println("====== Frame was valid ======");
    switch(recsertype) 
      {
      case 1:   // HELLO
        // TODO - implement hello code
        break;

      case 2:   // KEYPRESSED
        Serial.println("it was a KEYPRESSED event");
        if (recparam == 1) extendedbutton1tapped = true;  // this will be acted on and cleared in localactions()
        if (recparam == 2) extendedbutton2tapped = true;  // this will be acted on and cleared in localactions()    
        if (recparam == 3) extendedbutton3tapped = true;  // this will be acted on and cleared in localactions()  
        if (recparam == 4) extendedbutton4tapped = true;  // this will be acted on and cleared in localactions()
        break;
        
      }
    }
} //END - checkonserialfromslave()
#endif // SERIALSLAVE
