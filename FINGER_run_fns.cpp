//
// Misc FINGER RUN MODE State Machine related functions
//
// This is the guts of the State Machine that is started when the PING1 detects someone..or something!
// Much of this originated with my previous FPDS project.

#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef FINGER

// FPS Run States
#define FPS_RUN_START		    0	// do the one off tasks that need to happen when we transition to FPS Run Mode.
#define FPS_RUN_START_IDENTIFY	1
#define FPS_RUN_DO_IDENTIFY	    2
#define FPS_RUN_DOOR_UNLOCK     3
#define FPS_RUN_DOOR_UNLOCKED   4
#define FPS_RUN_DOOR_LOCK       5
#define	FPS_RUN_EXIT    		99	// do a cleanup.

// Other defines
#define FPS_IDENTIFY_TIME		60	// amount of time in seconds to keep checking for a user during FPSRunIdentify
#define FPS_DOOR_UNLOCKED_TIME 	20		// Once the door is unlocked, keep it unlocked for this period (in Seconds)

// Variables
int FPSRunState = FPS_RUN_START;	// ensure the FPS Run state starts correctly the very first time this State Machine is started.
int FPSIdentifyStatus = 999;   // 999 only there to help me debug.
unsigned long FPSRunIdentifyTimer;  // determines when to stop trying to identify a finger after a PING1 detection.
unsigned long FPSRunUnlockedTimer;  // determines when to re lock the door after it has been unlocked.

// ================ FPSRunModeStateMachine()
// Main State Machine for the FPS when in RUN MODE.

void FPSRunModeSM()
{
   #ifdef DEBUGPJx
      CONSOLE_PORT.println("FPSRunModeSM switcher");
   #endif

   switch (FPSRunState)			//Pick a state and execute it.
		{
		case FPS_RUN_START:				// Do first things as we enter Run mode.
			FPSRunStart();
			break;
		case FPS_RUN_START_IDENTIFY:
			FPSRunStartIdentify();
			break;
		case FPS_RUN_DO_IDENTIFY:
			FPSRunDoIdentify();
			break;
		case FPS_RUN_DOOR_UNLOCK:
			FPSRunDoorUnlocker();
			break;
		case FPS_RUN_DOOR_UNLOCKED:
			FPSRunDoorUnlocked();
			break;		
    case FPS_RUN_DOOR_LOCK:
			FPSRunDoorLocker();
			break;            
		case FPS_RUN_EXIT:				// Do last Run steps before switching to DB Mode.
			FPSRunExit();
			break;
		default:
			#ifdef DEBUGPJ2
				CONSOLE_PORT.print("Unknown FPS Run State: ");
				CONSOLE_PORT.println(FPSRunState);
			#endif
            #ifdef LCDGEN
				localwriteLCDGEN(0, 0, "Unknown FPS Run State");
			#endif
			#ifdef LCDNEXTION_FPS
				writeLCDNEXTION_FPS_instruction("Unknown FPS Run State - Halting Mega");
			#endif
			while (1)	// if we hit this state, we have a problem, so just stay here forever.
			{
			delay(1);
			}
			break;
		}
} // END - FPSRunModeSM()


void FPSRunStart()
{
	#ifdef DEBUGPJ2
		CONSOLE_PORT.println("F: FPSRunStart");
	#endif
	FPSTurnFootlightOn();

  #ifdef LCDGEN
  	localwriteLCDGEN(0, 4, "                    ");  // erase bottom of screen
  	localwriteLCDGEN(0, 5, "                    ");  // erase bottom of screen
  	localwriteLCDGEN(0, 6, "                    ");  // erase bottom of screen
  	localwriteLCDGEN(0, 7, "                    ");  // erase bottom of screen
  	localwriteLCDGEN(0, 4, "FPS Run Mode");
  #endif //LCDGEN

  FPSRunIdentifyTimer = millis() + (FPS_IDENTIFY_TIME * 1000L);	// set timer to a time a number of seconds in the future
	//FPSRunIdentifyTimer = 70000;
	#ifdef DEBUGPJx
        CONSOLE_PORT.println("-----");
        CONSOLE_PORT.print("                millis() = "); CONSOLE_PORT.println(millis());
        CONSOLE_PORT.print("FPS_IDENTIFY_TIME * 1000 = "); CONSOLE_PORT.println(FPS_IDENTIFY_TIME * 1000L);
        CONSOLE_PORT.print("     FPSRunIdentifyTimer = "); CONSOLE_PORT.println(FPSRunIdentifyTimer);
        CONSOLE_PORT.println("-----");
        //while(1);
     #endif
  FPSRunState = FPS_RUN_START_IDENTIFY;  // Transition to next state.
} // END - FPSRunStart()


void FPSRunStartIdentify()
{
	#ifdef DEBUGPJ2
		CONSOLE_PORT.println("F: FPSRunStartIdentify");
	#endif
  
  #ifdef LCDGEN
  	localwriteLCDGEN(0, 5, "                    ");  // erase bottom of screen
  	localwriteLCDGEN(0, 6, "                    ");  // erase bottom of screen	
  	localwriteLCDGEN(0, 5, "Scan your finger now");
  #endif //LCDGEN

  #ifdef LCDNEXTION_FPS
  	writeLCDNEXTION_FPS_instruction("Place finger on scanner.");
  #endif

	#ifdef BEEPER
  		tone(BEEPERPIN, beeperNotes[0], beeperDurations[0]);	// play tone to get user attention, to place finger.
	#endif // BEEPER
	FPSRunState = FPS_RUN_DO_IDENTIFY;  // Transition to next state.
} // END - FPSRunStartIdentify()


void FPSRunDoIdentify()
{
	#ifdef DEBUGPJ2
		CONSOLE_PORT.println("F: FPSRunDoIdentify");
	#endif

	if (millis() > FPSRunIdentifyTimer)	// i.e. timer has expired and we still have not made a positive identification...
		{
		#ifdef DEBUGPJ2
        CONSOLE_PORT.println("FPSRunIdentifyTimer expired");
        //while (1);
    #endif
    FPSRunState = FPS_RUN_EXIT;  // Transition to next state.	
		return;	// get out of this function right now.
		}

	FPSIdentifyStatus = FPSgetFingerprintID();	// try and get a fingerprint and if so, try to identify it.

#define FPS_NO_FINGER_DETECT 990 //see return codes above
#define FPS_FINGER_NO_MATCH 991 //see return codes above
#define FPS_OTHER_ERROR 992 //see return codes above
  
	switch (FPSIdentifyStatus) {
    case FPS_NO_FINGER_DETECT:
      FPSRunState = FPS_RUN_DO_IDENTIFY;  // Transition to next state.
      break;
    case FPS_FINGER_NO_MATCH:
      #ifdef LCDGEN
	  	localwriteLCDGEN(0, 6, "Finger but no match.");
	  #endif
	  #ifdef LCDNEXTION_FPS
	  	writeLCDNEXTION_FPS_instruction("No Match");
	  #endif
      #ifdef BEEPER
        tone(BEEPERPIN, beeperNotes[1], beeperDurations[1]);  // play tone to get user attention, FAIL.
      #endif // BEEPER
      send350 = true; // send an upstream notification, it will contain the error code.
      FPSRunState = FPS_RUN_START_IDENTIFY;  // Transition to next state.      
      break;
    case FPS_OTHER_ERROR:
      #ifdef LCDGEN
	  	localwriteLCDGEN(0, 7, "FPS - Other Error Occured.");
	  #endif
	  #ifdef LCDNEXTION_FPS
	  	writeLCDNEXTION_FPS_instruction("FPS - Other Error Occured.");
	  #endif	  
      #ifdef BEEPER
        tone(BEEPERPIN, beeperNotes[1], beeperDurations[1]);  // play tone to get user attention, FAIL.
      #endif // BEEPER
      send350 = true; // send an upstream notification, it will contain the error code.
      FPSRunState = FPS_RUN_START_IDENTIFY;  // Transition to next state.      
      break;
    default: // we should only get to here if we got a match ID back.

      #ifdef LCDGEN
	  localwriteLCDGEN(0, 5, "Identity Verified OK ");
	  #endif
	  #ifdef LCDNEXTION_FPS
	  	writeLCDNEXTION_FPS_instruction("Identity Verified OK ");
	  #endif
      #ifdef BEEPER
        tone(BEEPERPIN, beeperNotes[3], beeperDurations[3]);  // play tone to get user attention, SUCCESS.
      #endif // BEEPER
      send350 = true; // send an upstream notification, it will contain identity number.
      FPSRunState = FPS_RUN_DOOR_UNLOCK;  // Transition to next state.
 } // END of switch statement.

} // END - FPSRunDoIdentify()


void FPSRunDoorUnlocker()
{
	FPSUnlockDoor(); // Unlock the door.
	FPSRunUnlockedTimer = millis() + (FPS_DOOR_UNLOCKED_TIME * 1000);	// set timer to a time a number of seconds in the future
	#ifdef LCDGEN
		localwriteLCDGEN(0, 6, "Door Unlocked"); 
	#endif
	#ifdef LCDNEXTION_FPS
	  	writeLCDNEXTION_FPS_instruction("Door Unlocked");
	#endif		 
	FPSRunState = FPS_RUN_DOOR_UNLOCKED;	// Transition to next state.
}  // END - FPSDoorUnlocker()


void FPSRunDoorUnlocked()
{
	#ifdef DEBUGPJ2
		CONSOLE_PORT.println("F: FPSDoorUnlocked");
	#endif
	if (millis() > FPSRunUnlockedTimer)	// i.e. timer has expired, time to lock door again...
		{
		FPSRunState = FPS_RUN_DOOR_LOCK;  // Transition to next state.	
		}
	else
		{
		FPSRunState = FPS_RUN_DOOR_UNLOCKED;	// Not really required, but make sure we stay in this state until timer expires.	
		}	
}  // END - FPSDoorUnlocked()


void FPSRunDoorLocker()
{
	#ifdef DEBUGPJ2
		CONSOLE_PORT.println("F: FPSDoorLocker");
	#endif
  FPSLockDoor(); // Lock the door.
	#ifdef LCDGEN
		localwriteLCDGEN(0, 6, "Door Locked  ");  
	#endif
	#ifdef LCDNEXTION_FPS
	  	writeLCDNEXTION_FPS_instruction("Door Locked");
	#endif		
	FPSRunState = FPS_RUN_EXIT;	// Transition to next state.
}  // END - FPSDoorLocker()


void FPSRunExit()
{
	#ifdef DEBUGPJ2
		CONSOLE_PORT.println("F: FPSRunExit");
	#endif
  FPSTurnFootlightOff();  // don't need to worry about a timer, if we get to this state, just turn it off.
  #ifdef LCDGENERIC
  	localwriteLCDGEN(0, 4, "                     ");  // erase bottom of screen
  	localwriteLCDGEN(0, 5, "                     ");  // erase bottom of screen
  	localwriteLCDGEN(0, 6, "                     ");  // erase bottom of screen
  	localwriteLCDGEN(0, 7, "                     ");  // erase bottom of screen
  #endif
  #ifdef LCDNEXTION_FPS
	writeLCDNEXTION_FPS_instruction("Ready");
  #endif	  
  FPSRunState = FPS_RUN_START;	// Transition to next state. Ensure when we return to FPS Run Mode we start in start mode.
	EnableFPSRunModeStateMachine = false; // flag that we need to stop running FPS RUN MODE state machine.
} // END - FPSRunExit()

#endif FINGER
