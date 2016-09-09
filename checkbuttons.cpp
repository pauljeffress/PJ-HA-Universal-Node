//
// ================ checkbuttons()
// Checks if the physical BUTTONs on this node have been pressed and if so
// update associated ACTUATOR status and action it. 
// Note: There is an assumption that BUTTON 1/2 controls ACTUATOR1/2
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

void checkbuttons() {
  #ifdef BUTTON1
    curBtn1State = digitalRead(BUTTON1PIN);
    msgBlock = ((millis() - lastBtn1Press) < HOLDOFF); // hold-off time for additional button messages
    if (!msgBlock && (curBtn1State != lastBtn1State)) 
      { // input changed ?
      delay(5);
      lastBtn1Press = millis(); // take timestamp
      send40 = true; // set button1 message flag
      if (curBtn1State == LOW) 
        {
        if (toggleOnButton1) 
          { // button in toggle state ?
          #ifdef ACTUATOR1
            actuator1status = !actuator1status; // toggle output
            digitalWrite(ACTUATOR1PIN, actuator1status);
            send16 = true; // set ACT1 / LED message flag
          #endif
          } 
        else
          if (TIMinterval > 0 && !timerOnButton1) 
            { // button in timer state ?
            timerOnButton1 = true; // start timer interval
            #ifdef ACTUATOR1
              actuator1status = HIGH; // switch on ACT1
              digitalWrite(ACTUATOR1PIN, actuator1status);
              send16 = true; // set ACT1 / LED message flag
            #endif
            }
        }
      lastBtn1State = curBtn1State;
      }
      
    // TIMER CHECK for button 1
    //
    if (TIMinterval > 0 && timerOnButton1) // =0 means no timer
      {
      if ( millis() - lastBtn1Press > TIMinterval*1000) 
        { // timer expired ?
        timerOnButton1 = false; // then end timer interval
        #ifdef ACTUATOR1
          actuator1status = LOW; // and switch off Actuator
          digitalWrite(ACTUATOR1PIN, actuator1status);
          send16 = true; // set ACT1 / LED message flag
        #endif
        }
      }
  #endif

  #ifdef BUTTON2
    curBtn2State = digitalRead(BUTTON2PIN);
    msgBlock = ((millis() - lastBtn2Press) < HOLDOFF); // hold-off time for additional button messages
    if (!msgBlock && (curBtn2State != lastBtn2State)) 
      { // input changed ?
      delay(5);
      lastBtn2Press = millis(); // take timestamp
      send42 = true; // set button2 message flag
      if (curBtn2State == LOW) 
        {
        if (toggleOnButton2) 
          { // button in toggle state ?
        #ifdef ACTUATOR2
          actuator2status = !actuator2status; // toggle output
          digitalWrite(ACTUATOR2PIN, actuator2status);
          send17 = true; // set ACT2 / LED message flag
        #endif
        } 
        else
          if (TIMinterval > 0 && !timerOnButton2) 
            { // button in timer state ?
            timerOnButton2 = true; // start timer interval
            #ifdef ACTUATOR2
              actuator2status = HIGH; // switch on ACT2
              digitalWrite(ACTUATOR2PIN, actuator2status);
              send17 = true; // set ACT2 / LED message flag
            #endif
            }
        }
      lastBtn2State = curBtn2State;
      }
      
    // TIMER CHECK for button 2
    //
    if (TIMinterval > 0 && timerOnButton2) // =0 means no timer
      {
      if ( millis() - lastBtn2Press > TIMinterval*1000) 
        { // timer expired ?
        timerOnButton2 = false; // then end timer interval
        #ifdef ACTUATOR2
          actuator2status = LOW; // and switch off Actuator
          digitalWrite(ACTUATOR2PIN, actuator2status);
          send17 = true; // set ACT2 / LED message flag
        #endif
        }
      }
  #endif
} // END - checkbuttons()
