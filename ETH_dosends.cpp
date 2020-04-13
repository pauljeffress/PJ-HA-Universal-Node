/*  Edit Log
 *  ========
 *  14-02-16 - put #ifdef stuff around send 40, 41, 42 & 43 so they were only compiled if corresponding PIR / BUTTON etc were actually defined.
 *  
 */

//
// ========== dosends()
// sends Northbound (via IP/MQTT) all messages that are flagged for transmission by sendXX flags.
//

#include "a.h" // My global defines and extern variables to help multi file comilation.

void dosends() {

// xxxx - there should be a stanza here that acts on the wakeUp flag (like it does in RF rfSendMsg())

  if (send0) 
    {
    sendInteger(NODEID, 0, upTime);
    send0 = false; 
    }

  if (send1)
    {
    sendInteger(NODEID, 1, TXinterval);
    send1 = false;
    }

  if (send3) 
    {
    //for (i=0; i<6; i++){                  //    copy the string into the buff_Mess (i.e. payload we will send north)
    //  tempstring[i] = (VERSION[i]); } // xxxx - can I get rid of this, does the shorter version below work?
    //tempstring[i] = '\0';
    sendString(NODEID, 3, VERSION);
    send3 = false; 
    }

  if (send4)
    {
    sendReal(NODEID, 4, 99.99); //    xxxx - put code in here to extract battery voltage (see my RF_sendMsg() code). Just faking 99.99v for now
    send4 = false;
    }

  if (send5)
    {
    sendStatusInteger(NODEID, 5, setAck);
    send5 = false;
    }

  if (send6)
    {
    sendStatusInteger(NODEID, 6, toggleOnButton);
    send6 = false;
    }

  if (send7)
    {
    sendInteger(NODEID, 7, TIMinterval);
    send7 = false;
    }   

  if (send11) 
    {
    sendString(NODEID, 11, (__DATE__));
    send11 = false; 
    }

  if (send12) 
    {
    sendString(NODEID, 12, (__TIME__));
    send12 = false; 
    }

  #ifdef ACTUATOR1
  if (send16)
    {
    sendStatusInteger(NODEID, 16, actuator1status);
    send16 = false;
    }
  #endif

  #ifdef ACTUATOR2
  if (send17)
    {
    sendStatusInteger(NODEID, 17, actuator2status);
    send17 = false;
    }
  #endif

  #ifdef ACTUATOR3
  if (send18)
    {
    sendStatusInteger(NODEID, 18, actuator3status);
    send18 = false;
    }
  #endif

  #ifdef ACTUATOR4
  if (send19)
    {
    sendStatusInteger(NODEID, 19, actuator4status);
    send19 = false;
    }
 #endif
 
#ifdef BUTTON1 // BUTTON1 is connected
  if (send40)
    {
    sendStatusInteger(NODEID, 40, curBtn1State);
    send40 = false;
    }
#endif

#ifdef PIR1 // PIR1 is connected
  if (send41)
    {
    sendStatusInteger(NODEID, 41, curPIR1State);
    send41 = false;
    }
#endif

#ifdef BUTTON2 // BUTTON2 is connected
  if (send42)
    {
    sendStatusInteger(NODEID, 42, curBtn2State);
    send42 = false;
    }
#endif

#ifdef PIR2 // PIR2 is connected
  if (send43)
    {
    sendStatusInteger(NODEID, 43, curPIR2State);
    send43 = false;
    }
#endif

#ifdef SWITCH1 // SWITCH1 is connected
  if (send44)
    {
    sendStatusInteger(NODEID, 44, curSwitch1State);
    send44 = false;
    }
#endif

#ifdef SWITCH2 // SWITCH2 is connected
if (send45)
  {
  sendStatusInteger(NODEID, 45, curSwitch2State);
  send45 = false;
  }
#endif

#ifdef DHTSENSOR
    if (send48) { // Temperature DHT
      sendReal(NODEID, 48, dht.readTemperature()); // Degrees Celcius (float)
      send48 = false;
    }

    if (send49) { // Humidity
      sendReal(NODEID, 49, dht.readHumidity());  // Percentage (float)
      send49 = false;
    }
  #endif  // DHTSENSOR

#ifdef SOILMOISTURE1
    if (send68) { // 1st Soil Moisture sensor reading
      sendInteger(NODEID, 68, readSoilMoisture(SOILPOWERPIN1, SOILMOISTUREPIN1)); // Soil Moisture (int)
      send68 = false;
    }
#endif

#ifdef SOILMOISTURE2
    if (send69) { // 2nd Soil Moisture sensor reading
      sendInteger(NODEID, 69, readSoilMoisture(SOILPOWERPIN2, SOILMOISTUREPIN2)); // Soil Moisture (int)
      send69 = false;
    }
#endif

#ifdef MOTEINOWEATHER
  if (send51 || send54 || send55) 
    {
      //CONSOLE_PORT.println("call getW");
      getWeatherShield();  // if we have to send any of these, get them all first.
      //CONSOLE_PORT.println("back from getW");
    }
  if (send51)  // pressure
    {
    sendReal(NODEID, 51, WeatherShieldData[0]); 
    send51 = false;
    }
  if (send54)  // humidity
    {
     sendReal(NODEID, 54, WeatherShieldData[1]);  
     send54 = false; 
    }
  if (send55)  // temperature
    {
     sendReal(NODEID, 55, WeatherShieldData[2]);  
     send55 = false;    
    }
#endif // MOTEINOWEATHER

#ifdef TSL2651
    if (send52 || send53) 
    {
      getTSL2651();  // if we have to send any of these, get them all first.
    }
  if (send52)  // visible light
    {
     sendReal(NODEID, 52, TSL2651Data[0]);  
     send52 = false;   
    }
  if (send53)  // IR light
    {
     sendReal(NODEID, 53, TSL2651Data[1]);  
     send53 = false; 
    }
#endif // TSL2651


#ifdef PING1x
  if (send64) 
    {
     sendInteger(NODEID, 64, ping1Distance);  
     send64 = false;   
    }
  if (send65) 
    {
     sendInteger(NODEID, 65, ping1RangeMin);  
     send65 = false;   
    }
  if (send66) 
    {
     sendInteger(NODEID, 66, ping1RangeMax);  
     send66 = false;   
    }
  if (send67) 
    {
     sendInteger(NODEID, 67, ping1Detect);  // ping1Detect is actually a Bool, but gets sent as an integer ok.
     send67 = false;   
    }
#endif // PING1x

// Dev09x - Error messages
// xxxx - what other errors should be setup to send back from an ETHNODE (see RF_rfSendMsg etc on RF side for ideas)
if (send92) 
    {
    sendString(NODEID, 92, DEV92ERRORSTR);
    send92 = false; 
    }
if (send93) 
    {
    sendString(NODEID, 93, DEV93ERRORSTR);
    send93 = false; 
    }
if (send94) 
    {
    sendString(NODEID, 94, DEV94ERRORSTR);
    send94 = false; 
    }
if (send95) 
    {
    sendString(NODEID, 95, DEV95ERRORSTR);
    send95 = false; 
    }    

#ifdef XMASLIGHTS
  if (send100)  // XMAS LIGHTS present status
    {
    sendInteger(NODEID, 100, XmasLightsMode);
    send100 = false;
    }
#endif

#ifdef LEDSTRIP
  if (send200)  //  1st RGB LED Strip Type value - 0 = DUMB, 1 = PIXEL
    {
    sendInteger(NODEID, 200, LEDStrip1Type);
    send200 = false;
    }
  if (send201)  // 1st RGB LED Strip RED value
    {
    sendInteger(NODEID, 201, LEDStrip1RedValue);
    send201 = false;
    }
  if (send202)  // 1st RGB LED Strip GREEN value
    {
    sendInteger(NODEID, 202, LEDStrip1GreenValue);
    send202 = false;
    }
  if (send203)  // 1st RGB LED Strip BLUE value
    {
    sendInteger(NODEID, 203, LEDStrip1BlueValue);
    send203 = false;
    }
  if (send204)  // 1st RGB LED Strip BRIGHTNESS value
    {
    sendInteger(NODEID, 204, LEDStrip1BrightnessValue);
    send204 = false;
    }
  if (send221)  // 
    {
    sendInteger(NODEID, 221, current221);
    send221 = false;
    }
  if (send231)  // 
    {
    sendInteger(NODEID, 231, current231);
    send231 = false;
    }
  if (send291)  //
    {
    sendInteger(NODEID, 291, current291);
    send291 = false;
    }
#endif // LEDSTRIP

#ifdef RTC
    if (send300 || send301 || send302 || send303 || send304 || send305 || send306 || send307 || send308) 
    {
      if (!rtcOk) // i.e. rtc was not found or was not running at startup
        {
         #ifdef DEBUGPJ2
          CONSOLE_PORT.print("rtcOk = false, so rtc was either not found or was not running at startup");
          // xxxx - should trigger the send of an error MQTT msg here.
         #endif  
        }
      now = rtc.now();  // if we have to send any of these, get a snapshot of them all first.
      #ifdef DEBUGPJ2
          CONSOLE_PORT.print(now.year(), DEC);
          CONSOLE_PORT.print('/');
          CONSOLE_PORT.print(now.month(), DEC);
          CONSOLE_PORT.print('/');
          CONSOLE_PORT.print(now.day(), DEC);
          CONSOLE_PORT.print(" (");
          CONSOLE_PORT.print(rtcDaysOfTheWeek[now.dayOfTheWeek()]);
          CONSOLE_PORT.print(") ");
          CONSOLE_PORT.print(now.hour(), DEC);
          CONSOLE_PORT.print(':');
          CONSOLE_PORT.print(now.minute(), DEC);
          CONSOLE_PORT.print(':');
          CONSOLE_PORT.print(now.second(), DEC);
          CONSOLE_PORT.println();
       #endif
    }
  if (send300)  // 300: Unixtime - high 16 bits (Unixtime is a 32bit Arduino 'Long')
    {
     rtcUnixTimeHigh=1234; // xxxx - come back here and actually set variable proerly before sending it.
     sendReal(NODEID, 300, rtcUnixTimeHigh);  
     send300 = false;   
    }
  if (send301)  // 301: Unixtime - low 16 bits (Unixtime is a 32bit Arduino 'Long')
    {
     rtcUnixTimeLow=4567; // xxxx - come back here and actually set variable proerly before sending it.
     sendReal(NODEID, 301, rtcUnixTimeLow);  
     send301 = false;   
    }
  if (send302)  // 302: date - year (integer)
    {
     sendReal(NODEID, 302, now.year());  
     send302 = false;   
    }
  if (send303)  // 303: date - month (integer)
    {
     sendReal(NODEID, 303, now.month());  
     send303 = false;   
    }
  if (send304)  // 304: date - day (integer)
    {
     sendReal(NODEID, 304, now.day());  
     send304 = false;   
    }
  if (send305)  // 305: date - day of the week(integer) (Sunday is start of week)
    {
     sendReal(NODEID, 305, now.dayOfTheWeek());  
     send305 = false;   
    }
  if (send306)  // 306 time - hour (integer) 24hr
    {
     sendReal(NODEID, 306, now.hour());  
     send306 = false;   
    }
  if (send307)  // 307: time - minute (integer)
    {
     sendReal(NODEID, 307, now.minute());  
     send307 = false;   
    }
  if (send308)  // 308: time - second (integer)
    {
     sendReal(NODEID, 308, now.second());  
     send308 = false;   
    }
#endif // RTC

#ifdef LCDGENERIC
  if (send320)  // 320 - Display String (R/W) (max 20 char string/array) Although only makes sense to WRITE.
    {
     sendString(NODEID, 320, lcdGenStr);  
     send320 = false;   
    }
  if (send321)  // 321 - String Length (R/W)  (integer) Although only makes sense to WRITE.
    {
     sendInteger(NODEID, 321, lcdGenStrLen);  
     send321 = false;   
    }
  if (send322)  // 322 - X-Coord (R/W)  (integer) Although only makes sense to WRITE.
    {
     sendInteger(NODEID, 322, lcdGenXPos);  
     send322 = false;   
    }
  if (send323)  // 323 - Y-Coord (R/W)  (integer) Although only makes sense to WRITE.
    {
     sendInteger(NODEID, 323, lcdGenYPos);  
     send323 = false;   
    }
  if (send324)  // 324 - DisplayNow command - (WO) (integer) only when a node gets a true/1 written to this DEV does it use info in 320-323 and write to LCD.  It only makes sense to WRITE a true/1 to this. The node resets it to false/0 after it has written to the LCD.
    {
     sendString(NODEID, 94, "error:you tried to READ a WRITEONLY device. Dev324");  
     send324 = false;   
    }
#endif // LCDGENERIC

#ifdef BEEPER
  if (send340)  // 340 - Play beep X now - (WO) (integer).  
    {
     sendString(NODEID, 94, "error:you tried to READ a WRITEONLY device. Dev340");  
     send340 = false;   
    }
#endif

#ifdef FINGER
  if (send350)  // 350 - FPS lookup occurred and the result is ...(RO) (integer).
    {
    sendInteger(NODEID, 350, FPSIdentifyStatus);
    send350 = false;
    }
#endif

#ifdef EXTVAR40X
  if (send400)  // 400     - TestExtVar - purely for testing, does not represent any real external variable. (Real - R/W)
    {
    sendReal(NODEID, 400, extVar400);
    send400 = false;
    }
  if (send401)  // 401     - MasterLEDBrightness (0-255) - if 0, tells DUE to turn off whole display. 1..255 means turn it on and set master brightness to X. (Real - R/W)
    {
    sendReal(NODEID, 401, extVar401);
    send401 = false;
    }
    
#endif // EXTVAR40X


} // END - dosends()

