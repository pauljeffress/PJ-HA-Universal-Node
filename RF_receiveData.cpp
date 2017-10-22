// ======== receiveData() : 
// RF receive data from gateway over radio and
// simply places a received message into the "mes" structure, for actioning in parsecmd().
//

#include "a.h" // My global defines and extern variables to help multi file comilation.

bool receiveData() {
  bool validPacket = false;

  if (radio.receiveDone()) // check for received packets
    {
    #ifdef DEBUGx
      Serial.println();
      Serial.println("<<<rf-rx<<< so starting in receiveData()"); 
    #endif
    
    CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
  
    if (radio.DATALEN == sizeof(mes)) // we got valid sized message.
      {
      mes = *(Message*)radio.DATA;  // copy radio packet
      #ifdef DEBUGPJ2
        Serial.println("<<<< Inbound RF packet was correct size."); 
      #endif
      
      validPacket = true; // YES, we have a packet !
      signalStrength = radio.RSSI;
      #ifdef DEBUGPJ2
      Serial.print("Inbound Message from Node:");Serial.print(radio.SENDERID);Serial.print(" with RSSI:");Serial.println(radio.RSSI);
      Serial.println("=========RF msg data===================");
      Serial.print("From devID:");Serial.println(mes.devID);
      Serial.print("       cmd:");Serial.println(mes.cmd);
      Serial.print("    intVal:");Serial.println(mes.intVal);
      Serial.print(" fltintVal:");Serial.println(mes.fltintVal);
      Serial.print("To  NodeID:");Serial.println(mes.nodeID);
      Serial.print("   payLoad:");
            for (int i=0; i<32; i++) Serial.print(mes.payLoad[i]);
      Serial.println(":");
      Serial.println("=======================================");
      #endif  

      }  
    else  // wrong message size means trouble
      {
      #ifdef DEBUGPJ2
        Serial.println("ERROR: Inbound RF packet was incorrect size.");
        Serial.print("radio.DATALEN:");
        Serial.println(radio.DATALEN);
        Serial.print("expected mes size:");
        Serial.println(sizeof(mes));
      #endif
      }
 
  if (radio.ACKRequested()) radio.sendACK(); // respond to any ACK request
    }
  return validPacket; // return code indicates packet received
} // end recieveData()
