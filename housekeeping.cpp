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
  long uptimeSecs = millis()/1000;
  if ((uptimeSecs & 1) == 0) digitalWrite(STATUS_LED_PIN, HIGH);  // Turn the Comms LED on as we are in an odd second
  else digitalWrite(STATUS_LED_PIN, LOW);  // Turn the Comms LED on as we are in an even second
  

// Attend to the CommsLED
  if (CommsLEDStart &&  !CommsLEDStatus)  // i.e. its not currently on, but something wants us turn it on.
    {
    digitalWrite(COMMS_LED_PIN, HIGH);  // Turn the Comms LED on
    CommsLEDonMillis = millis();  // grab a timestamp of when we turned the LED on.
    CommsLEDStatus = true;  // The LED is now on.
    CommsLEDStart = false;  // Reset the start flag as we have started.
    }
   
  if (CommsLEDStatus)   // i.e. its on, but is it time to turn it off yet?
    {            
    if (millis() - CommsLEDonMillis > COMMS_LED_ON_PERIOD) // has timer expired?
      {
      digitalWrite(COMMS_LED_PIN, LOW); // Turn the Comms LED off
      CommsLEDStatus = false;  // The LED is now off.
      }
    }


// Attend to the 'upTime' counter
    if (lastMinute != (millis()/60000)) {     // another minute passed ?
      lastMinute = millis()/60000;
      upTime++;
      }

} // END - housekeeping()
