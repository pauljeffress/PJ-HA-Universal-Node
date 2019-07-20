// ============== parseCmd()
// Analyse messages and execute commands received from gateway.
// Is only called just after a new Southbound RF message for this node has been received intact
// up in receiveData(). Operates on the recently received "mes" structure.
// The action required may be as simple ase setting a sendXX flag so that next time dosends() is
// called, the Device's value is sent.
//

#include "a.h" // My global defines and extern variables to help multi file comilation.

void parseCmd() { // parse messages received from the gateway
#ifdef DEBUGPJ2
  Serial.println("starting parseCmd()");
#endif

switch (mes.devID) // devID indicates device (sensor) type
  {
  case (0): // uptime
  if (mes.cmd == 1) send0 = true; // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;

  case (1): // polling interval in seconds
  if (mes.cmd == 0) { // cmd == 0 means write a value
    TXinterval = mes.intVal; // change interval to radio packet value
    if (TXinterval <10 && TXinterval !=0) TXinterval = 10; // minimum interval is 10 seconds
    if (setAck) send1 = true; // send message if required
    #ifdef DEBUG
      Serial.print("Setting interval to ");
      Serial.print(TXinterval);
      Serial.println(" seconds.");
    #endif
    }
  else send1 = true; // cmd == 1 is a read request, so send polling interval
  break;

  case (2): // signal strength
  if (mes.cmd == 1) send2 = true; // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;

  case (3): // software version
  #ifdef DEBUGPJ2
    Serial.println("parseCmd() case3 hit");
  #endif
  if (mes.cmd == 1) send3 = true; // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;

  case (4): // battery level
  if (mes.cmd == 1) send4 = true;  // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;

  case (5): // set ack status
  if (mes.cmd == 0) {
    if (mes.intVal == 0) setAck = false;
    if (mes.intVal == 1) setAck = true;
    if (setAck) send5 = true; // acknowledge message ?
  }
  else send5 = true; // read request means schedule a message
  break;

  case (6): // set toggle
  if (mes.cmd == 0) {
    if (mes.intVal == 0) toggleOnButton = false;
    if (mes.intVal == 1) toggleOnButton = true;
    if (setAck) send6 = true; // acknowledge message ?
  }
  else send6 = true; // read request means schedule a message
  break;

  case (7): // timer interval in seconds
  if (mes.cmd == 0) { // cmd == 0 means write a value
    TIMinterval = mes.intVal; // change interval
    if (TIMinterval <5 && TIMinterval !=0) TIMinterval = 5;
    if (setAck) send7 = true; // acknowledge message ?
  } 
  else send7 = true; // read request means schedule a message
  break;

  case (11): // Compiled Date: Gets embedded into code at compile time (string)(RO)
  #ifdef DEBUGPJ2
    Serial.println("parseCmd() case11 hit");
  #endif
  if (mes.cmd == 1) send11 = true; // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;

  case (12): // Compiled Time: Gets embedded into code at compile time (string)(RO)
  #ifdef DEBUGPJ2
    Serial.println("parseCmd() case12 hit");
    //delay(1000);
  #endif
  if (mes.cmd == 1) send12 = true; // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;

#ifdef ACTUATOR1
  case (16): // Actuator 1
  if (mes.cmd == 0) { // cmd == 0 means write
    if(mes.intVal == 0 || mes.intVal == 1) {
      actuator1status = mes.intVal;
      // digitalWrite(ACTUATOR1PIN, actuator1status);
      if (setAck) send16 = true; // acknowledge message ?
      // #ifdef DEBUGPJ2
      //   Serial.print("Set LED to ");
      //   Serial.println(actuator1status);
      // #endif
    }
  }
  else send16 = true; // cmd == 1 means read
  break;
#endif // ACTUATOR1

#ifdef ACTUATOR2
  case (17): // Actuator 2
  if (mes.cmd == 0) { // cmd == 0 means write
    if(mes.intVal == 0 || mes.intVal == 1) {
      actuator2status = mes.intVal;
      // digitalWrite(ACTUATOR2PIN, actuator2status);
      if (setAck) send17 = true; // acknowledge message ?
      // #ifdef DEBUGPJ2
      //   Serial.print("Set LED to ");
      //   Serial.println(actuator2status);
      // #endif
    }
  }
  else send17 = true; // cmd == 1 means read
  break;
#endif // ACTUATOR2

#ifdef ACTUATOR3
  case (18): // Actuator 3
  if (mes.cmd == 0) { // cmd == 0 means write
    if(mes.intVal == 0 || mes.intVal == 1) {
      actuator3status = mes.intVal;
      // digitalWrite(ACTUATOR3PIN, actuator3status);
      if (setAck) send18 = true; // acknowledge message ?
      // #ifdef DEBUGPJ2
      //   Serial.print("Set LED to ");
      //   Serial.println(actuator3status);
      // #endif
    }
  }
  else send18 = true; // cmd == 1 means read
  break;
#endif // ACTUATOR3

#ifdef ACTUATOR4
  case (19): // Actuator 4
  if (mes.cmd == 0) { // cmd == 0 means write
    if(mes.intVal == 0 || mes.intVal == 1) {
      actuator4status = mes.intVal;
      // digitalWrite(ACTUATOR4PIN, actuator4status);
      if (setAck) send19 = true; // acknowledge message ?
      // #ifdef DEBUGPJ2
      //   Serial.print("Set LED to ");
      //   Serial.println(actuator4status);
      // #endif
    }
  }
  else send19 = true; // cmd == 1 means read
  break;
#endif // ACTUATOR4

#ifdef BUTTON1
  case (40): // binary input
  if (mes.cmd == 1) send40 = true; // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;
#endif // BUTTON1

#ifdef BUTTON2
  case (42): // binary input
  if (mes.cmd == 1) send42 = true; // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;
#endif // BUTTON2

#ifdef PIR1
  case (41): // PIR binary input
  if (mes.cmd == 1) send41 = true;  // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;
#endif // PIR1

#ifdef PIR2
  case (43): // PIR binary input
  if (mes.cmd == 1) send43 = true;  // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;
#endif // PIR2

#ifdef SWITCH1
  case (44): // binary input
  if (mes.cmd == 1) send44 = true; // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;
#endif // SWITCH1

#ifdef SWITCH2
  case (45): // binary input
  if (mes.cmd == 1) send45 = true; // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;
#endif // SWITCH2

#ifdef DHTSENSOR
  case (48): // temperature
  if (mes.cmd == 1) send48 = true;  // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;

  case (49): // humidity
  if (mes.cmd == 1) send49 = true;  // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;
#endif

#ifdef DS18
  case (50): // temperature
  if (mes.cmd == 1) send50 = true;  // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;
#endif

#ifdef SOILMOISTURE1
  case (68): // 1st soil sensor
  if (mes.cmd == 1) send68 = true;  // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;
#endif

#ifdef SOILMOISTURE2
  case (69): // 2nd soil sensor
  if (mes.cmd == 1) send69 = true;  // cmd == 1 means read
  else send93 = true;  // tried to write to RO dev
  break;
#endif

  #ifdef XMASLIGHTS
  case (100): // Xmas Lights Mode Value
  if (mes.cmd == 0) { // cmd == 0 means write
    if(mes.intVal >= 0 || mes.intVal <= 31) {   // test for correct value thats in range.
      XmasLightsMode = mes.intVal;
      #ifdef DEBUG
        Serial.println("calling setXmasLightsMode()");
      #endif
      setXmasLightsMode();
      if (setAck) send100 = true; // acknowledge message ?
    }
  }
  else send100 = true; // cmd == 1 means read
  break;
  #endif  // XMASLIGHTS

  
  #ifdef LEDSTRIP
  // insert code here for each DevID this node should respond to.
    case 200:   // 200: 1st RGB LED Strip Type value - 0 = DUMB, 1 = PIXEL, no other value is valid. (see below for explanation)  (Integer - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send200 = true;
        }
      else  // WRITE
        {
        if(mes.intVal >= 0 || mes.intVal <= 1)    // test for correct value thats in range.
          {
            #ifdef LEDSTRIPS_REMOTE
              current200 = mes.intVal;
            #else
              LEDStrip1Type = mes.intVal;
            #endif
            if (setAck) send200 = true; // acknowledge message ?
          }     
        }
      break;

    case 201:   // 201: 1st RGB LED Strip RED value 0-255 only, no other value is valid. (Integer - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send201 = true;
        }
      else  // WRITE
        {
        if(mes.intVal >= 0 || mes.intVal <= 255)    // test for correct value thats in range.
          {
          if (current291 != STATIC_ONE_COLOUR) // if we were not in this mode prior to this message arriving....
            {
              current291 = STATIC_ONE_COLOUR; // ... then change into this mode....
              send291 = true;               // ... and make sure upstream controller (Home Assistant) knows and updates any GUI etc
            }
            #ifdef LEDSTRIPS_REMOTE
              current201 = mes.intVal; // we only need to do this if we have remote LED Strips  
            #else
              LEDStrip1RedValue = mes.intVal;
              setStaticOneColourLEDStrip(1);  // update the LED strip. 
            #endif
            if (setAck) send201 = true; // acknowledge message ?
          } 
        else send95 = true;  // Value to be WRITTEN is out of range.      
        }
      break;

    case 202:   // 202: 1st RGB LED Strip GREEN value 0-255 only, no other value is valid. (Integer - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send202 = true;
        }
      else  // WRITE
        {
        if(mes.intVal >= 0 || mes.intVal <= 255)    // test for correct value thats in range.
          {
          if (current291 != STATIC_ONE_COLOUR) // if we were not in this mode prior to this message arriving....
            {
              current291 = STATIC_ONE_COLOUR; // ... then change into this mode....
              send291 = true;               // ... and make sure upstream controller (Home Assistant) knows and updates any GUI etc
            }
            #ifdef LEDSTRIPS_REMOTE
              current202 = mes.intVal; // we only need to do this if we have remote LED Strips  
            #else
              LEDStrip1GreenValue = mes.intVal;
              setStaticOneColourLEDStrip(1);  // update the LED strip
            #endif
            if (setAck) send202 = true; // acknowledge message ?
          }
        else send95 = true;  // Value to be WRITTEN is out of range.       
        }
      break;

    case 203:   // 203: 1st RGB LED Strip BLUE value 0-255 only, no other value is valid. (Integer - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send203 = true;
        }
      else  // WRITE
        {
        if(mes.intVal >= 0 || mes.intVal <= 255)    // test for correct value thats in range.
          {
          if (current291 != STATIC_ONE_COLOUR) // if we were not in this mode prior to this message arriving....
            {
              current291 = STATIC_ONE_COLOUR; // ... then change into this mode....
              send291 = true;               // ... and make sure upstream controller (Home Assistant) knows and updates any GUI etc
            }
            #ifdef LEDSTRIPS_REMOTE
              current203 = mes.intVal; // we only need to do this if we have remote LED Strips  
            #else
              LEDStrip1BlueValue = mes.intVal;
              setStaticOneColourLEDStrip(1);  // update the LED strip
            #endif
            if (setAck) send203 = true; // acknowledge message ?
          } 
        else send95 = true;  // Value to be WRITTEN is out of range.      
        }
      break;

    case 204:   // 204: 1st RGB LED Strip BRIGHTNESS value - 0-255 only, no other value is valid. (Integer - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send204 = true;
        }
      else  // WRITE
        {
        if(mes.intVal >= 0 || mes.intVal <= 255)    // test for correct value thats in range.
          {
          if (current291 != STATIC_ONE_COLOUR) // if we were not in this mode prior to this message arriving....
            {
              current291 = STATIC_ONE_COLOUR; // ... then change into this mode....
              send291 = true;               // ... and make sure upstream controller (Home Assistant) knows and updates any GUI etc
            }
            #ifdef LEDSTRIPS_REMOTE
              current204 = mes.intVal; // we only need to do this if we have remote LED Strips  
            #else
              LEDStrip1BrightnessValue = mes.intVal;
              setStaticOneColourLEDStrip(1);  // update the LED strip
                  #ifdef RMT_PWR
                    if(LEDStrip1BrightnessValue == 0) RMT_PWROff(); // When Brightness set to 0, no mater what mode
                                                                // the LED strip is in, cut the power. We don't
                                                                // want the remote PSU to stay on when brightness = 0.
                  #endif
            #endif
            if (setAck) send204 = true; // acknowledge message ?
          } 
        else send95 = true;  // Value to be WRITTEN is out of range.      
        }
      break;

      case 221:   // 221: 1st RGB PIXEL LED Strip - Static Pattern Selector (see below for explanation)  (Integer - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send221 = true;
        }
      else  // WRITE
        {
        if(mes.intVal >= 0 || mes.intVal <= 255)    // test for correct value thats in range.
          {
            if (current291 != STATIC_PATTERN) // if we were not in this mode prior to this message arriving....
            {
              current291 = STATIC_PATTERN; // ... then change into this mode....
              send291 = true;               // ... and make sure upstream controller (Home Assistant) knows and updates any GUI etc
            }
            #ifdef LEDSTRIPS_REMOTE
              current221 = mes.intVal; // we only need to do this if we have remote LED Strips  
            #else
              current221 = mes.intVal;
              setStaticPatternLEDStripMode(1,current221);
            #endif
            if (setAck) send221 = true; // acknowledge message ?
          }     
        else send95 = true;  // Value to be WRITTEN is out of range.
        }
      break;

       case 231:   // 231: 1st RGB PIXEL LED Strip - Dynamic Pattern Selector (see below for explanation)  (Integer - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send231 = true;
        }
      else  // WRITE
        {
          if (current291 != DYNAMIC_PATTERN) // if we were not in this mode prior to this message arriving....
          {
            current291 = DYNAMIC_PATTERN; // ... then change into this mode....
            send291 = true;               // ... and make sure upstream controller (Home Assistant) knows and updates any GUI etc
          }
          if(mes.intVal >= 0 || mes.intVal <= 255)    // test for correct value thats in range.
          {
            #ifdef LEDSTRIPS_REMOTE
              current231 = mes.intVal; // we only need to do this if we have remote LED Strips  
            #else
              current231 = mes.intVal;
              setDynamicPatternLEDStripMode(1,current231);
            #endif
            if (setAck) send231 = true; // acknowledge message ?
          }
        else send95 = true;  // Value to be WRITTEN is out of range.     
        }
      break;

      case 291:   // 291: Is Led Strip 1 in STATIC ONE COLOUR, STATIC PATTERN or DYNAMIC PATTERN mode? (Integer - Read/Write)
      if (mes.cmd == 1) // READ
        {
        send291 = true;
        }
      else  // WRITE
        {
        if(mes.intVal == STATIC_ONE_COLOUR || mes.intVal == STATIC_PATTERN || mes.intVal == DYNAMIC_PATTERN)    // test for correct value thats in range.
          {
            #ifdef LEDSTRIPS_REMOTE
              current291 = mes.intVal; // we only need to do this if we have remote LED Strips  
            #else
              current291 = mes.intVal;
              switch (current291) // devID indicates device (sensor) type
                {
                case (STATIC_ONE_COLOUR): 
                  setStaticOneColourLEDStrip(1);
                  break;
                case (STATIC_PATTERN): 
                  setStaticPatternLEDStripMode(1,current221);
                  break;
                case (DYNAMIC_PATTERN): 
                  setDynamicPatternLEDStripMode(1,current231);
                  break;                
                }
            #endif
            if (setAck) send291 = true; // acknowledge message ?
          }
        else send95 = true;  // Value to be WRITTEN is out of range.     
        }
      break;


  case 299: // 299:  Offload to subordinate MCU via Serial  0 = no, 1 = yes   (Integer - Read Only)
    if (mes.cmd == 1) send299 = true; // cmd == 1 means read
    else send93 = true;  // tried to write to RO dev
    break;

  #endif  // LEDSTRIP

#ifdef EXTVAR40X
  case 400: // 400     - TestExtVar - purely for testing, does not represent any real external variable. (Real - R/W)
    if (mes.cmd == 0)  // WRITE
      {
      extVar400 = mes.fltintVal; 
      if (setAck) send400 = true; // send message if required
      #ifdef DEBUGPJ2
        Serial.print("Setting extVar400 to ");
        Serial.println(extVar400);
      #endif
      }
    else // READ
      send400 = true; 
    break;
  case 401: // 401     - MasterLEDBrightness (0-255) - if 0, tells DUE to turn off whole display. 1..255 means turn it on and set master brightness to X. (Real - R/W)
    if (mes.cmd == 0)  // WRITE
      {
      extVar401 = mes.fltintVal; 
      if (setAck) send401 = true; // send message if required
      #ifdef DEBUGPJ2
        Serial.print("Setting extVar401 to ");
        Serial.println(extVar401);
      #endif
      }
    else // READ
      send401 = true; 
    break;
    
#endif // EXVAR40X
  
  default: send92 = true; // no valid device parsed
  
  } // end of switch statement
} // end parseCmd()
