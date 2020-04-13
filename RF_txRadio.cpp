// ======================= txRadio() ==============
// Transmits the 'mes'-struct via RF to the RF<>ETH gateway
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef RFNODETYPE

void txRadio() 
  {
  #ifdef DEBUG
    CONSOLE_PORT.print("-> Atempting to  transmit radio msg - ");  
    CONSOLE_PORT.print("Node: ");
    CONSOLE_PORT.print(mes.nodeID);
    CONSOLE_PORT.print(" device: ");
    CONSOLE_PORT.print(mes.devID);
    CONSOLE_PORT.print(" cmd: ");
    CONSOLE_PORT.print(mes.cmd);
    CONSOLE_PORT.print(" intVal: ");
    CONSOLE_PORT.print(mes.intVal);
    CONSOLE_PORT.print(" fltintVal: ");
    CONSOLE_PORT.print(mes.fltintVal);
    CONSOLE_PORT.print(" payLoad:");
    CONSOLE_PORT.print(mes.payLoad);
    //CONSOLE_PORT.print(" RSSI: ");
    //CONSOLE_PORT.println(radio.RSSI);
    CONSOLE_PORT.print(" mes size:");
    CONSOLE_PORT.println(sizeof(mes));
  #endif

  uint8_t RH_SendStatus = 0;  // I use this to track detailed response from RH's sendtoWait function.

  CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
  
  //RadioHead based sender code
  CONSOLE_PORT.print("Sending to Mesh Addr: ");
  CONSOLE_PORT.println(GATEWAYID);  

  // XXXX - copy mes into radioDataBuf here XXXXXX
  // uint8_t junkdata[] = "Hello World!";
  // for(int i=0; i++; i < sizeof(junkdata))
  //   {
  //     radioDataBuf[i] = junkdata[i];
  //   } 

  // Send a message to an rf69 mesh node
  // A route to the destination will be automatically discovered.
  // if (manager.sendtoWait((uint8_t*)&mes, sizeof(mes), GATEWAYID) == RH_ROUTER_ERROR_NONE)
  // {
  //   // It has been reliably delivered to the next node.
  //   CONSOLE_PORT.println("Message reliably delivered.");
  // }
  // else
  // {   
  //   //The send failed...
  //   CONSOLE_PORT.println("sendtoWait failed. rf network issue?");
  //   #ifdef DEBUGPJ2
  //     CONSOLE_PORT.print("No connection with GW RF node:");
  //     CONSOLE_PORT.println(GATEWAYID);
  //   #endif
  // }

  RH_SendStatus = manager.sendtoWait((uint8_t*)&mes, sizeof(mes), GATEWAYID);
  
  if (RH_SendStatus == RH_ROUTER_ERROR_NONE) CONSOLE_PORT.println("Message reliably delivered.");
  else if (RH_SendStatus == RH_ROUTER_ERROR_INVALID_LENGTH) CONSOLE_PORT.println("SEND FAIL: RH library - message too large.");     
  else if (RH_SendStatus == RH_ROUTER_ERROR_NO_ROUTE) CONSOLE_PORT.println("SEND FAIL: RH library - no route found.");
  else if (RH_SendStatus == RH_ROUTER_ERROR_UNABLE_TO_DELIVER) CONSOLE_PORT.println("SEND FAIL: RH library - Unable to deliver.");
  else CONSOLE_PORT.println("SEND FAIL: returned error");     




  #ifdef DEBUGXX
    CONSOLE_PORT.println("txRadio() ending");  
  #endif
} // end txRadio

#endif // RFNODETYPE
