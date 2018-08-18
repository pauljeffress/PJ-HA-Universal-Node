// ======================= txRadio() ==============
// Transmits the 'mes'-struct via RF to the RF<>ETH gateway
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef RFNODETYPE

void txRadio() 
  {
  #ifdef DEBUG
    Serial.print("-> Atempting to  transmit radio msg - ");  
    Serial.print("Node: ");
    Serial.print(mes.nodeID);
    Serial.print(" device: ");
    Serial.print(mes.devID);
    Serial.print(" cmd: ");
    Serial.print(mes.cmd);
    Serial.print(" intVal: ");
    Serial.print(mes.intVal);
    Serial.print(" fltintVal: ");
    Serial.print(mes.fltintVal);
    Serial.print(" payLoad:");
    Serial.print(mes.payLoad);
    //Serial.print(" RSSI: ");
    //Serial.println(radio.RSSI);
    Serial.print(" mes size:");
    Serial.println(sizeof(mes));
  #endif

  uint8_t RH_SendStatus = 0;  // I use this to track detailed response from RH's sendtoWait function.

  CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
  
  //RadioHead based sender code
  Serial.print("Sending to Mesh Addr: ");
  Serial.println(GATEWAYID);  

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
  //   Serial.println("Message reliably delivered.");
  // }
  // else
  // {   
  //   //The send failed...
  //   Serial.println("sendtoWait failed. rf network issue?");
  //   #ifdef DEBUGPJ2
  //     Serial.print("No connection with GW RF node:");
  //     Serial.println(GATEWAYID);
  //   #endif
  // }

  RH_SendStatus = manager.sendtoWait((uint8_t*)&mes, sizeof(mes), GATEWAYID);
  
  if (RH_SendStatus == RH_ROUTER_ERROR_NONE) Serial.println("Message reliably delivered.");
  else if (RH_SendStatus == RH_ROUTER_ERROR_INVALID_LENGTH) Serial.println("SEND FAIL: RH library - message too large.");     
  else if (RH_SendStatus == RH_ROUTER_ERROR_NO_ROUTE) Serial.println("SEND FAIL: RH library - no route found.");
  else if (RH_SendStatus == RH_ROUTER_ERROR_UNABLE_TO_DELIVER) Serial.println("SEND FAIL: RH library - Unable to deliver.");
  else Serial.println("SEND FAIL: returned error");     




  #ifdef DEBUGXX
    Serial.println("txRadio() ending");  
  #endif
} // end txRadio

#endif // RFNODETYPE
