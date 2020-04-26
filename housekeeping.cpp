// =========== housekeeping()
// This function is called at the start of loop(), so runs every cycle of loop()
// and does any basic jobs that have to occur.

// CONTROL Comms LED AND CALCULATE UPTIME 
//

#include "a.h" // My global defines and extern variables to help multi file comilation.

void housekeeping() {

// Attend to STATUS LED
// Blink it on for roughly 1 sec, then off for roughly 1 sec, continuously. 
// This bit of code needs to be called often in order for it to work successfully. I wanted to avoid 
// using interupts etc as the node is already dealing with interupts from the Eth/RF interfaces.
  // work out if we are in an odd or even sec of uptime, swap LED state accordingly.

  //long uptimeSecs = millis()/1000;
  if (((millis() % STATUS_LED_CYCLE_PERIOD) == 0) &&  !StatusLEDStatus)  // If Status LED is off, check if its time to turn it on
                                                                           // i.e. are we at beginning of cycle
    {
      digitalWriteHighPower(STATUS_LED_PIN, HIGH_HP);
      StatusLEDonMillis = millis();  // grab a timestamp of when we turned the LED on.
      StatusLEDStatus = true;  // The LED is now on.
      #ifdef DEBUGPJx 
        CONSOLE_PORT.print("ON: ");
        CONSOLE_PORT.println(StatusLEDonMillis);
      #endif
    }

    if (StatusLEDStatus)   // i.e. its on, but is it time to turn it off yet?
    {            
    #ifdef DEBUGPJx
      CONSOLE_PORT.print("current: ");
      CONSOLE_PORT.println(millis());
    #endif
    if (millis() - StatusLEDonMillis > STATUS_LED_ON_PERIOD) // has timer expired?
      {
      #ifdef DEBUGPJx
        CONSOLE_PORT.print("OFF: ");
        CONSOLE_PORT.println(millis());
      #endif
      digitalWriteHighPower(STATUS_LED_PIN, LOW); // Turn the LED off
      StatusLEDStatus = false;  // The LED is now off.
      }
    }




// Attend to the CommsLED
  if (CommsLEDStart &&  !CommsLEDStatus)  // i.e. its not currently on, but something wants us turn it on.
    {
    digitalWriteHighPower(COMMS_LED_PIN, HIGH_HP);  // Turn the Comms LED on
    CommsLEDonMillis = millis();  // grab a timestamp of when we turned the LED on.
    CommsLEDStatus = true;  // The LED is now on.
    CommsLEDStart = false;  // Reset the start flag as we have started.
    }
   
  if (CommsLEDStatus)   // i.e. its on, but is it time to turn it off yet?
    {            
    if (millis() - CommsLEDonMillis > COMMS_LED_ON_PERIOD) // has timer expired?
      {
      digitalWriteHighPower(COMMS_LED_PIN, LOW); // Turn the Comms LED off
      CommsLEDStatus = false;  // The LED is now off.
      }
    }


// Attend to the 'upTime' counter
    if (lastMinute != (millis()/60000)) {     // another minute passed ?
      lastMinute = millis()/60000;
      upTime++;
      #ifdef DEBUGXX
        CONSOLE_PORT.print("Uptime:");
        CONSOLE_PORT.print(upTime);
        CONSOLE_PORT.print("  CommsLEDonMillis:");
        CONSOLE_PORT.print(CommsLEDonMillis);
        CONSOLE_PORT.print("  millis:");
        CONSOLE_PORT.print(millis());
        CONSOLE_PORT.print("  COMMS_LED_ON_PERIOD:");
        CONSOLE_PORT.println(COMMS_LED_ON_PERIOD);        
      #endif
      }


// Attend to the 'canRfTx' flag
    if (lastRfTxTime != (millis()/CANRFTX_DELAY)) {     // has another period of CANRFTX_DELAY mSec passed ?
      lastRfTxTime = millis()/CANRFTX_DELAY;  // Yes - well lets store the current millis time.
      rfTxAllowed = true;                   // And allow a tx to happen.
      
      #ifdef DEBUGPJ
        CONSOLE_PORT.print("rfTxAllowed set at:");
        CONSOLE_PORT.println(lastRfTxTime);
      #endif
      }

} // END - housekeeping()
