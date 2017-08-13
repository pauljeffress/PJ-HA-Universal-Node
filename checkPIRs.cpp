//
// ================ checkPIRs()
// Check if any PIRs have a detection.
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef PIR1 or PIR2

void checkPIRs() {

  #ifdef DEBUGPJx
	  Serial.println("S: CheckPIRs");
  #endif

  int pir1wait;
  int pir2wait;
  int newPIR1State, newPIR2State; // used to temp hold the sample taken on the way through here.

  #ifdef PIR1
    //PIRdelay; // give the grid time to stabilize for the PIR, otherwise false triggers occur after an RFM send/rcv (might need 2s)
    pir1wait = ((millis() - lastPIR1) < (PIRHOLDOFF * 1000)); // hold-off time for additional PIR1 messages 
  #endif

  #ifdef PIR2
    //PIRdelay; // give the grid time to stabilize for the PIR, otherwise false triggers occur after an RFM send/rcv (might need 2s)
    pir2wait = ((millis() - lastPIR2) < (PIRHOLDOFF * 1000)); // hold-off time for additional PIR2 messages
  #endif


#ifdef PIR1
    if (!pir1wait)  // i.e. we are not in the blocking period for this PIR.
      {
      newPIR1State = digitalRead(Pir1Input); // read PIR digital pin
      if (newPIR1State != curPIR1State) 
        { // PIR state change detected
        delay(5); // xxxx - do we need this?
        curPIR1State = newPIR1State;  // copy it into current variable, ready for sending and future comparing.
        lastPIR1 = millis(); // take new timestamp
        send41 = true; // set PIR1 message flag
        #ifdef DEBUGPJ2
          Serial.println("PIR1 state change");
        #endif
        }
      }
  #endif // PIR1

#ifdef PIR2
    if (!pir2wait)  // i.e. we are not in the blocking period for this PIR.
      {
      newPIR2State = digitalRead(Pir2Input); // read PIR digital pin
      if (newPIR2State != curPIR2State) 
        { // PIR state change detected
        delay(5); // xxxx - do we need this?
        curPIR2State = newPIR2State;  // copy it into current variable, ready for sending and future comparing.
        lastPIR2 = millis(); // take new timestamp
        send43 = true; // set PIR2 message flag
        #ifdef DEBUGPJ2
          Serial.println("PIR2 state change");
        #endif
        }
      }
  #endif // PIR1

} // END - checkPIRs()

#endif PIR1 or PIR2
