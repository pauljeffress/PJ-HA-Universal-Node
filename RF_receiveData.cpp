// ======== receiveData() : 
// RF receive data from gateway over radio and
// simply places a received message into the "mes" structure, for actioning in parsecmd().
//

#include "a.h" // My global defines and extern variables to help multi file comilation.

bool receiveData() {
  bool validPacket = false;

  // if (radio.receiveDone()) // check for received packets
  //   {
  //   #ifdef DEBUGx
  //     Serial.println();
  //     Serial.println("<<<rf-rx<<< so starting in receiveData()"); 
  //   #endif
    
  
  
  //if (radio.DATALEN == sizeof(mes)) // we got valid sized message. // old pre RadioHead code.
    
  // New RadioHead code to check for and grab an RF packet
  uint8_t len = sizeof(mes);
  uint8_t from;
  if (manager.recvfromAck((uint8_t*)&mes, &len, &from)) // a valid RadioHead message was received for this node.
    {
    CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
    #ifdef DEBUGPJ2
      Serial.print("Got a RadioHead message for this node from Node: ");
      Serial.println(from);
      
    #endif
      
    if (len == HARFPACKSIZE) // we got valid sized home automation message from a node.
      {
      // XXXX populate mes from radioDataBuf, maybe use something like mes = *(Message*)radio.DATA;
      validPacket = true; 
      }

    if (!validPacket) // Bad message size - wrong message size means trouble
      {
      #ifdef DEBUGPJ2
        Serial.println();
        Serial.println("<<<< RF msg received but had invalid home automation message size.");
        Serial.print("len:");
        Serial.println(len);
        Serial.print("expected mes size:");
        Serial.println(HARFPACKSIZE);
      #endif  // DEBUGPJ2
      }
    else  // HA message size is good...    
      {
      #ifdef DEBUGPJ2
        Serial.println();
        Serial.println("<<<< RF msg received with correct size.");
        Serial.print("Inbound Message from Node:");Serial.println(from);
        Serial.println("=========RF msg data===================");
        Serial.print("From   NodeID:");Serial.println(mes.nodeID);
        Serial.print("      devID:");Serial.println(mes.devID);
        Serial.print("        cmd:");Serial.println(mes.cmd);
        Serial.print("     intVal:");Serial.println(mes.intVal);
        Serial.print("  fltintVal:");Serial.println(mes.fltintVal);
        Serial.print("    payLoad:");
        for (int i=0; i<32; i++) Serial.print(mes.payLoad[i]);
        Serial.println(":");
        Serial.println("=======================================");
      #endif  // DEBUGPJ2 
      }
        // if (radio.ACKRequested()) radio.sendACK(); // respond to any ACK request // old pre RadioHead code.
    }
  return validPacket; // return code indicates packet received
} // end recieveData()
