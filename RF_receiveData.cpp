// ======== receiveData() : 
// RF receive data from gateway over radio and
// simply place a received message in "mes" structure, for actioning in parsecmd().
//

#include "a.h" // My global defines and extern variables to help multi file comilation.

bool receiveData() {
  bool validPacket = false;
  bool MesSizeOK = false;
  uint8_t tempRecvPacket[RF69_MAX_DATA_LEN];  // The RF packet layouts (in the code I picked up) were designed for AVRs only and so there is 
                                              // an expectation that a receiving device uses 16 bit unsigned integers for 'int's
                                              // rather than ARM (i.e. FEATHERM0RFM69 boards) which even when using the type
                                              // uint16_t, still stores it in 32 bits with padding.  So we need to write a special receive
                                              // routine here that is only used when we are on an ARM, that will pull in an 'AVR packet' 
                                              // (as the RFM_GW is an AVR) and spread it out into the correct variables for the ARM to
                                              // then work on.

  if (radio.receiveDone()) // check for received packets
    {
    #ifdef DEBUGPJ2
      Serial.println();
      Serial.println("<<<rf-rx<<< so Start receiveData()"); 
    #endif
    CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
  
    #ifndef FEATHERM0RFM69  // i.e. we are on an AVR, so no special processing required.
      if (radio.DATALEN == sizeof(mes))) // we got valid sized message from an AVR node.
      {
      mes = *(Message*)radio.DATA;  // copy radio packet
      MesSizeOK = true;
      Serial.println("Im an AVR and pack size OK"); 
      }
  
    #else // i.e. we are on an ARM, so need to do special packet processing.
      if (radio.DATALEN == 46) // we got valid sized message from an AVR node.
      {
      Serial.println("Im an ARM and pack size OK"); 
      Serial.print("radio.DATA:");
      for (int i=0; i<radio.DATALEN; i++)
        {
        Serial.print(radio.DATA[i]);
        Serial.print(":");  
        }
      Serial.println();
      // translate the message from ARM Node into std msg before proceeding.
      for (int i=0; i<radio.DATALEN; i++)
        {
        tempRecvPacket[i] = radio.DATA[i];  // copy radio packet into temp byte array, ready for manipulation.
        }
      int16_t PJnodeID = tempRecvPacket[0] + (tempRecvPacket[1] * 256);
      Serial.print("PJnodeID:"); Serial.println(PJnodeID);
      int16_t PJdevID = tempRecvPacket[2] + (tempRecvPacket[3] * 256);
      Serial.print("PJdevID:"); Serial.println(PJdevID);
      int16_t PJcmd = tempRecvPacket[4] + (tempRecvPacket[5] * 256);
      Serial.print("PJcmd:"); Serial.println(PJcmd);
      int32_t PJintVal = tempRecvPacket[6] + (tempRecvPacket[7] * 256) + (tempRecvPacket[8] * 65536) + (tempRecvPacket[9] * 65536 * 256) ;
      Serial.print("PJintVal:"); Serial.println(PJintVal);
      float PJfltVal = 987.65;  // I haven't workjed out how to convert the relevant four bytes in tempRecvPacket back into fltVal yet
                                // so for now I am just hard setting it at this obvious number.  Good thing is so far in my HA
                                // system I am not WRITing any DEVices that are floats. fltVal is only used for READs.
      Serial.print("PJfltVal:"); Serial.println(PJfltVal);
      uint8_t PJpayLoad[32];
      for (int i=14; i<radio.DATALEN; i++)
        {
        PJpayLoad[i-14] = radio.DATA[i];  // copy radio packet into temp byte array, ready for manipulation.
        }
      Serial.print("PJpayload:");
      for (int i=0; i<32; i++)
        {
        Serial.print(PJpayLoad[i]);
        Serial.print(":");  
        }
      Serial.println();

      // Now put all the parameters into mes so the rest of teh sketch can use it as normal.
      mes.nodeID = PJnodeID;
      mes.devID = PJdevID;
      mes.cmd = PJcmd;
      mes.intVal = PJintVal;
      mes.fltVal = PJfltVal;
      for (int i=0; i<32; i++)
      {
      mes.payLoad[i] = PJpayLoad[i];
      }
      MesSizeOK = true;  
      }
    #endif  
  
  
    if (!MesSizeOK) // wrong message size means trouble
        {
        #ifdef DEBUG
          Serial.println("<<<< RF msg received but had invalid message size.");
          Serial.print("radio.DATALEN:");
          Serial.println(radio.DATALEN);
  
          Serial.print("expected mes size:");
          Serial.println(sizeof(mes));
        #endif
        }
    else
      {
      //mes = *(Message*)radio.DATA;
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
