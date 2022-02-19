// =========== mqttinbound()
//
// MQTT/IP - CHECK CONNECTION STILL UP 
//              - IF YES - check if packet has arrived and if so laucnch callback to process it.
//              - IF NO - RE-ESTABLISH MQTT CONNECTION & RE-SUBSCRIBE
//              

#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef ETHNODETYPE  // i.e this is an Ethernet Node.

void mqttinbound() {
  if (!mqttClient.loop()) {      // check connection MQTT server and process MQTT subscription input
    mqttCon = 0;                  // if you get to this line the MQTT/IP connection is down.
    digitalWrite(MQCON, LOW);     // switch off the MQTT/IP Connection LED, as we have lost connection.
    
    // PJ 19-2-22 I modified the below so it only tries once and then moves on. That way we don't get stuck
    //            here and stop the node from doing local jobs anyway.
    
    //while(mqttCon != 1){          // stay in this WHILE loop and try to reconnect every 2 seconds, until successful connection.
    //        // xxxx - can get stuck in here if can't re-establish MQTT connection!!!!!!!!!!!
      mqttCon = mqttClient.connect(clientName, "mosquittouser", "mosquittouser");   // try to connect
    //  delay(2000);                                // wait 2 secs
    //  }                                     
    if(mqttCon){                      // Yes, we have re-established the MQTT/IP link so,
      digitalWrite(MQCON, mqttCon);   // turn on MQTT link indicator and
      mqttClient.subscribe(subTopic); // re-subscribe to mqtt topic
      }
    }
} // END - mqttcheckconnection()

#endif // ETHNODETYPE
