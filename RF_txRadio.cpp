// ======================= txRadio() ==============
// Transmits the 'mes'-struct via RF to the RF<>ETH gateway
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef RFNODETYPE

void txRadio() 
  {
    // #ifdef FEATHERM0RFM69
    //   typedef struct { // Radio packet structure max 66 bytes
    //     int16_t nodeID; // node identifier
    //     int16_t devID; // device identifier 0 is node; 31 is temperature, 32 is humidity
    //     int16_t cmd; // read or write
    //     int32_t intVal; // integer payload
    //     float fltVal; // floating payload
    //     char payLoad[32]; // char array payload
    //     } AVRMessage;
        
    //   AVRMessage AVRmes;

    //   AVRmes.nodeID = mes.nodeID;
    //   AVRmes.devID = mes.devID;
    //   AVRmes.cmd = mes.cmd;
    //   AVRmes.intVal = mes.intVal;
    //   AVRmes.fltVal = mes.fltVal;
    //   AVRmes.payLoad[32] = mes.payLoad[32];
    // #endif

    #ifdef DEBUG
    Serial.print("-> Attempint to transmit radio msg - ");  
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
    Serial.print(" payLoad:");
    Serial.print(mes.payLoad);
    Serial.print(" RSSI: ");
    Serial.println(radio.RSSI);
  #endif

  // Serial.print("AVRmes size:");
  // Serial.println(sizeof(AVRmes));

  Serial.print("mes size:");
  Serial.println(sizeof(mes));


  CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
  #ifdef DEBUG
    Serial.print("about to call sendWithRetry()");  
  #endif
 
  // #ifdef FEATHERM0RFM69
  //   if (radio.sendWithRetry(GATEWAYID, (const void*)(&AVRmes), sizeof(AVRmes)));
  //   else Serial.println("FEATHER:No con...or no ACK");
  // #else
    if (radio.sendWithRetry(GATEWAYID, (const void*)(&mes), sizeof(mes)));
    else Serial.println("AVR:No con...or no ACK");
        // xxxx - when we hit the ELSE above, we should also flash an error/pwr LED a few times if available??? S.O.S pattern :)
  // #endif

  #ifdef DEBUG
    Serial.println("txRadio() ending");  
  #endif
} // end txRadio

#endif // RFNODETYPE