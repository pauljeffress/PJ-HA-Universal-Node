//
// ================ SWITCH Functions ===================
// 
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef SWITCH1 or SWITCH2 




//
// ================ checkswitches()
// Checks if the physical SWITCHes on this node have been changed state and if so
// send an update.
//

void checkswitches() {
  int tempSwitchRead;  
  #ifdef SWITCH1
    tempSwitchRead = digitalRead(SWITCH1PIN);
    if (curSwitch1State != tempSwitchRead)   // i.e. Switch has changed
        { 
        curSwitch1State = tempSwitchRead;
        send44 = true; // set SWITCH1 message flag
        }
  #endif

  #ifdef SWITCH2
    tempSwitchRead = digitalRead(SWITCH2PIN);
    if (curSwitch2State != tempSwitchRead)   // i.e. Switch has changed
        { 
        curSwitch2State = tempSwitchRead;
        send45 = true; // set SWITCH2 message flag
        }
#endif    
} // END - checkswitches()

#endif
