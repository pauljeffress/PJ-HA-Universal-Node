// ====================== rfSendMsg(): 
// sends Northbound (via RF) all messages that are flagged for transmission by sendXX flags.
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

void rfSendMsg() { // prepares values to be transmitted
  float tempFloat;
  //bool tx = false; // transmission flag
  bool txOnce = true; // flag that ensures only one variable is sent per run through this function.
                      // previously when I did not use this, every variable thats send flag was true
                      // got sent during a single p[ass through this function. That was ok pre 
                      // migration to RadioHead, but since then I think that the RFM GW gets overloaded
                      // when a Node sends too many variables to closely together.  So now it will take
                      // few passes through to send out all variables that have send flags set.

  int i;

  //delay(200);
  rfTxAllowed = false; // helps gate how often rfSendMsg() is called.

  // prep clean RF mes structure
  mes.nodeID=NODEID;
  mes.devID = 999; // If I every see this in a message, I know something is wrong.
  mes.intVal = 0;
  mes.fltintVal = 0;
  mes.cmd = 0; // '0' means no action needed in gateway
  // clean up payload as well, so it is always empty unless explicitly used.
  for ( i = 0; i < 32; i++){
        mes.payLoad[i] = '\0';}
        
  if (wakeUp) { // send wakeUp call
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev99 - wakeUp call");
    #endif
    mes.devID = 99;
    wakeUp = false; // reset transmission flag for this message
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called. // transmit
  }
    
  if (txOnce && send0) {
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev0 - mins uptime");
    #endif
    mes.devID = 0;
    mes.intVal = upTime; // minutes uptime
    send0 = false; // reset transmission flag for this message
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    mes.intVal = 0;  // clear it after use.
  }
 
  if (txOnce && send1) { // transmission interval
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev1 - tx interval");
    #endif
    mes.devID = 1;
    mes.intVal = TXinterval; // seconds (integer)
    send1 = false;
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    mes.intVal = 0;  // clear it after use.
  }
    
  if (txOnce && send2) {  // As this whole function only runs if we are on RFNODETYPE, I don't need to check again here.
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev2 - sig strength");
    #endif
    mes.devID = 2;
    mes.intVal = signalStrength; // signal strength (integer)
    send2 = false;
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    mes.intVal = 0;  // clear it after use.
  }
  
  if (txOnce && send3) { // node software version (string)
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev3 - node sw version string");
    #endif
    mes.devID = 3; // node sw version (string)
    // place software version in payload string
    for ( i = 0; i < sizeof(VERSION); i++){
        mes.payLoad[i] = VERSION[i]; }
    mes.payLoad[i] = '\0'; 
    send3 = false;
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    for ( i = 0; i < 32; i++){  // clear it after use.
        mes.payLoad[i] = '\0';}
  }
  
  if (txOnce && send4) { // measure voltage.. xxxx - is this appropriate for my Moteino nodes and others?
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev4 - voltage");
    #endif
    mes.devID = 4;
    mes.fltintVal = fltTofltint(123.45);  // send this fake number unless one of the below board specific routines kicks in.
    
    #ifdef FEATHERM0 //  The below code to measure battery voltage only applies to Feather M0 boards
      #define VBATPIN A7
      float measuredvbat = analogRead(VBATPIN);
      measuredvbat *= 2;    // we divided by 2, so multiply back
      measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
      measuredvbat /= 1024; // convert to voltage
      mes.fltintVal = fltTofltint(measuredvbat);   // convert to one of my special fltints ready to send.
      #ifdef DEBUG
        Serial.print("VBat: " ); Serial.println(measuredvbat);
        Serial.print("as fltintVal: "); Serial.println(mes.fltintVal);
      #endif
    #endif

    #ifdef MOTEINONODE  //  The below code to measure battery voltage only applies to Moteino boards
      long result; // Read 1.1V reference against AVcc
      ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
      delay(2); // Wait for Vref to settle
      ADCSRA |= _BV(ADSC); // Convert
      while (bit_is_set(ADCSRA,ADSC));
      result = ADCL;
      result |= ADCH<<8;
      result = 1126400L / result; // Back-calculate in mV
      tempFloat = float(result/1000.0);  // Voltage in Volt (float) 
      mes.fltintVal = fltTofltint(tempFloat); 
    #endif
     
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    send4 = false;
    mes.fltintVal = 0;  // clear it after use.
  }
    
  if (txOnce && send5) { // Acknowledge on 'SET'
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev5 - Ack on SET");
    #endif
    mes.devID = 5;
    if (setAck) mes.intVal = 1; else mes.intVal = 0;// state (integer)
    send5 = false;
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    mes.intVal = 0;  // clear it after use.
  }
  
  if (txOnce && send6) { // Toggle on Buttonpress
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev6 - toogle on button press");
    #endif
    mes.devID = 6;
    if (toggleOnButton) mes.intVal = 1; // read state of toggle flag
    else mes.intVal = 0; // state (integer)
    send6 = false;
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    mes.intVal = 0;  // clear it after use.
  }

  if (txOnce && send7) { // timer interval
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev7 - timer interval");
    #endif
    mes.devID = 7;
    mes.intVal = TIMinterval; // seconds (integer)
    send7 = false;
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    mes.intVal = 0;  // clear it after use.
  }

  if (txOnce && send11) { // Compiled Date: Gets embedded into code at compile time (string)(RO)
    #ifdef DEBUGPJ
      Serial.println("rfSendMsg: sending dev11");
    #endif
    mes.devID = 11; 
    for ( i = 0; i < sizeof(__DATE__); i++){
        mes.payLoad[i] = __DATE__[i]; }
    mes.payLoad[i] = '\0'; 
    send11 = false;
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    for ( i = 0; i < 32; i++){  // clear it after use.
        mes.payLoad[i] = '\0';}
  }

  if (txOnce && send12) { // Compiled Time: Gets embedded into code at compile time (string)(RO)
    #ifdef DEBUGPJ
      Serial.println("rfSendMsg: sending dev12");
    #endif
    mes.devID = 12; 
    for ( i = 0; i < sizeof(__TIME__); i++){
        mes.payLoad[i] = __TIME__[i]; }
    mes.payLoad[i] = '\0'; 
    send12 = false;
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    for ( i = 0; i < 32; i++){  // clear it after use.
        mes.payLoad[i] = '\0';}
  }

  #ifdef ACTUATOR1
    if (txOnce && send16) { // state of Actuator 1
      #ifdef DEBUG
        Serial.println("rfSendMsg: sending dev16 - state of Actuator 1");
      #endif
      mes.devID = 16;
      mes.intVal = actuator1status; // state (integer)
      send16 = false;
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
      mes.intVal = 0;  // clear it after use.
    }
  #endif // ACTUATOR1

  #ifdef ACTUATOR2
    if (txOnce && send17) { // state of Actuator 2
      #ifdef DEBUG
        Serial.println("rfSendMsg: sending dev17 - state of Actuator 2");
      #endif
      mes.devID = 17;
      mes.intVal = actuator2status; // state (integer)
      send17 = false;
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
      mes.intVal = 0;  // clear it after use.
    }
  #endif // ACTUATOR2

  #ifdef ACTUATOR3
    if (txOnce && send18) { // state of Actuator 3
      #ifdef DEBUG
        Serial.println("rfSendMsg: sending dev18 - state of Actuator 3");
      #endif
      mes.devID = 18;
      mes.intVal = actuator3status; // state (integer)
      send18 = false;
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
      mes.intVal = 0;  // clear it after use.
    }
  #endif // ACTUATOR3

  #ifdef ACTUATOR4
    if (txOnce && send19) { // state of Actuator 4
      #ifdef DEBUG
        Serial.println("rfSendMsg: sending dev19 - state of Actuator 4");
      #endif
      mes.devID = 19;
      mes.intVal = actuator4status; // state (integer)
      send19 = false;
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
      mes.intVal = 0;  // clear it after use.
    }
  #endif // ACTUATOR4

  #ifdef BUTTON1
    if (txOnce && send40) { // Binary input read
      #ifdef DEBUG
        Serial.println("rfSendMsg: sending dev40 - Binary input read");
      #endif
      mes.devID = 40;
      if (curBtn1State == LOW) mes.intVal = 1; // state (integer)
      send40 = false;
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
      mes.intVal = 0;  // clear it after use.
    }
  #endif // BUTTON1

  #ifdef BUTTON2
    if (txOnce && send42) { // Binary input read
      #ifdef DEBUG
        Serial.println("rfSendMsg: sending dev42 - Binary input read");
      #endif
      mes.devID = 42;
      if (curBtn2State == LOW) mes.intVal = 1; // state (integer)
      send42 = false;
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
      mes.intVal = 0;  // clear it after use.
    }
  #endif // BUTTON2

  #ifdef PIR1
    if (txOnce && send41) { // PIR Binary input read
      #ifdef DEBUGPJ2
        Serial.println("rfSendMsg: sending dev41 - PIR");
      #endif
      mes.devID = 41;
      if (curPIR1State == HIGH) mes.intVal = 1; // state (integer) 1 for ON
      send41 = false;
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
      mes.intVal = 0;  // clear it after use.
    }
  #endif // PIR1

  #ifdef PIR2
    if (txOnce && send43) { // PIR Binary input read
      mes.devID = 43;
      if (curPIR2State == HIGH) mes.intVal = 1; // state (integer) 1 for ON
      send43 = false;
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
      mes.intVal = 0;  // clear it after use.
    }
  #endif // PIR2

  #ifdef SWITCH1
    if (txOnce && send44) { // Binary input read
      #ifdef DEBUG
        Serial.println("rfSendMsg: sending dev44 - Binary input read");
      #endif
      mes.devID = 44;
      if (curSwitch1State == ON) mes.intVal = 1; // state (integer)
      send44 = false;
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
      mes.intVal = 0;  // clear it after use.
    }
  #endif // SWITCH1

  #ifdef SWITCH2
    if (txOnce && send45) { // Binary input read
      #ifdef DEBUG
        Serial.println("rfSendMsg: sending dev45 - Binary input read");
      #endif
      mes.devID = 45;
      if (curSwitch2State == ON) mes.intVal = 1; // state (integer)
      send45 = false;
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
      mes.intVal = 0;  // clear it after use.
    }
  #endif // SWITCH2  

  #ifdef DHT || DS18
    if (txOnce && send48) { // Temperature
      mes.devID = 48;
      #ifdef DHT
        temp = dht.readTemperature();
      #endif
      #ifdef DS18
        temp = sensors.getTempCByIndex(0);
      #endif
      mes.fltintVal = temp; // Degrees Celcius (float)
      send48 = false;
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
      mes.fltintVal = 0;  // clear it after use.
    }
  #endif

  #ifdef DHT
    if (txOnce && send49) { // Humidity
      mes.devID = 49;
      #ifdef DHT
        hum = dht.readHumidity();
      #endif
      mes.fltintVal = fltTofltint(hum); // Percentage (float)
      send49 = false;
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
      mes.fltintVal = 0;  // clear it after use.
    }
  #endif

  #ifdef DHT || DS18  
    if (txOnce && send50) { // Temperature
      mes.devID = 50;
      #ifdef DHT
        temp = dht.readTemperature();
      #endif
      #ifdef DS18
        temp = sensors.getTempCByIndex(1);
      #endif
      mes.fltintVal = fltTofltint(temp); // Degrees Celcius (float)
      send50 = false;
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
      mes.fltintVal = 0;  // clear it after use.
    }
  #endif

  #ifdef MOTEINOWEATHER
  if (txOnce && send51 || send54 || send55) 
    {
      #ifdef DEBUGPJx
        Serial.println("call getW");
      #endif
      getWeatherShield();  // if we have to send any of these, get them all first.
      #ifdef DEBUG PJx
        Serial.println("back from getW");
      #endif
    }
  if (txOnce && send51) { // pressure
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev51 - pressure");
    #endif
    mes.devID = 51;
    mes.fltintVal = fltTofltint(WeatherShieldData[0]); // get pressure from MOTEINO Weather
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    send51 = false;
    mes.fltintVal = 0;  // clear it after use.
  }
  if (txOnce && send54) { // humidity
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev54 - humidity");
    #endif
    mes.devID = 54;
    mes.fltintVal = fltTofltint(WeatherShieldData[1]); // get humidity from MOTEINO Weather
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    send54 = false;
    mes.fltintVal = 0;  // clear it after use.
  }
  if (txOnce && send55) { // temperature
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev55 - temperature");
    #endif
    mes.devID = 55;
    mes.fltintVal = fltTofltint(WeatherShieldData[2]); // get temp from MOTEINO Weather
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    send55 = false;
    mes.fltintVal = 0;  // clear it after use.
  }
  #endif // MOTEINOWEATHER

  #ifdef TSL2651
    if (txOnce && send52 || send53) 
    {
      getTSL2651();  // if we have to send any of these, get them all first.
    }
  if (txOnce && send52) { // visible light
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev52 - visible light");
    #endif
    mes.devID = 52;
    mes.fltintVal = fltTofltint(TSL2651Data[0]); // get visible light level from TSL2651
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    send52 = false;
    mes.fltintVal = 0;  // clear it after use.
  }
  if (txOnce && send53) { // IR light
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev53 - IR light");
    #endif
    mes.devID = 53;
    mes.fltintVal = fltTofltint(TSL2651Data[1]); // get IR light level from TSL2651
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    send53 = false;
    mes.fltintVal = 0;  // clear it after use.
  }
  #endif // TSL2651

// xxxx - shouldn't I have a 91 sender toooo

  if (txOnce && send92) { // error message invalid device
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev92 - error message invalid device");
    #endif
    mes.intVal = mes.devID;
    mes.devID = 92;
    send92 = false;
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    mes.intVal = 0;  // clear it after use.
  }

  if (txOnce && send93) { // error message 
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev93 - error message you tried to write to a RO device");
    #endif
    mes.intVal = mes.devID;
    mes.devID = 93;
    send93 = false;
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    mes.intVal = 0;  // clear it after use.
  }

  if (txOnce && send94) { // error message 
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev94 - error message you to READ a WRITEONLY device");
    #endif
    mes.intVal = mes.devID;
    mes.devID = 94;
    send94 = false;
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    mes.intVal = 0;  // clear it after use.
  }  

  if (txOnce && send95) { // error message 
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev95 - error message Value to be WRITTEN is out of range");
    #endif
    mes.intVal = mes.devID;
    mes.devID = 95;
    send95 = false;
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    mes.intVal = 0;  // clear it after use.
  }

  #ifdef XMASLIGHTS
  if (txOnce && send100) { // XMAS LIGHTS present status
    mes.devID = 100;
    mes.intVal = XmasLightsMode; // i.e. whatever mode we last set Xmas lights to.
    send100 = false; // as we are about to send it in next statement, so clear flag.
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called. // RF send msg.
    mes.intVal = 0;  // clear it after use.
  }
  #endif

#ifdef LEDSTRIP
  if (txOnce && send200)  
    {
      mes.devID = 200;
      #ifdef LEDSTRIPS_REMOTE
        mes.intVal = current200;
      #else
        mes.intVal = LEDStrip1Type; 
      #endif
      send200 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called. // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (txOnce && send201)  
    {
      mes.devID = 201;
      #ifdef LEDSTRIPS_REMOTE
        mes.intVal = current201;
      #else
        mes.intVal = LEDStrip1RedValue; 
      #endif
      send201 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called. // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (txOnce && send202)  
    {
      mes.devID = 202;
      #ifdef LEDSTRIPS_REMOTE
        mes.intVal = current202;
      #else
        mes.intVal = LEDStrip1GreenValue; 
      #endif
      send202 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called. // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (txOnce && send203)  
    {
      mes.devID = 203;
      #ifdef LEDSTRIPS_REMOTE
        mes.intVal = current203;
      #else
        mes.intVal = LEDStrip1BlueValue; 
      #endif 
      send203 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called. // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (txOnce && send204)  
    {
      mes.devID = 204;
      #ifdef LEDSTRIPS_REMOTE
        mes.intVal = current204;
      #else
        mes.intVal = LEDStrip1BrightnessValue;
      #endif
      send204 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called. // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (txOnce && send221)  
    {
      mes.devID = 221;
      mes.intVal = current221;
      send221 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called. // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (txOnce && send231)  
    {
      mes.devID = 231;
      mes.intVal = current231;
      send231 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called. // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (txOnce && send291)  
    {
      mes.devID = 291;
      mes.intVal = current291;
      send291 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called. // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }



  if (txOnce && send299)  
    {
      mes.devID = 299;
      #ifdef LEDSTRIPS_REMOTE
        mes.intVal = 1;
      #else 
        mes.intVal = 0; 
      #endif
      send299 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called. // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }
#endif // LEDSTRIP

#ifdef EXTVAR40X
  if (txOnce && send400)  // 400     - TestExtVar - purely for testing, does not represent any real external variable. (Real - R/W)
    {
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev400-TestExtVar");
    #endif
    mes.devID = 400;
    mes.fltintVal = fltTofltint(extVar400); // current value we have for it on this Node.
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    send400 = false;
    mes.fltintVal = 0;  // clear it after use.
    }
  if (txOnce && send401)  // 401     - MasterLEDBrightness (0-255) - if 0, tells DUE to turn off whole display. 1..255 means turn it on and set master brightness to X. (Real - R/W)
    {
    #ifdef DEBUG
      Serial.println("rfSendMsg: sending dev401-MasterLEDBrightness");
    #endif
    mes.devID = 401;
    mes.fltintVal = fltTofltint(extVar401); // current value we have for it on this Node.
    txRadio(); txOnce = false; // we have transmitted one variable, don't tx anymore til next time this function is called.
    send401 = false;
    mes.fltintVal = 0;  // clear it after use.
    }
#endif // EXTVAR40X

  
} // end rfSendMsg()
