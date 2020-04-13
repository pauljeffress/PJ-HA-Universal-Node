//
// ================ checkPINGs()
// Cause any PINGs to take a sample.
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef PING1x

bool checkPINGs()
{
  #ifdef DEBUGPJx
	  CONSOLE_PORT.println("S: CheckPINGs");
  #endif

//xxxx - temp commented out all of the below as my sensor was going off all the time and causing probs while 
//       I was debugging other stuff, need to fix this later.

if (((millis() - ping1OnMillis) / 1000) > PING1HOLDTIME) // i.e. holdtime from last detection has passed
  {
	long ping1Duration;	// Proximity - Duration used to calculate distance
	/* The following trigPin/echoPin cycle is used to determine the
	distance of the nearest object by bouncing soundwaves off of it. */
	digitalWrite(PING1TRIGGERPIN, LOW);
	delayMicroseconds(2);

	digitalWrite(PING1TRIGGERPIN, HIGH);
	delayMicroseconds(10);

	digitalWrite(PING1TRIGGERPIN, LOW);
	ping1Duration = pulseIn(PING1ECHOPIN, HIGH);

	//Calculate the distance (in cm) based on the speed of sound.
	ping1Distance = ping1Duration / 58.2;

    #ifdef DEBUGPJx
	    CONSOLE_PORT.print("F: CheckApproach - Dist = ");
	    CONSOLE_PORT.println(ping1Distance);
    #endif

	if (ping1Distance >= ping1RangeMax || ping1Distance <= ping1RangeMin)
		{
		// We do NOT have a detection within range.
		ping1Detect = false;
		#ifdef DEBUGPJx
			CONSOLE_PORT.println("Nothing detected in proximity range."); 
		#endif
		//Delay 50ms before next reading otherwise PING becomes unstable.
		delay(50);
		return false;
		}
	else 
		{
		// We DO have a detection within range.
		ping1Detect = true;
    ping1OnMillis = millis();
		send67 = true;	// proactively send an MQTT update if there is a detection.
		send64 = true;		// and send the detection distance with it.
		#ifdef DEBUGPJ2
			CONSOLE_PORT.print("Object has been detected at range - ");
			CONSOLE_PORT.print(ping1Distance);
			CONSOLE_PORT.println("cm");
		#endif
		//Delay 50ms before next reading otherwise PING becomes unstable.
		delay(50);
		return true;
		}
  }
       
}  // END - checkPINGs()
#endif // PING1 





