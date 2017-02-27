
//
// ======== localactions()
//

#include "a.h" // My global defines and extern variables to help multi file comilation.

void localactions(){
  // Look at current local actuator/sensor status variables and ensure any local actuators are in sync with them.
  // We do this by checking the last known state of the local actuators/sensors and if it has changed we act, and also update
  // the last known state to the new state.
  //
  // The local actuator status variables may get changed by incoming MQTT messages, whereas the local sensor status variables
  // can by thier very nature update here on the device at anytime.  This is a key component of the Node being able to work
  // while offline to a degree.
  // Local rules can be implemented here, for example if PIR1 detects movement, then switch on Actuator 1 for X minutes?
  // Local rules may negate the need for some rules/logic up at the HA Master, such that the HA master is really just kept
  // aware of whats happening down on this local node.  The HA Master may come into play when additional more complex logic/rules/parameters
  // are required to make a decision.  For example, perhaps a sensore on another Node altogether is desired as input to an 
  // actuator on this node, only the Master HA could pull that together and issue a command.
  // commands from time to time. 
  //
  // ****** These actions are generally customised per node. ******

#ifdef EXTENDEDBUTTON1
  if (extendedbutton1tapped)
    {
    #ifdef DEBUGPJ2
        Serial.println("===== localactions() =====");
        Serial.println("extendedbutton1tapped");
    #endif
    actuator1status = !actuator1status; // toggle the associated actuator flag. Which will be acted on further down.
    if (actuator1status) sendserialtoslave(EXTENDEDACTUATORON, 1); // turn on the associated extended actuator (i.e. LED)
    else sendserialtoslave(EXTENDEDACTUATOROFF, 1); // turn off the associated extended actuator (i.e. LED)
    extendedbutton1tapped = false;      // clear the flag now that we have actioned it.
    }
#endif // EXTENDEDBUTTON1

#ifdef EXTENDEDBUTTON2
  if (extendedbutton2tapped)
    {
    actuator2status = !actuator2status; // toggle the associated actuator.
    if (actuator2status) sendserialtoslave(EXTENDEDACTUATORON, 2); // turn on the associated extended actuator (i.e. LED)
    else sendserialtoslave(EXTENDEDACTUATOROFF, 2); // turn off the associated extended actuator (i.e. LED)
    extendedbutton2tapped = false;      // clear the flag now that we have actioned it.
    }
#endif // EXTENDEDBUTTON2

#ifdef EXTENDEDBUTTON3
  if (extendedbutton3tapped)
    {
    actuator3status = !actuator3status; // toggle the associated actuator.
    if (actuator3status) sendserialtoslave(EXTENDEDACTUATORON, 3); // turn on the associated extended actuator (i.e. LED)
    else sendserialtoslave(EXTENDEDACTUATOROFF, 3); // turn off the associated extended actuator (i.e. LED)
    extendedbutton3tapped = false;      // clear the flag now that we have actioned it.
    }
#endif // EXTENDEDBUTTON3

#ifdef EXTENDEDBUTTON4
  if (extendedbutton4tapped)
    {
    actuator4status = !actuator4status; // toggle the associated actuator.
    if (actuator4status) sendserialtoslave(EXTENDEDACTUATORON, 4); // turn on the associated extended actuator (i.e. LED)
    else sendserialtoslave(EXTENDEDACTUATOROFF, 4); // turn off the associated extended actuator (i.e. LED)
    extendedbutton4tapped = false;      // clear the flag now that we have actioned it.
    }
#endif // EXTENDEDBUTTON4


// Keep the local Actuators in sync with thier status variables, in case they were changed by local action or MQTT msg.
// Also send an MQTT to let whomever is interested, know they have changed state.  This is important if they changed state
// due to something locally here on the node, we must keep things like OpenHAB up to date.
#ifdef ACTUATOR1
  if (actuator1status != lastactuator1status) // has its status variable been changed, if so act on it.
      {
      #ifdef ACTUATOR1REVERSE // for reversed Actuator behaviour do this
        digitalWrite(ACTUATOR1PIN, !actuator1status); // Set the ACTUATOR to new desired state. Reversed behaviour.
      #else // for normal Actuator behavior do this
        digitalWrite(ACTUATOR1PIN, actuator1status); // Set the ACTUATOR to new desired state.
      #endif
      lastactuator1status = actuator1status;  // keep track of last applied state of this actuator.
      send16 = true;                      // Advise upstream of new state of actuator.
      }   
#endif
#ifdef ACTUATOR2
  if (actuator2status != lastactuator2status) // has its status variable been changed, if so act on it.
    {
    #ifdef ACTUATOR2REVERSE // for reversed Actuator behaviour do this
      digitalWrite(ACTUATOR2PIN, !actuator2status); // Set the ACTUATOR to new desired state. Reversed behaviour.
    #else // for normal Actuator behavior do this
      digitalWrite(ACTUATOR2PIN, actuator2status); // Set the ACTUATOR to new desired state.
    #endif
    lastactuator2status = actuator2status;  // keep track of last applied state of this actuator.
    send17 = true;                      // Advise upstream of new state of actuator.
    }
#endif
#ifdef ACTUATOR3
  if (actuator3status != lastactuator3status) // has its status variable been changed, if so act on it.
    {
    digitalWrite(ACTUATOR3PIN, actuator3status); // Set the ACTUATOR to new desired state.
    lastactuator3status = actuator3status;  // keep track of last applied state of this actuator.
    send18 = true;                      // Advise upstream of new state of actuator.
    }
#endif
#ifdef ACTUATOR4
  if (actuator4status != lastactuator4status) // has its status variable been changed, if so act on it.
    {
    digitalWrite(ACTUATOR4PIN, actuator4status); // Set the ACTUATOR to new desired state.
    lastactuator4status = actuator4status;  // keep track of last applied state of this actuator.
    send19 = true;                      // Advise upstream of new state of actuator.
    }
#endif  

//Do we need to do fire up the FPS?
#ifdef FINGER   // All three of these DEVices need to be present.
  // We need to determine if we should start the FPS Run Mode State Machine...
  // but once we have enabled it, we should not 
  if (!EnableFPSRunModeStateMachine && ping1Detect && (lastBtn1State == FPS_MODE_RUN)) 
                    // If we haven't already started the StateMachine 
                    //  AND if PING1 gets a true detection 
                    //  AND the Mode Switch (BUTTON1) is still set to RUN mode, not DataBase mode, 
                    //  THEN flag that the FPS state machine should start, else skip it.
                    // Keep in mind that the PING1 gets a lockout from taking anymore samples for a set period 
                    // once it has made an in range detection. Also keep in mind other local actons may need
                    // to take place when a PING1 in range detection occurs, so be careful not to reset 'ping1Detect'
                    // as thats best left to the lockout timer code.
    {
    #ifdef DEBUGPJ2
      Serial.println("localactions(): As a PING detect occured, enabling FPS RUN state machine.");
    #endif
    EnableFPSRunModeStateMachine = true; // flag that says FPS RUN MODE state machine should start and stay running until this is no longer true;
    }

  if (EnableFPSRunModeStateMachine) // Now that the FPS state machine has been started, keep running it regardless of current PINGs
                          // until it deems it itself deems it is finished.
    {
    FPSRunModeSM();
    }
    

  if (lastBtn1State == FPS_MODE_DB)
    {
    #ifdef DEBUGPJ2
      Serial.println("localactions(): BUTTON1, our Mode Switch, is in DB_MODE position, so enabling FPS DB state machine.");
    #endif
    EnableFPSDBModeStateMachine = true; // flag that says FPS DB MODE state machine should start and stay running until this is no longer true;
    #ifdef LCDGENERIC // if we have an LCD....
      localwriteLCDGEN(0, 4, "FPS DB  Mode");
    #endif
    // xxxx - add code to do DB stuff here :)
    // xxxx - stuff like enrol fingers etc, if I ever want to do that on the Arduino not external Win PC.
    }
#endif // PING1 && FINGER && BUTTON1 

#ifdef OCEANMIRROR // Do this stuff if only if I have my Ocean Mirror attached via Serial to this Node
  if (extVar400 != mirrorDev400) // i.e. this extVar must have been updated recently, so I need to tell the Due.
    {
      mirrorDev400 = extVar400; // update my mirror copy of this ExtVar
      MIRROR_sendSerialToDue(400, mirrorDev400);  //  Tell the DUE which variable changed and what its new value is.
    }
  if (extVar401 != mirrorDev401) // i.e. this extVar must have been updated recently, so I need to tell the Due.
    {
      mirrorDev401 = extVar401; // update my mirror copy of this ExtVar
      // Tell ATX PSU to wake up and supply the main +5v for LED strip
      if (extVar401 != 0) // i.e we have been asked for a brightness > 0, remeber 0 means don't display anything, so turn on shared ATX PSU
        RMT_PWROn();
      else
        RMT_PWROff();     // we have been asked for brightness 0, so turn ATX PSU off.
      MIRROR_sendSerialToDue(401, mirrorDev401);  //  Tell the DUE which variable changed and what its new value is.
    }
#endif// OCEANMIRROR

} //END - localactions()
