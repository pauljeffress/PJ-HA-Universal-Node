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
  if (manager.sendtoWait((uint8_t*)&mes, sizeof(mes), GATEWAYID) == RH_ROUTER_ERROR_NONE)
  {
    // It has been reliably delivered to the next node.
    Serial.println("Message reliably delivered.");
  }
  else
  {   
    //The send failed...
    Serial.println("sendtoWait failed. rf network issue?");
    #ifdef DEBUGPJ2
      Serial.print("No connection with GW RF node:");
      Serial.println(GATEWAYID);
    #endif
  }

  // code below is the old pre RadioHead rf send code.
  //if (radio.sendWithRetry(GATEWAYID, (const void*)(&mes), sizeof(mes)));
  //else Serial.println("No con...or no ACK");

  #ifdef DEBUGXX
    Serial.println("txRadio() ending");  
  #endif
} // end txRadio

#endif // RFNODETYPE
