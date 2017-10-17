//==================== LOOP ========================================

#include "a.h" // My global defines and extern variables to help multi file comilation.

void loop() {

housekeeping(); // Assorted basic jobs that need to be checked up on.

#ifdef ETHNODETYPE
    mqttinbound();  // Check status of MQTT/IP connection to MQTT Broker & Receive/Process and subscriptions.
#endif

// ------------------------------------------------------
// ------------------------------------------------------
// check assorted locally attached DEVices, so we can 
// send MQTT updates and/or act on them later in loop().
#ifdef BUTTON1 or BUTTON2
    checkbuttons();    // Check for status change on buttons
#endif

#ifdef SWITCH1 or SWITCH2
    checkswitches();    // Check for status change on switches
#endif

#ifdef PIR1 or PIR2
    checkPIRs();    // Check for status change on PIR(s)
#endif
    
#ifdef SERIALSLAVE
    checkonserialfromslave();
#endif

#ifdef DS18
  DS18_check();
#endif // DS18
#ifdef DS18 // if DS18 and SLEEPY then wake up every 10 min to do measurement and go back to sleep
  DS18_sleepyStuff();
#endif // DS18

#ifdef PING1x or PING2
    //delay (250);  // just for debugging, not needed usually.  xxxx
    checkPINGs();    // take a reading of any PING sensors
#endif
// ------------------------------------------------------
// ------------------------------------------------------

localactions();   // Take local action on any buttons/PIR changes/PING detections/flags etc.

setPeriodicTransmissions();  // tag what parameters are to be sent in the "periodic transmissions"

#ifdef RFNODETYPE
    /* SEND RADIO PACKETS */
    sendMsg(); // send any radio messages whose sendXX flags are set.
    /* RECEIVE RADIO PACKETS and action them */
    if (receiveData()) parseCmd(); // receive from RF and parse/action any valid radio message.
#endif

#ifdef ETHNODETYPE
    // Note: We do not have to check for incoming MQTT messages explicitly here, as we registered 
    //       mqtt_subs() to be called by the PubSub library when a message arrives. A bit of processing
    //       is done in there, like setting sendXX flags etc.  So, all we have to do out here in loop() is
    //       call dosends() to send all flagged messages.
    dosends(); // do IP/MQTT sends based on status of all the sendXX flags.
#endif

} // end loop()

