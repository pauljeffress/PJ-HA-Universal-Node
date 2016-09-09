//
// ================ checkPIRs()
//
void checkPIRs() {

  int pir1wait;
  int pir2wait;
  
  #ifdef PIR1 or PIR2
    //PIRdelay; // give the grid time to stabilize for the PIR, otherwise false triggers occur after an RFM send/rcv (might need 2s)
    pir1wait = ((millis() - lastPIR1) < (PIRHOLDOFF * 1000)); // hold-off time for additional PIR1 messages 
    pir2wait = ((millis() - lastPIR2) < (PIRHOLDOFF * 1000)); // hold-off time for additional PIR2 messages
  #endif
  
  #ifdef PIR1
    curPIR1State = digitalRead(Pir1Input); // read PIR digital pin
    if (!pir1wait && (curPIR1State == 1)) 
      { // presence detected!
      delay(5);
      lastPIR1 = millis(); // take timestamp
      send41 = true; // set PIR1 message flag
      Serial.println("PIR1 detect");
      }
  #endif

  #ifdef PIR2
    curPIR2State = digitalRead(Pir2Input); // read PIR digital pin
    if (!pir2wait && (curPIR2State == 1)) 
      { // presence detected!
      delay(5);
      lastPIR2 = millis(); // take timestamp
      send43 = true; // set PIR2 message flag
      Serial.println("PIR2 detect");
      }
  #endif
} // END - checkPIRs()

