// ======================= txRadio() ==============
// Transmits the 'mes'-struct via RF to the RF<>ETH gateway
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

void txRadio() 
  {
  #ifdef DEBUG
    Serial.print("-> Attempint to transmit radio msg - ");  
    Serial.print("Node: ");
    Serial.print(mes.nodeID);
    Serial.print(" device: ");
    Serial.print(mes.devID);
    Serial.print(" cmd: ");
    Serial.print(mes.cmd);
    Serial.print(" val: ");
    Serial.print(mes.intVal);
    Serial.print(" val: ");
    Serial.print(mes.fltVal);
    Serial.print(" RSSI: ");
    Serial.println(radio.RSSI);
  #endif
  CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
  if (radio.sendWithRetry(GATEWAYID, (const void*)(&mes), sizeof(mes)));
  else Serial.println("No connection...or no ACK????...or using this send incorrectly check github");
        // xxxx - when we hit the ELSE above, we should also flash an error/pwr LED a few times if available??? S.O.S pattern :)
} // end txRadio
