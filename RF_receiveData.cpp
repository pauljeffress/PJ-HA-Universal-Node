// ======== receiveData() : 
// RF receive data from gateway over radio and
// simply place a received message in "mes" structure, for actioning in parsecmd().
//

#include "a.h" // My global defines and extern variables to help multi file comilation.

bool receiveData() {
  bool validPacket = false;
  if (radio.receiveDone()) // check for received packets
    {
    CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
    if (radio.DATALEN != sizeof(mes)) // wrong message size means trouble
      #ifdef DEBUG
        Serial.println("Received invalid message structure..")
      #endif
      ;
    else
      {
      mes = *(Message*)radio.DATA;
      validPacket = true; // YES, we have a packet !
      signalStrength = radio.RSSI;
      #ifdef DEBUG
        Serial.print("<-- Received message: ");
        Serial.print("Node: ");
        Serial.print(mes.nodeID);
        Serial.print(" device: ");
        Serial.print(mes.devID);
        Serial.print(" cmd: ");
        Serial.print(mes.cmd);
        Serial.print(" intVal: ");
        Serial.print(mes.intVal);
        Serial.print(" fltVal: ");
        Serial.print(mes.fltVal);
        Serial.print(" payLoad: ");
        Serial.print(mes.payLoad);
        Serial.print(" , RSSI= ");
        Serial.println(radio.RSSI);
      #endif  
      }
    }
  if (radio.ACKRequested()) radio.sendACK(); // respond to any ACK request
  return validPacket; // return code indicates packet received
} // end recieveData()
