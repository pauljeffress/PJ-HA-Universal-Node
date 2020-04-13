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
  //     CONSOLE_PORT.println();
  //     CONSOLE_PORT.println("<<<rf-rx<<< so starting in receiveData()"); 
  //   #endif
    
  
  
  //if (radio.DATALEN == sizeof(mes)) // we got valid sized message. // old pre RadioHead code.
    
  // New RadioHead code to check for and grab an RF packet
  uint8_t len = sizeof(mes);
  uint8_t from;
  if (manager.recvfromAck((uint8_t*)&mes, &len, &from)) // a valid RadioHead message was received for this node.
    {
    CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
    #ifdef DEBUGPJ2
      CONSOLE_PORT.print("Got a RadioHead message for this node from Node: ");
      CONSOLE_PORT.println(from);
      
    #endif
      
    if (len == HARFPACKSIZE) // we got valid sized home automation message from a node.
      {
      // XXXX populate mes from radioDataBuf, maybe use something like mes = *(Message*)radio.DATA;
      validPacket = true; 
      }

    if (!validPacket) // Bad message size - wrong message size means trouble
      {
      #ifdef DEBUGPJ2
        CONSOLE_PORT.println();
        CONSOLE_PORT.println("<<<< RF msg received but had invalid home automation message size.");
        CONSOLE_PORT.print("len:");
        CONSOLE_PORT.println(len);
        CONSOLE_PORT.print("expected mes size:");
        CONSOLE_PORT.println(HARFPACKSIZE);
      #endif  // DEBUGPJ2
      }
    else  // HA message size is good...    
      {
      #ifdef DEBUGPJ2
        CONSOLE_PORT.println();
        CONSOLE_PORT.println("<<<< RF msg received with correct size.");
        CONSOLE_PORT.print("Inbound Message from Node:");CONSOLE_PORT.println(from);
        CONSOLE_PORT.println("=========RF msg data===================");
        CONSOLE_PORT.print("From   NodeID:");CONSOLE_PORT.println(mes.nodeID);
        CONSOLE_PORT.print("      devID:");CONSOLE_PORT.println(mes.devID);
        CONSOLE_PORT.print("        cmd:");CONSOLE_PORT.println(mes.cmd);
        CONSOLE_PORT.print("     intVal:");CONSOLE_PORT.println(mes.intVal);
        CONSOLE_PORT.print("  fltintVal:");CONSOLE_PORT.println(mes.fltintVal);
        CONSOLE_PORT.print("    payLoad:");
        for (int i=0; i<32; i++) CONSOLE_PORT.print(mes.payLoad[i]);
        CONSOLE_PORT.println(":");
        CONSOLE_PORT.println("=======================================");
      #endif  // DEBUGPJ2 
      }
        // if (radio.ACKRequested()) radio.sendACK(); // respond to any ACK request // old pre RadioHead code.
    }
  return validPacket; // return code indicates packet received
} // end recieveData()
