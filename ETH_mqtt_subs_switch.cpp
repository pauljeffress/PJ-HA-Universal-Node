//
// ======== mqtt_subs_switch()
//
// Switch based on each possible destDevID and action it... 
// This fn is called by mqtt_subs(), after it received a valid MQTT request and placed it in the
// following global variables, ready to be acted on here.
//      mes
//      destDevID
//      strPayload
// The action required may be as simple ase setting a sendXX flag so that next time dosends() is
// called, the Device's value is sent.

#include "a.h" // My global defines and extern variables to help multi file comilation.

void mqtt_subs_switch() {
  
int i; 

  switch(destDevID) {
    case 0:   // 000 - Uptime (Integer - ReadOnly)
      send0 = true;  // as this Device is ReadOnly, there is no need to check the mes.cmd as only a read makes sense.
                     // xxxx - Although, in RF_parseCmd() which is for RF Nodes, I do send an error if I get a wtite to a RO device.
      error = 0;      // xxxx - some of the scenarios below do not set error = 0 before breaking????? Maybe set it before switch?
      break;

    case 1:   // 001 - Periodic TX Interval (Integer - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send1 = true;
        error = 0;
        }
      else  // WRITE
        {
        if (TXinterval <10 && TXinterval !=0) TXinterval = 10; // minimum interval is 10 seconds, or 0 to turn it off.
        TXinterval = strPayload.toInt();
        if (setAck) send1 = true; // acknowledge message  
        }
      break;

    case 3:   // 003 - Version (String - ReadOnly)
      send3 = true;
      error =0;
      break;

    case 4:   // 004 - Battery Voltage (Real - ReadOnly)
      send4 = true;
      error = 0;
      break;

    case 5:   // 005 - ACK: read/set acknowledge message after a 'set' request (see void parseCmd() in node code) (Status Int - Read/Write)
       if (mes.cmd == 1) // READ
        {
        send5 = true;                      
        error = 0;
        }
      else  // WRITE
        {
        if (strPayload.toInt() == 1) setAck = 1;
        if (strPayload.toInt() == 0) setAck = 0;
        if (setAck) send5 = true; // acknowledge message
        }
      break;

    case 6:  // 006 - toggle: read/set toggle function on button press  (see note below under "The Button") (see void parseCmd() in node code) (Status Int - Read/Write))
      if (mes.cmd == 1) // READ
        {
        send6 = true;                      
        error = 0;
        }
      else  // WRITE
        {
        if (strPayload.toInt() == 1) toggleOnButton = 1;
        if (strPayload.toInt() == 0) toggleOnButton = 0;
        if (setAck) send6 = true; // acknowledge message
        }
      break;

    case 7: // 007 - read/set activation timer after button press in seconds, 0 means no timer (see void parseCmd() in node code) (Integer - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send7 = true;                      
        error = 0;
        }
      else  // WRITE
        {
        if (TIMinterval <5 && TIMinterval !=0) TIMinterval = 5; // minimum interval is 5 seconds, or 0 to turn it off.
        TIMinterval = strPayload.toInt(); 
        if (setAck) send7 = true; // acknowledge message 
        }
      break;

    case 11:   // 011 - Compiled Date: Gets embedded into code at compile time (string)(RO)
      send11 = true;
      error =0;
      break;

    case 12:   // 012 - Compiled Time: Gets embedded into code at compile time (string)(RO)
      send12 = true;
      error =0;
      break;
      

#ifdef ACTUATOR1
    case 16: // 016 - Binary Actuator 1 Output (Status Int - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send16 = true;                     
        error = 0;
        }
      else  // WRITE
        {
        if (strPayload.toInt() == 1) actuator1status = 1;
        if (strPayload.toInt() == 0) actuator1status = 0;
        #ifdef EXTENDEDACT1
          if (actuator1status) sendserialtoslave(EXTENDEDACTUATORON, 1); // turn on the associated extended actuator (i.e. LED)
          else sendserialtoslave(EXTENDEDACTUATOROFF, 1); // turn off the associated extended actuator (i.e. LED)
        #endif
        if (setAck) send16 = true; // acknowledge message 
        }
      break;
#endif

#ifdef ACTUATOR2
    case 17: // 017 - Binary Actuator 1 Output (Status Int - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send17 = true;                     
        error = 0;
        }
      else  // WRITE
        {
        if (strPayload.toInt() == 1) actuator2status = 1;
        if (strPayload.toInt() == 0) actuator2status = 0;
        #ifdef EXTENDEDACT2
          if (actuator2status) sendserialtoslave(EXTENDEDACTUATORON, 2); // turn on the associated extended actuator (i.e. LED)
          else sendserialtoslave(EXTENDEDACTUATOROFF, 2); // turn off the associated extended actuator (i.e. LED)
        #endif
        if (setAck) send17 = true; // acknowledge message
        }
      break;
#endif

#ifdef ACTUATOR3
    case 18: // 018 - Binary Actuator 3 Output (Status Int - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send18 = true;                     
        error = 0;
        }
      else  // WRITE
        {
        if (strPayload.toInt() == 1) actuator3status = 1;
        if (strPayload.toInt() == 0) actuator3status = 0;
        #ifdef EXTENDEDACT3
          if (actuator3status) sendserialtoslave(EXTENDEDACTUATORON, 3); // turn on the associated extended actuator (i.e. LED)
          else sendserialtoslave(EXTENDEDACTUATOROFF, 3); // turn off the associated extended actuator (i.e. LED)
        #endif
        if (setAck) send18 = true; // acknowledge message
        }
      break;
#endif

#ifdef ACTUATOR4
    case 19: // 019 - Binary Actuator 4 Output (Status Int - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send19 = true;                     
        error = 0;
        }
      else  // WRITE
        {
        if (strPayload.toInt() == 1) actuator4status = 1;
        if (strPayload.toInt() == 0) actuator4status = 0;
        #ifdef EXTENDEDACT4
          if (actuator4status) sendserialtoslave(EXTENDEDACTUATORON, 4); // turn on the associated extended actuator (i.e. LED)
          else sendserialtoslave(EXTENDEDACTUATOROFF, 4); // turn off the associated extended actuator (i.e. LED)
        #endif
        if (setAck) send19 = true; // acknowledge message
        }
      break;
#endif

#ifdef BUTTON1
  case 40: // 040 - Binary Sensor Input (Status Int - ReadOnly) Button1
    if (mes.cmd == 1) send40 = true;
    break;
#endif

#ifdef PIR1
  case 41: // 041 - Binary Sensor Input (Status Int - ReadOnly) PIR1
    if (mes.cmd == 1) send41 = true;
    break;
#endif

#ifdef BUTTON2
  case 42: // 042 - Binary Sensor Input (Status Int - ReadOnly) Button2
    if (mes.cmd == 1) send42 = true;
    break;
#endif

#ifdef PIR2
  case 43: // 043 - Binary Sensor Input (Status Int - ReadOnly) PIR2
    if (mes.cmd == 1) send43 = true;
    break; 
#endif

#ifdef MOTEINOWEATHER
  case 51: // 051 - pressure from MOTEINO Weather (Real - ReadOnly)
    if (mes.cmd == 1) send51 = true;
    break;
  case 54: // 054 - humidity from MOTEINO Weather (Real - ReadOnly)
    if (mes.cmd == 1) send54 = true;
    break;
  case 55: // 055 - temperature from MOTEINO Weather (Real - ReadOnly)
    if (mes.cmd == 1) send55 = true;
    break;
#endif

#ifdef TSL2651
  case 52: // 052 - visible light from TSL2651 (Real - ReadOnly)
    if (mes.cmd == 1) send52 = true;
    break;
  case 53: // 053 - IR light from TSL2651 (Real - ReadOnly)
    if (mes.cmd == 1) send53 = true;
    break;
#endif


// xxxx - add switch for XMASLIGHTS, R/W, send100 etc - see stuff in main ino.
//          see stuff in ETH_dosends();
//          also check this code is prob in RF_parseCmd() in case its asked of an RF Node

// xxxx - add switch for PIXELLEDSTRIP, R/W, send201 etc - see stuff in main ino.
//          see stuff in ETH_dosends();
//          also check this code is prob in RF_parseCmd() in case its asked of an RF Node


#ifdef PING1x
  case 64: // 064 - last dist sample (Int - ReadOnly)
    if (mes.cmd == 1) send64 = true;
    break;
  case 65: // 065 - min for detect (Int - Read/Write)
     if (mes.cmd == 1) // READ
        {
        send65 = true;
        error = 0;
        }
      else  // WRITE
        {
        ping1RangeMin = strPayload.toInt();
        if (ping1RangeMin <5) ping1RangeMin = 5; // minimum min is 5 cm.
        if (ping1RangeMin >190) ping1RangeMin = 190; // maximum min is 190 cm.   
        if (ping1RangeMin > ping1RangeMax) ping1RangeMin = ping1RangeMax - 10; // minimum diff is 10 cm.     
        if (setAck) send65 = true; // acknowledge message  
        }
      break;
  case 66: // 066 - max for detect (Int - Read/Write)
     if (mes.cmd == 1) // READ
        {
        send66 = true;
        error = 0;
        }
      else  // WRITE
        {
        ping1RangeMax = strPayload.toInt();
        if (ping1RangeMax <15) ping1RangeMax = 15; // minimum max is 15 cm.
        if (ping1RangeMax >200) ping1RangeMax = 200; // maximum min is 200 cm.   
        if (ping1RangeMax < ping1RangeMin) ping1RangeMax = ping1RangeMin + 10; // minimum diff is 10 cm.     
        if (setAck) send66 = true; // acknowledge message  
        }
      break;    
  case 67: // 067 - was there a detection  sample (Int - ReadOnly)
    if (mes.cmd == 1) send64 = true;
    break;    

#endif // PING1x

#ifdef RTC
  case 300: // 300: Unixtime - high 16 bits (Unixtime is a 32bit Arduino 'Long')
     if (mes.cmd == 1) // READ
        {
        send300 = true;
        error = 0;
        }
      else  // WRITE
        {
        rtcUnixTimeHigh = strPayload.toInt();
        if (rtcUnixTimeHigh <0) rtcUnixTimeHigh = 0; // don't let it be negative.
        // xxxx - add code to write this into the actual RTC.
        if (setAck) send300 = true; // acknowledge message  
        }
      break;
  case 301: // 301: Unixtime - low 16 bits (Unixtime is a 32bit Arduino 'Long')
     if (mes.cmd == 1) // READ
        {
        send301 = true;
        error = 0;
        }
      else  // WRITE
        {
        rtcUnixTimeLow = strPayload.toInt();
        if (rtcUnixTimeLow <0) rtcUnixTimeLow = 0; // don't let it be negative.
        // xxxx - add code to write this into the actual RTC.
        //if (ping1RangeMin >190) ping1RangeMin = 190; // maximum min is 190 cm.   
        if (setAck) send301 = true; // acknowledge message  
        }
      break; 
  case 302: // 302: date - year (integer)
     if (mes.cmd == 1) // READ
        {
        send302 = true;
        error = 0;
        } 
     else  // WRITE
        {
        rtcYear = strPayload.toInt();
        if (rtcYear < 1980) rtcYear = 1980; // don't let it be set stupidly into the past.
        if (rtcYear > 2100) rtcYear = 2100; // don't let it be set stupidly into the future. 
        // now update the clock with the new value.
        if (!rtcOk) // check if clock was ok at startup, only work on it if it was ok.
          {
          #ifdef DEBUGPJ2
            Serial.print("rtcOk = false, so rtc was either not found or was not running at startup");
            // xxxx - should trigger the send of an error MQTT msg here.
          #endif  
          }
        else // rtcOk was true, so go ahead and work with the clock.
          {
          now = rtc.now();     // get current time record from the RTC.
          #ifdef DEBUGPJ2
            Serial.print("setting RTC year to:");
            Serial.println(rtcYear);
            Serial.print("It was :");
            Serial.println(now.year());
          #endif
          rtc.adjust(DateTime(rtcYear, now.month(), now.day(), now.hour(), now.minute(), now.second()));     // set new data record back to the RTC.
          }
        if (setAck) send302 = true; // acknowledge message  
        } // end of WRITE
      break;
  case 303: // 303: date - month (integer)
     if (mes.cmd == 1) // READ
        {
        send303 = true;
        error = 0;
        }
      else  // WRITE
        {
        rtcMonth = strPayload.toInt();
        if (rtcMonth < 1) rtcMonth = 1; // can't be less than 1
        if (rtcMonth > 12) rtcMonth = 12; // can't be greater than 12  
        // now update the clock with the new value.
        if (!rtcOk) // check if clock was ok at startup, only work on it if it was ok.
          {
          #ifdef DEBUGPJ2
            Serial.print("rtcOk = false, so rtc was either not found or was not running at startup");
            // xxxx - should trigger the send of an error MQTT msg here.
          #endif  
          }
        else // rtcOk was true, so go ahead and work with the clock.
          {
          now = rtc.now();     // get current time record from the RTC.
          #ifdef DEBUGPJ2
            Serial.print("setting RTC Month to:");
            Serial.println(rtcMonth);
            Serial.print("It was :");
            Serial.println(now.month());
          #endif
          rtc.adjust(DateTime(now.year(), rtcMonth, now.day(), now.hour(), now.minute(), now.second()));     // set new data record back to the RTC.         
          }
        if (setAck) send303 = true; // acknowledge message  
        }
      break; 
  case 304: // 304: date - day (integer)
     if (mes.cmd == 1) // READ
        {
        send304 = true;
        error = 0;
        }
      else  // WRITE
        {
        rtcDay = strPayload.toInt();
        if (rtcDay < 1) rtcDay = 1; // can't be less than 1
        if (rtcDay > 31) rtcDay = 31; // can't be greater than 12  
        // now update the clock with the new value.
        if (!rtcOk) // check if clock was ok at startup, only work on it if it was ok.
          {
          #ifdef DEBUGPJ2
            Serial.print("rtcOk = false, so rtc was either not found or was not running at startup");
            // xxxx - should trigger the send of an error MQTT msg here.
          #endif  
          }
        else // rtcOk was true, so go ahead and work with the clock.
          {
          now = rtc.now();     // get current time record from the RTC.
          #ifdef DEBUGPJ2
            Serial.print("setting RTC Day to:");
            Serial.println(rtcDay);
            Serial.print("It was :");
            Serial.println(now.day());
          #endif
          rtc.adjust(DateTime(now.year(), now.month(), rtcDay, now.hour(), now.minute(), now.second()));     // set new data record back to the RTC.         
          }
        if (setAck) send304 = true; // acknowledge message  
        }
      break; 
  case 305: // 305: date - day of the week (integer) Sunday is first day of week.
     if (mes.cmd == 1) // READ
        {
        send305 = true;
        error = 0;
        }
      else  // WRITE
        {
        // xxxx - you can't WRITE to this value, its computed, send an error
        if (setAck) send305 = true; // acknowledge message  
        }
      break; 
  case 306: // 306: time - hour (integer) 24hr time
     if (mes.cmd == 1) // READ
        {
        send306 = true;
        error = 0;
        }
      else  // WRITE
        {
        rtcHour = strPayload.toInt();
        if (rtcHour < 0) rtcHour = 0; // can't be less than 0
        if (rtcHour > 23) rtcHour = 23; // can't be greater than 23   
        // now update the clock with the new value.
        if (!rtcOk) // check if clock was ok at startup, only work on it if it was ok.
          {
          #ifdef DEBUGPJ2
            Serial.print("rtcOk = false, so rtc was either not found or was not running at startup");
            // xxxx - should trigger the send of an error MQTT msg here.
          #endif  
          }
        else // rtcOk was true, so go ahead and work with the clock.
          {
          now = rtc.now();     // get current time record from the RTC.
          #ifdef DEBUGPJ2
            Serial.print("setting RTC Hour to:");
            Serial.println(rtcHour);
            Serial.print("It was :");
            Serial.println(now.hour());
          #endif
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), rtcHour, now.minute(), now.second()));     // set new data record back to the RTC.         
          }
        if (setAck) send306 = true; // acknowledge message  
        }
      break; 
  case 307: // 307: time - minute (integer)
     if (mes.cmd == 1) // READ
        {
        send307 = true;
        error = 0;
        }
      else  // WRITE
        {
        rtcMinute = strPayload.toInt();
        if (rtcMinute < 0) rtcMinute = 0; // can't be less than 0
        if (rtcMinute > 59) rtcMinute = 59; // can't be greater than 59  
        // now update the clock with the new value.
        if (!rtcOk) // check if clock was ok at startup, only work on it if it was ok.
          {
          #ifdef DEBUGPJ2
            Serial.print("rtcOk = false, so rtc was either not found or was not running at startup");
            // xxxx - should trigger the send of an error MQTT msg here.
          #endif  
          }
        else // rtcOk was true, so go ahead and work with the clock.
          {
          now = rtc.now();     // get current time record from the RTC.
          #ifdef DEBUGPJ2
            Serial.print("setting RTC Minute to:");
            Serial.println(rtcMinute);
            Serial.print("It was :");
            Serial.println(now.minute());
          #endif
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), rtcMinute, now.second()));     // set new data record back to the RTC.         
          }
        if (setAck) send307 = true; // acknowledge message  
        }
      break; 
  case 308: // 308: time - second (integer)
     if (mes.cmd == 1) // READ
        {
        send308 = true;
        error = 0;
        }
      else  // WRITE
        {
        rtcSecond = strPayload.toInt();
        if (rtcSecond < 0) rtcSecond = 0; // can't be less than 0
        if (rtcSecond > 59) rtcSecond = 59; // can't be greater than 59  
        // now update the clock with the new value.
        if (!rtcOk) // check if clock was ok at startup, only work on it if it was ok.
          {
          #ifdef DEBUGPJ2
            Serial.print("rtcOk = false, so rtc was either not found or was not running at startup");
            // xxxx - should trigger the send of an error MQTT msg here.
          #endif  
          }
        else // rtcOk was true, so go ahead and work with the clock.
          {
          now = rtc.now();     // get current time record from the RTC.
          #ifdef DEBUGPJ2
            Serial.print("setting RTC Second to:");
            Serial.println(rtcSecond);
            Serial.print("It was :");
            Serial.println(now.second());
          #endif
          rtc.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), rtcSecond));     // set new data record back to the RTC.         
          }
        if (setAck) send308 = true; // acknowledge message  
        }
      break;
#endif // RTC

#ifdef LCDGENERIC
  case 320: // 320 - Display String (R/W) (max 20 char string/array) Although only makes sense to WRITE.
     if (mes.cmd == 1) // READ
        {
        send320 = true;
        error = 0;
        }
      else  // WRITE
        {
        //lcdGenStr = strPayload;
        for (i = 0; i <= strPayload.length(); i++)
          {
          lcdGenStr[i] = strPayload[i]; 
          //inString += (char)inChar; 
          }
        if (setAck) send320 = true; // acknowledge message  
        }
      break;
  case 321: // 321 - String Length (R/W)  (integer) Although only makes sense to WRITE.
     if (mes.cmd == 1) // READ
        {
        send321 = true;
        error = 0;
        }
      else  // WRITE
        {
        lcdGenStrLen = strPayload.toInt();
        if (lcdGenStrLen < 0) lcdGenStrLen = 0; // can't be less than 0
        if (lcdGenStrLen > LCDGENERIC_XMAX) lcdGenStrLen = LCDGENERIC_XMAX; // can't be greater than width of screen.  
        if (setAck) send321 = true; // acknowledge message  
        }
      break;
  case 322: // 322 - X-Coord (R/W)  (integer) Although only makes sense to WRITE.
     if (mes.cmd == 1) // READ
        {
        send322 = true;
        error = 0;
        }
      else  // WRITE
        {
        lcdGenXPos = strPayload.toInt();
        if (lcdGenXPos < 0) lcdGenXPos = 0; // can't be less than 0
        if (lcdGenXPos > LCDGENERIC_XMAX) lcdGenXPos = LCDGENERIC_XMAX; // can't be greater than width of screen.  
        if (setAck) send322 = true; // acknowledge message  
        }
      break;
  case 323: // 323 - Y-Coord (R/W)  (integer) Although only makes sense to WRITE.
     if (mes.cmd == 1) // READ
        {
        send323 = true;
        error = 0;
        }
      else  // WRITE
        {
        lcdGenYPos = strPayload.toInt();
        if (lcdGenYPos < 0) lcdGenYPos = 0; // can't be less than 0
        if (lcdGenYPos > LCDGENERIC_YMAX) lcdGenYPos = LCDGENERIC_YMAX; // can't be greater than height of screen.  
        if (setAck) send323 = true; // acknowledge message  
        }
      break;
  case 324: // 324 - DisplayNow command - (WO) (integer) only when a node gets a true/1 written to this 
            // DEV does it use info in 320-323 and write to LCD.  It only makes sense to WRITE a true/1 
            // to this. The node resets it to false/0 after it has written to the LCD.
     if (mes.cmd == 1) // READ
        {
        send324 = true;
        error = 0;
        }
      else  // WRITE
        {
        if (strPayload.toInt() == 1)  // make sure that what was written was a 1, just for formaility sake.
                                      // and if so, use DEV320-323 to write string to screen.
          {
          // write to screen  
          writeLCDGEN();
          //checkPINGs();
          if (setAck) send323 = true; // acknowledge message  
          }
        else 
          {
          //  xxxx - send an error (dev09x) back, that the payload of the dev324 WRITE was not 1 and anything other than 1 is invalid. 
          if (setAck) send323 = true; // acknowledge message   
          }
        }
      break;

#endif // LCDGENERIC

#ifdef BEEPER
  case 340: // 340 - Play beep X now - (WO) (integer).  X is used as a lookup on the node and 
            // the node thenplays that beep.  No other control over the sound is available from 
            // MQTT side. The beep is defined on the node, this instruction just says play it.
     if (mes.cmd == 1) // READ
        {
        send340 = true;
        error = 0;
        }
      else  // WRITE
        {
        beeperCommand = strPayload.toInt();
        if (beeperCommand < 0) beeperCommand = 0; // can't be less than 0
        if (beeperCommand > (BEEPNUMMAX-1)) beeperCommand = BEEPNUMMAX - 1; // can't be greater than max num notes defined on this node itself.
        // play note  
        tone(BEEPERPIN, beeperNotes[beeperCommand], beeperDurations[beeperCommand]);
        if (setAck) send340 = true; // acknowledge message  
        }
      break;
#endif //BEEPER

#ifdef FINGER
  case 350: // FPS lookup occurred and the result is ...(RO) (integer). 
            // Contains the result of the last succesful finger detection and lookup.
            // See my documentation for this Dev for suitable values that may be here.
     if (mes.cmd == 1) // READ
        {
        send350 = true;
        error = 0;
        }
      else  // WRITE - invalid, this is a RO device type.
        {
        #ifdef DEBUGPJ2
          Serial.println("mqtt_subs_switch(): sending dev93 - error message you tried to write to a RO device");
        #endif
        send93 = true;  // send appropriate error message
        if (setAck) send350 = true; // acknowledge message  
        }
      break;
#endif // FINGER

#ifdef EXTVAR40X
  case 400: // 400     - TestExtVar - purely for testing, does not represent any real external variable. (Real - R/W)
     if (mes.cmd == 1) // READ
        {
        send400 = true;
        error = 0;
        }
     else // WRITE
        {
        extVar400 = strPayload.toFloat();
        if (setAck) send400 = true; // acknowledge message
        }
    break;
  case 401: // 401     - MasterLEDBrightness (0-255) - if 0, tells DUE to turn off whole display. 1..255 means turn it on and set master brightness to X. (Real - R/W)
     if (mes.cmd == 1) // READ
        {
        send401 = true;
        error = 0;
        }
     else // WRITE
        {
        extVar401 = strPayload.toFloat();
        if (setAck) send401 = true; // acknowledge message
        }
    break;

#endif // EXTVAR40X


  default:
      // We really should not get to here unless I forgot to cater for a particular DevID OR that DevID is not supported on this Node???
      Serial.println(); // make sure I get a new line before printing below messages.
      Serial.println("mqtt_subs_switch() - error = 11 (Somehow we have go this far but the DevID has not been SWITCH'd! - is it even supported on this node?");
      error = 11;
      mqtt_subs_senderror(); 
  }
} // END - mqtt_subs_switch()

