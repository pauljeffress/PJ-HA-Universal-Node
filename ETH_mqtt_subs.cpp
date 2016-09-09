#include "a.h" // My global defines and extern variables to help multi file comilation.

//
// ========= mqtt_subs_senderror()
//
void mqtt_subs_senderror() {
  // if we have an error, send an MQTT north with devID = 91 from the GW/Node, with error details in the MQTT payload. Publish it.
  if ((error != 0) && verbose)          
    {
    sprintf(buff_mess, "syntax error %d for node %02d", error,NODEID);  
    sprintf(buff_topic, "home/eth_nd/nb/node%02d/dev091", NODEID);             // construct MQTT topic and message
    mqttClient.publish(buff_topic,buff_mess);                       // publish ...
    #ifdef DEBUG
        Serial.print("Syntax error code is: ");
        Serial.println(error);
    #endif
   }
} // END - mqtt_subs_senderror()








