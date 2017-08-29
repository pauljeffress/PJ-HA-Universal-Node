// ====================== sendMsg(): 
// sends Northbound (via RF) all messages that are flagged for transmission by sendXX flags.
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

void sendMsg() { // prepares values to be transmitted
  
  bool tx = false; // transmission flag
  int i;
  // prep clean RF mes structure
  mes.nodeID=NODEID;
  mes.devID = 999; // If I every see this in a message, I know something is wrong.
  mes.intVal = 0;
  mes.fltVal = 0;
  mes.cmd = 0; // '0' means no action needed in gateway
  // clean up payload as well, so it is always empty unless explicitly used.
  for ( i = 0; i < 32; i++){
        mes.payLoad[i] = '\0';}
        
  if (wakeUp) { // send wakeUp call
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev99 - wakeUp call");
    #endif
    mes.devID = 99;
    wakeUp = false; // reset transmission flag for this message
    txRadio(); // transmit
  }
    
  if (send0) {
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev0 - mins uptime");
    #endif
    mes.devID = 0;
    mes.intVal = upTime; // minutes uptime
    send0 = false; // reset transmission flag for this message
    txRadio();
    mes.intVal = 0;  // clear it after use.
  }
  
  if (send1) { // transmission interval
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev1 - tx interval");
    #endif
    mes.devID = 1;
    mes.intVal = TXinterval; // seconds (integer)
    send1 = false;
    txRadio();
    mes.intVal = 0;  // clear it after use.
  }
  
  if (send2) {  // As this whole function only runs if we are on RFNODETYPE, I don't need to check again here.
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev2 - sig strength");
    #endif
    mes.devID = 2;
    mes.intVal = signalStrength; // signal strength (integer)
    send2 = false;
    txRadio();
    mes.intVal = 0;  // clear it after use.
  }

  if (send3) { // node software version (string)
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev3 - node sw version string");
    #endif
    mes.devID = 3; // node sw version (string)
    // place software version in payload string
    for ( i = 0; i < sizeof(VERSION); i++){
        mes.payLoad[i] = VERSION[i]; }
    mes.payLoad[i] = '\0'; 
    send3 = false;
    txRadio();
    for ( i = 0; i < 32; i++){  // clear it after use.
        mes.payLoad[i] = '\0';}
  }

  if (send4) { // measure voltage.. xxxx - is this appropriate for my Moteino nodes and others?
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev4 - voltage");
    #endif
    mes.devID = 4;
    long result; // Read 1.1V reference against AVcc
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Convert
    while (bit_is_set(ADCSRA,ADSC));
    result = ADCL;
    result |= ADCH<<8;
    result = 1126400L / result; // Back-calculate in mV
    mes.fltVal = float(result/1000.0); // Voltage in Volt (float)
    txRadio();
    send4 = false;
    mes.fltVal = 0;  // clear it after use.
  }
  
  if (send5) { // Acknowledge on 'SET'
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev5 - Ack on SET");
    #endif
    mes.devID = 5;
    if (setAck) mes.intVal = 1; else mes.intVal = 0;// state (integer)
    send5 = false;
    txRadio();
    mes.intVal = 0;  // clear it after use.
  }

  if (send6) { // Toggle on Buttonpress
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev6 - toogle on button press");
    #endif
    mes.devID = 6;
    if (toggleOnButton) mes.intVal = 1; // read state of toggle flag
    else mes.intVal = 0; // state (integer)
    send6 = false;
    txRadio();
    mes.intVal = 0;  // clear it after use.
  }

  if (send7) { // timer interval
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev7 - timer interval");
    #endif
    mes.devID = 7;
    mes.intVal = TIMinterval; // seconds (integer)
    send7 = false;
    txRadio();
    mes.intVal = 0;  // clear it after use.
  }

  if (send11) { // Compiled Date: Gets embedded into code at compile time (string)(RO)
    #ifdef DEBUGPJ
      Serial.println("SENDMSG: sending dev11");
    #endif
    mes.devID = 11; 
    for ( i = 0; i < sizeof(__DATE__); i++){
        mes.payLoad[i] = __DATE__[i]; }
    mes.payLoad[i] = '\0'; 
    send11 = false;
    txRadio();
    for ( i = 0; i < 32; i++){  // clear it after use.
        mes.payLoad[i] = '\0';}
  }

  if (send12) { // Compiled Time: Gets embedded into code at compile time (string)(RO)
    #ifdef DEBUGPJ
      Serial.println("SENDMSG: sending dev12");
    #endif
    mes.devID = 12; 
    for ( i = 0; i < sizeof(__TIME__); i++){
        mes.payLoad[i] = __TIME__[i]; }
    mes.payLoad[i] = '\0'; 
    send12 = false;
    txRadio();
    for ( i = 0; i < 32; i++){  // clear it after use.
        mes.payLoad[i] = '\0';}
  }

  #ifdef ACTUATOR1
    if (send16) { // state of Actuator 1
      #ifdef DEBUG
        Serial.println("SENDMSG: sending dev16 - state of Actuator 1");
      #endif
      mes.devID = 16;
      mes.intVal = actuator1status; // state (integer)
      send16 = false;
      txRadio();
      mes.intVal = 0;  // clear it after use.
    }
  #endif // ACTUATOR1

  #ifdef ACTUATOR2
    if (send17) { // state of Actuator 2
      #ifdef DEBUG
        Serial.println("SENDMSG: sending dev17 - state of Actuator 2");
      #endif
      mes.devID = 17;
      mes.intVal = actuator2status; // state (integer)
      send17 = false;
      txRadio();
      mes.intVal = 0;  // clear it after use.
    }
  #endif // ACTUATOR2

  #ifdef ACTUATOR3
    if (send18) { // state of Actuator 3
      #ifdef DEBUG
        Serial.println("SENDMSG: sending dev18 - state of Actuator 3");
      #endif
      mes.devID = 18;
      mes.intVal = actuator3status; // state (integer)
      send18 = false;
      txRadio();
      mes.intVal = 0;  // clear it after use.
    }
  #endif // ACTUATOR3

  #ifdef ACTUATOR4
    if (send19) { // state of Actuator 4
      #ifdef DEBUG
        Serial.println("SENDMSG: sending dev19 - state of Actuator 4");
      #endif
      mes.devID = 19;
      mes.intVal = actuator4status; // state (integer)
      send19 = false;
      txRadio();
      mes.intVal = 0;  // clear it after use.
    }
  #endif // ACTUATOR4

  #ifdef BUTTON1
    if (send40) { // Binary input read
      #ifdef DEBUG
        Serial.println("SENDMSG: sending dev40 - Binary input read");
      #endif
      mes.devID = 40;
      if (curBtn1State == LOW) mes.intVal = 1; // state (integer)
      send40 = false;
      txRadio();
      mes.intVal = 0;  // clear it after use.
    }
  #endif // BUTTON1

  #ifdef BUTTON2
    if (send42) { // Binary input read
      #ifdef DEBUG
        Serial.println("SENDMSG: sending dev42 - Binary input read");
      #endif
      mes.devID = 42;
      if (curBtn2State == LOW) mes.intVal = 1; // state (integer)
      send42 = false;
      txRadio();
      mes.intVal = 0;  // clear it after use.
    }
  #endif // BUTTON2

  #ifdef PIR1
    if (send41) { // PIR Binary input read
      #ifdef DEBUGPJ2
        Serial.println("SENDMSG: sending dev41 - PIR");
      #endif
      mes.devID = 41;
      if (curPIR1State == HIGH) mes.intVal = 1; // state (integer) 1 for ON
      send41 = false;
      txRadio();
      mes.intVal = 0;  // clear it after use.
    }
  #endif // PIR1

  #ifdef PIR2
    if (send43) { // PIR Binary input read
      mes.devID = 43;
      if (curPIR2State == HIGH) mes.intVal = 1; // state (integer) 1 for ON
      send43 = false;
      txRadio();
      mes.intVal = 0;  // clear it after use.
    }
  #endif // PIR2
  
  if (send48) { // Temperature
    mes.devID = 48;
    #ifdef DHT
      temp = dht.readTemperature();
    #endif
    #ifdef DS18
      temp = sensors.getTempCByIndex(0);
    #endif
    mes.fltVal = temp; // Degrees Celcius (float)
    send48 = false;
    txRadio();
    mes.fltVal = 0;  // clear it after use.
  }

  if (send49) { // Humidity
    mes.devID = 49;
    #ifdef DHT
      hum = dht.readHumidity();
    #endif
    mes.fltVal = hum; // Percentage (float)
    send49 = false;
    txRadio();
    mes.fltVal = 0;  // clear it after use.
  }

  if (send50) { // Temperature
    mes.devID = 50;
    #ifdef DHT
      temp = dht.readTemperature();
    #endif
    #ifdef DS18
      temp = sensors.getTempCByIndex(1);
    #endif
    mes.fltVal = temp; // Degrees Celcius (float)
    send50 = false;
    txRadio();
    mes.fltVal = 0;  // clear it after use.
  }

  #ifdef MOTEINOWEATHER
  if (send51 || send54 || send55) 
    {
      #ifdef DEBUGPJx
        Serial.println("call getW");
      #endif
      getWeatherShield();  // if we have to send any of these, get them all first.
      #ifdef DEBUG PJx
        Serial.println("back from getW");
      #endif
    }
  if (send51) { // pressure
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev51 - pressure");
    #endif
    mes.devID = 51;
    mes.fltVal = WeatherShieldData[0]; // get pressure from MOTEINO Weather
    txRadio();
    send51 = false;
    mes.fltVal = 0;  // clear it after use.
  }
  if (send54) { // humidity
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev54 - humidity");
    #endif
    mes.devID = 54;
    mes.fltVal = WeatherShieldData[1]; // get humidity from MOTEINO Weather
    txRadio();
    send54 = false;
    mes.fltVal = 0;  // clear it after use.
  }
  if (send55) { // temperature
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev55 - temperature");
    #endif
    mes.devID = 55;
    mes.fltVal = WeatherShieldData[2]; // get temp from MOTEINO Weather
    txRadio();
    send55 = false;
    mes.fltVal = 0;  // clear it after use.
  }
  #endif // MOTEINOWEATHER

  #ifdef TSL2651
    if (send52 || send53) 
    {
      getTSL2651();  // if we have to send any of these, get them all first.
    }
  if (send52) { // visible light
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev52 - visible light");
    #endif
    mes.devID = 52;
    mes.fltVal = TSL2651Data[0]; // get visible light level from TSL2651
    txRadio();
    send52 = false;
    mes.fltVal = 0;  // clear it after use.
  }
  if (send53) { // IR light
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev53 - IR light");
    #endif
    mes.devID = 53;
    mes.fltVal = TSL2651Data[1]; // get IR light level from TSL2651
    txRadio();
    send53 = false;
    mes.fltVal = 0;  // clear it after use.
  }
  #endif // TSL2651

// xxxx - shouldn't I have a 91 sender toooo

  if (send92) { // error message invalid device
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev92 - error message invalid device");
    #endif
    mes.intVal = mes.devID;
    mes.devID = 92;
    send92 = false;
    txRadio();
    mes.intVal = 0;  // clear it after use.
  }

  if (send93) { // error message 
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev93 - error message you tried to write to a RO device");
    #endif
    mes.intVal = mes.devID;
    mes.devID = 93;
    send93 = false;
    txRadio();
    mes.intVal = 0;  // clear it after use.
  }

  if (send94) { // error message 
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev94 - error message you to READ a WRITEONLY device");
    #endif
    mes.intVal = mes.devID;
    mes.devID = 94;
    send94 = false;
    txRadio();
    mes.intVal = 0;  // clear it after use.
  }  

  if (send95) { // error message 
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev95 - error message Value to be WRITTEN is out of range");
    #endif
    mes.intVal = mes.devID;
    mes.devID = 95;
    send95 = false;
    txRadio();
    mes.intVal = 0;  // clear it after use.
  }

  #ifdef XMASLIGHTS
  if (send100) { // XMAS LIGHTS present status
    mes.devID = 100;
    mes.intVal = XmasLightsMode; // i.e. whatever mode we last set Xmas lights to.
    send100 = false; // as we are about to send it in next statement, so clear flag.
    txRadio(); // RF send msg.
    mes.intVal = 0;  // clear it after use.
  }
  #endif

#ifdef LEDSTRIP
  if (send200)  
    {
      mes.devID = 200;
      #ifdef LEDSTRIPS_REMOTE
        mes.intVal = current200;
      #else
        mes.intVal = LEDStrip1Type; 
      #endif
      send200 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (send201)  
    {
      mes.devID = 201;
      #ifdef LEDSTRIPS_REMOTE
        mes.intVal = current201;
      #else
        mes.intVal = LEDStrip1RedValue; 
      #endif
      send201 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (send202)  
    {
      mes.devID = 202;
      #ifdef LEDSTRIPS_REMOTE
        mes.intVal = current202;
      #else
        mes.intVal = LEDStrip1GreenValue; 
      #endif
      send202 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (send203)  
    {
      mes.devID = 203;
      #ifdef LEDSTRIPS_REMOTE
        mes.intVal = current203;
      #else
        mes.intVal = LEDStrip1BlueValue; 
      #endif 
      send203 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (send204)  
    {
      mes.devID = 204;
      #ifdef LEDSTRIPS_REMOTE
        mes.intVal = current204;
      #else
        mes.intVal = LEDStrip1BrightnessValue;
      #endif
      send204 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (send221)  
    {
      mes.devID = 221;
      mes.intVal = current221;
      send221 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (send231)  
    {
      mes.devID = 231;
      mes.intVal = current231;
      send231 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }

  if (send291)  
    {
      mes.devID = 291;
      mes.intVal = current291;
      send291 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }



  if (send299)  
    {
      mes.devID = 299;
      #ifdef LEDSTRIPS_REMOTE
        mes.intVal = 1;
      #else 
        mes.intVal = 0; 
      #endif
      send299 = false; // as we are about to send it in next statement, so clear flag.
      txRadio(); // RF send msg.
      mes.intVal = 0;  // clear it after use.
    }
#endif // LEDSTRIP

#ifdef EXTVAR40X
  if (send400)  // 400     - TestExtVar - purely for testing, does not represent any real external variable. (Real - R/W)
    {
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev400-TestExtVar");
    #endif
    mes.devID = 400;
    mes.fltVal = extVar400; // current value we have for it on this Node.
    txRadio();
    send400 = false;
    mes.fltVal = 0;  // clear it after use.
    }
  if (send401)  // 401     - MasterLEDBrightness (0-255) - if 0, tells DUE to turn off whole display. 1..255 means turn it on and set master brightness to X. (Real - R/W)
    {
    #ifdef DEBUG
      Serial.println("SENDMSG: sending dev401-MasterLEDBrightness");
    #endif
    mes.devID = 401;
    mes.fltVal = extVar401; // current value we have for it on this Node.
    txRadio();
    send401 = false;
    mes.fltVal = 0;  // clear it after use.
    }
#endif // EXTVAR40X

  
} // end sendMsg()
