// These subroutines are called on ETHNODETYPE nodes from dosends() 
//

#include "a.h" // My global defines and extern variables to help multi file comilation.

//
// ======== sendInteger()
//
void sendInteger(int sendNodeID, int sendDevID, int sendInt){
      sprintf(buff_mess,  "%d", sendInt);                                          //    copy upTime into the buff_Mess (i.e. payload we will send north)
      sprintf(buff_topic, "home/eth_nd/nb/node%02d/dev%03d", sendNodeID, sendDevID); //    copy the correct topic to publish this info out on into buff_topic 
      CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
      mqttClient.publish(buff_topic,buff_mess);                                 // MQTT publish the topic & payload
} // END - sendInteger()


//
// ======== sendStatusInteger()
//
void sendStatusInteger(int sendNodeID, int sendDevID, int sendInt){
      if (sendInt == 1 )sprintf(buff_mess, "ON"); //    copy/convert status integer into the buff_Mess (i.e. payload we will send north)
      if (sendInt == 0 )sprintf(buff_mess, "OFF");
      sprintf(buff_topic, "home/eth_nd/nb/node%02d/dev%03d", sendNodeID, sendDevID); //    copy the correct topic to publish this info out on into buff_topic 
      CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
      mqttClient.publish(buff_topic,buff_mess);                                 // MQTT publish the topic & payload
} // END - sendStatusInteger()


//
// ======== sendString()
//
void sendString(int sendNodeID, int sendDevID, char* sendStr){
int i; 
      //Serial.println(sendStr);
      //for (i=0; i<sizeof(sendStr); i++){                  //    copy the string into the buff_Mess (i.e. payload we will send north)
      for (i=0; i<8; i++){                  //    copy the string into the buff_Mess (i.e. payload we will send north)
      buff_mess[i] = (sendStr[i]); }
      buff_mess[i] = '\0';
      sprintf(buff_topic, "home/eth_nd/nb/node%02d/dev%03d", sendNodeID, sendDevID); //    copy the correct topic to publish this info out on into buff_topic 
      CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
      mqttClient.publish(buff_topic,buff_mess);           // MQTT publish the topic & payload
} // END - sendString()


//
// =============== sendReal()
//
void sendReal(int sendNodeID, int sendDevID, float sendR){
      dtostrf(sendR, 7, 2, buff_mess);        
      sprintf(buff_topic, "home/eth_nd/nb/node%02d/dev%03d", sendNodeID, sendDevID);  
      CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
      mqttClient.publish(buff_topic,buff_mess);             
} // END - sendReal()




