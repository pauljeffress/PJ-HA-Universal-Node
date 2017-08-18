//===================== SETUP ========================================

#include "a.h" // My global defines and extern variables to help multi file comilation.

void setup() {

#ifdef SLEEPY // uncomment if flash on board
/*  if (flash.initialize()) {
      Serial.println("Flash Init OK!");
      flash.sleep();          // put flash (if it exists) into low power mode
      Serial.println("Flash sleeping");
    } else {
      Serial.println("Flash Init FAIL!");
    } */
#endif

#ifdef DEBUGPJ1
  Serial.begin(SERIAL_BAUD); // Initialise the 1st hw serial port for Arduino IDE Serial Monitor
#endif

#ifdef DEBUGPJ2
  Serial.begin(SERIAL_BAUD); // Initialise the 1st hw serial port for Arduino IDE Serial Monitor
#endif


#ifdef DEBUGPJx
  Serial.print( F("Compiled: "));
  Serial.print( F(__DATE__));
  Serial.print( F(", "));
  Serial.println( F(__TIME__));
  
  Serial.println(F("PJ HA Unified Node"));
  Serial.print("N ");
  Serial.print(NODEID);
  Serial.print(" ");
  Serial.println(VERSION);
  #ifdef RFNODETYPE
    Serial.println("RF Node Type");
    Serial.print("Freq ");
    Serial.print(FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
    Serial.println(" Mhz");
  #endif
  #ifdef ETHNODETYPE
    Serial.println("ETH Node Type");
    Serial.print("MAC ");
    for (int i = 0; i < 6; i++)
      {
      Serial.print(mac[i], HEX);
      Serial.print(":");
      }
    Serial.println();  
    Serial.print("Node IP Addr: ");
    for (int i = 0; i < 4; i++)
      {
      Serial.print(ip[i]);
      Serial.print(".");
      }
    Serial.println();  
    Serial.print("MQTT Broker: ");
    for (int i = 0; i < 4; i++)
      {
      Serial.print(mqtt_server[i]);
      Serial.print(".");
      }
    Serial.println();    
  #endif  
#endif

  //serSent200 = 5;
  //Serial.print("current200 = "); Serial.print(current200); Serial.print("   serSent200 = "); Serial.println(serSent200);
  //delay(1000);


#ifdef BEEPER
  for (int i = 0; i < 4; i++)
    {
      tone(BEEPERPIN, beeperNotes[i], beeperDurations[i]);
      delay(250);
    }
#endif // BEEPER



  // Setup indicator LEDs to initial state
  pinMode(COMMS_LED_PIN, OUTPUT);   // set pin of IP indicator
#ifdef ETHNODETYPE  
  pinMode(MQCON, OUTPUT);           // set pin for MQTT connection indicator
#endif
  pinMode(STATUS_LED_PIN, OUTPUT);  // set pin for Status indicator
#ifdef ETHNODETYPE    
  digitalWrite(MQCON, LOW);         // switch off MQTT connection indicator
#endif
  digitalWrite(COMMS_LED_PIN, LOW); // switch off IP indicator
  digitalWrite(STATUS_LED_PIN, LOW);         // switch ON Power/Startup indicator
  // test all indicator LEDS - flash all LEDS 3 times. Then leave just STATUS LED on.
  delay(300);
#ifdef ETHNODETYPE    
  digitalWrite(MQCON, HIGH);         // switch all LEDS ON
#endif
  digitalWrite(COMMS_LED_PIN, HIGH);       
  digitalWrite(STATUS_LED_PIN, HIGH);        
  delay(300);
#ifdef ETHNODETYPE    
  digitalWrite(MQCON, LOW);         // switch all LEDS OFF
#endif
  digitalWrite(COMMS_LED_PIN, LOW);       
  digitalWrite(STATUS_LED_PIN, LOW);        
  delay(300);
#ifdef ETHNODETYPE    
  digitalWrite(MQCON, HIGH);         // switch all LEDS ON
#endif
  digitalWrite(COMMS_LED_PIN, HIGH);       
  digitalWrite(STATUS_LED_PIN, HIGH);        
  delay(300);
#ifdef ETHNODETYPE    
  digitalWrite(MQCON, LOW);         // switch all LEDS OFF
#endif
  digitalWrite(COMMS_LED_PIN, LOW);       
  digitalWrite(STATUS_LED_PIN, LOW);        
  delay(300);
#ifdef ETHNODETYPE    
  digitalWrite(MQCON, HIGH);         // switch all LEDS ON
#endif
  digitalWrite(COMMS_LED_PIN, HIGH);       
  digitalWrite(STATUS_LED_PIN, HIGH);        
  delay(300);
#ifdef ETHNODETYPE    
  digitalWrite(MQCON, LOW);         // switch all LEDS OFF but leave Power LED ON
#endif
  digitalWrite(COMMS_LED_PIN, LOW);             
  delay(300);
  
#ifdef ETHNODETYPE   // Do all of the ETHNODETYPE unique setup() actions.
// Setup Ethernet port
  Ethernet.begin(mac, ip);     // start the Ethernet connection with static IP
  // Make initial connection to MQTT Broker via IP
  Serial.println("MQTT connecting...");
  mqttCon = 0;          // reset connection flag
  mqttCon = mqttClient.connect(clientName);  // try first time to connect to broker.
  while(mqttCon != 1){        // retry MQTT connection every 2 seconds
      Serial.println("con failed...");
      mqttCon = mqttClient.connect(clientName); // retry connection to broker
      delay(2000);          // every 2 seconds
        }
  if(mqttCon){          // Connected !
    Serial.println("con with MQTT server");
    digitalWrite(MQCON, HIGH);      // switch on MQTT connection indicator LED
    mqttClient.subscribe(subTopic);     // subscribe to all southbound messages
    }
    else Serial.println("no con with MQTT server");
#endif  // ETHNODETYPE

#ifdef RFNODETYPE
  // Do all of the RFNODETYPE unique setup() actions.
  // xxxx - decide what LEDs will be on a standard RF Node and ensure they get pinMode intialiased and flashed here, like they do for ETH NODE
  radio.initialize(FREQUENCY,NODEID,NETWORKID); // initialise radio
  #ifdef IS_RFM69HW
    radio.setHighPower(); // only for RFM69HW!
  #endif
  radio.encrypt(ENCRYPTKEY); // set radio encryption
  radio.promiscuous(promiscuousMode); // only listen to closed network
  wakeUp = true; // set flag to send wakeup message (because we have just started booted)
  delay(5000); // xxxx - do I need this?
  radio.sleep();  // xxxx - do I need this?
#endif  // RFNODETYPE

// Do any device specific initialisations (note these cmds can't be used before setup()...compiler errors

#ifdef BUTTON1 // 1st Button ===============================================
  #ifdef DEBUGPJ2
    Serial.print("BUT1 PIN is:");
    Serial.print(BUTTON1PIN);
    Serial.println(": and setting it to Input");
  #endif
  pinMode(BUTTON1PIN, INPUT);
#endif
#ifdef BUTTON2 // 2nd Button ===============================================
  pinMode(BUTTON2PIN, INPUT);
#endif
 
#ifdef ACTUATOR1 // 1st Actuator ===============================================
  pinMode(ACTUATOR1PIN, OUTPUT);
  #ifdef ACTUATOR1REVERSE // for reversed Actuator behaviour do this
     digitalWrite(ACTUATOR1PIN, !actuator1status); // Set the ACTUATOR starting state. Reversed behaviour.
  #else // for normal Actuator behavior do this
     digitalWrite(ACTUATOR1PIN, actuator1status); // Set the ACTUATOR starting state.
  #endif
#endif
#ifdef ACTUATOR2 // 2nd ACTUATOR ===============================================
  pinMode(ACTUATOR2PIN, OUTPUT);
  #ifdef ACTUATOR2REVERSE // for reversed Actuator behaviour do this
     digitalWrite(ACTUATOR2PIN, !actuator2status); // Set the ACTUATOR starting state. Reversed behaviour.
  #else // for normal Actuator behavior do this
     digitalWrite(ACTUATOR2PIN, actuator2status); // Set the ACTUATOR starting state.
  #endif
#endif
#ifdef ACTUATOR3 // 3rd Actuator ===============================================
  pinMode(ACTUATOR3PIN, OUTPUT);
  #ifdef ACTUATOR3REVERSE // for reversed Actuator behaviour do this
     digitalWrite(ACTUATOR3PIN, !actuator3status); // Set the ACTUATOR starting state. Reversed behaviour.
  #else // for normal Actuator behavior do this
     digitalWrite(ACTUATOR3PIN, actuator3status); // Set the ACTUATOR starting state.
  #endif
#endif
#ifdef ACTUATOR4 // 4th ACTUATOR ===============================================
  pinMode(ACTUATOR4PIN, OUTPUT);
    #ifdef ACTUATOR4REVERSE // for reversed Actuator behaviour do this
     digitalWrite(ACTUATOR4PIN, !actuator4status); // Set the ACTUATOR starting state. Reversed behaviour.
  #else // for normal Actuator behavior do this
     digitalWrite(ACTUATOR4PIN, actuator4status); // Set the ACTUATOR starting state.
  #endif
#endif

#ifdef PIR1
  pinMode(PIR1PIN,INPUT);
  lastPIR1 = millis();
#endif
#ifdef PIR2
  pinMode(PIR2PIN,INPUT);
  lastPIR2 = millis();
#endif
#ifdef PIR1 or PIR2  
  Serial.print("cal PIR(s) ");
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
  Serial.println("");
#endif
  
#ifdef DHT
  delay(50);
  dht.begin(); // initialise temp/hum sensor
#endif
  
#ifdef DS18
  delay(50);
  sensors.begin(); // Onewire: Start up the library
#endif

#ifdef SLEEPY
    // Configure wake up pin as input.
    // This will consumes few uA of current.
    pinMode(wakeUpPin, INPUT);
#endif

#ifdef LEDSTRIP
   #if LEDSTRIPS_REMOTE  // if LED strips are not local...
     Serial1.begin(115200); // Initialise the 2nd hw serial port for inter Arduino serial link
   #else
    setupLEDStrips();  // do any prep work for any local led strips
  #endif
#endif


#ifdef I2CLCD // initialise the lcd
  lcd.init();         
  lcd.backlight();
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("RF Node:");
  lcd.setCursor(8, 0);
  lcd.print(NODEID);
  lcd.setCursor(14, 0);
  lcd.print("v");
  lcd.print(6);
#endif

#ifdef XMASLIGHTS // initialise xmas lights specific stuff 
  lcd.setCursor(0, 1);  // assumption is we always have LCD if we have XMASLIGHTS
  lcd.print("XmasMode:");
  lcd.print(XmasLightsMode);  

  pinMode(READY_OUT_PIN, OUTPUT);
  pinMode(DATA_OUT_16_PIN, OUTPUT);
  pinMode(DATA_OUT_8_PIN, OUTPUT);
  pinMode(DATA_OUT_4_PIN, OUTPUT);
  pinMode(DATA_OUT_2_PIN, OUTPUT);
  pinMode(DATA_OUT_1_PIN, OUTPUT);
  // put all zeros on DATA_OUT pins and then allow it to be read.
  //  i.e until this node is told otherwise, its telling the connected Arduino to use Mode 0
//  digitalWrite(READY_OUT_PIN, LOW);
//  digitalWrite(DATA_OUT_16_PIN, LOW);
//  digitalWrite(DATA_OUT_8_PIN, LOW);
//  digitalWrite(DATA_OUT_4_PIN, LOW);
//  digitalWrite(DATA_OUT_2_PIN, LOW);
//  digitalWrite(DATA_OUT_1_PIN, LOW);
//  digitalWrite(READY_OUT_PIN, HIGH);

  setXmasLightsMode();  // output whatever the default in "XmasLightsMode" as a starting position.
  
#endif //XMASLIGHTS

#ifdef MOTEINOWEATHER
  // insert any setup() code for this specific device here.
  pinMode(MOTEINO_WEATHERSHIELD_V_VALUE_PIN, INPUT);  // Setup the pin the Moteino can analog read the Vin/Batt level on, as per cct on WeatherShield.

  if (SI7021sensor.begin())
    Serial.println("SI7021Ok");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.
    Serial.println("SI7021fail");
    #ifdef DEBUG
      while(1); // Pause forever.
    #endif
  }

  // **********************************************
  // I have commented out all of this code that reads the BMP180 on the WeatherShield as my current WeatherShield is
  // missing a BMP180 chip.  See my Evernotes. 
  
  // if (BMP180pressure.begin())
  //   Serial.println("BMP180ok");
  // else
  // {
  //   // Oops, something went wrong, this is usually a connection problem,
  //   // see the comments at the top of this sketch for the proper connections.

  //   Serial.println("BMP180fail");
  //   //while(1); // Pause forever.
  // }
#endif // MOTEINOWEATHER

#ifdef TSL2651
  // insert any setup() code for this specific device here.
  #ifdef DEBUGPJ2
    Serial.println("TSL2651 Setup code start");
  #endif
  /* Initialise the sensor */
  if(!tsl.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.print("no TSL2561");
    while(1);
  }
  /* Display some basic information on this sensor */
  #ifdef DEBUGPJx
    displaySensorDetails();
  #endif
//  #ifdef DEBUG
//    Serial.println("TSL2651 disp done");
//  #endif
  /* Setup the sensor gain and integration time */
  configureSensor();
  /* We're ready to go! */
  #ifdef DEBUGPJ2
    Serial.println("TSL2651 Setup code done");
  #endif
#endif // TSL2651

#ifdef SERIALSLAVE
  Serial1.begin(9600); // Initialise the 2nd hw serial port for inter Arduino serial link
#endif

#ifdef RMT_PWR  // does this node use remote power switching?
  pinMode(RMT_PWR_ENA_PIN, OUTPUT);
  RMT_PWROff();  // don't turn on remote ATX PSU until its needed.
#endif //RMT_PWR
  
#ifdef PING1x
  pinMode(PING1TRIGGERPIN, OUTPUT); 
	pinMode(PING1ECHOPIN, INPUT);
#endif

#ifdef RTC
  #ifdef DEBUGPJ2
    Serial.println("RTC setup");
  #endif
  if (rtc.begin()) 
    {
    Serial.println("Found RTC");
    if (rtc.isrunning()) 
      {
      Serial.println("RTC is running!");
      rtcOk=true;
      }
    else
      { 
      Serial.println("RTC is NOT running!");
      rtcOk=false;
      // xxxx - should trigger the send of an error MQTT msg here.
      }
    }
  else
    {
    rtcOk=false;
    Serial.println("Couldn't find RTC");
    // xxxx - should trigger the send of an error MQTT msg here.
    }
#endif

#ifdef LCDGENERIC // dev320 - 329 LCD (Generic one) (#define LCDGENERIC)
  #ifdef LCDGENERIC_SFGLCD  // items specific to the SparkFun LCD-09351
    // delay(1200);///wait for the one second spalsh screen before anything is sent to the LCD.
    #ifdef DEBUGPJ2
      Serial.println("initalising LCD");
    #endif
    lcdGen.setBacklight(50); // xxxx should be LCDGENERIC_BRIGHTNESS not set at 50 // 0-100 are the levels from off to full brightness
    lcdGen.setHome();    //set the cursor back to 0,0.
    lcdGen.clearScreen();//clear anything that may have been previously printed ot the screen.
    
    delay(100);  // allow screen time to process else it gets confused :(
    
    lcdGen.printStr("PJ HA Node:");
    lcdGen.printNum(int(NODEID));
    lcdGen.nextLine();
    lcdGen.printStr("SW Ver:");
    lcdGen.printStr(VERSION);
    lcdGen.nextLine();
    
    delay(100);  // allow screen time to process else it gets confused :(
    
    #ifdef RFNODETYPE
      lcdGen.printStr("RF Node Type, Freq:");
      //lcdGen.printStr(FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
      lcdGen.printStr(" Mhz");
      lcdGen.nextLine();
    #endif
    #ifdef ETHNODETYPE
      lcdGen.printStr("ETH Node Type");
      delay(100);   // allow screen time to process else it gets confused :(
      lcdGen.nextLine();
      lcdGen.printStr("MyIP:");
      for (int i = 0; i < 4; i++)
        {
        lcdGen.printNum(ip[i]);
        lcdGen.printStr(".");
        }
      lcdGen.nextLine();  
    #endif  
  #endif 

  #ifdef LCDGENERIC_BLAH  // items specific to the blah LCD
    
  #endif 
#endif

#ifdef LCDNEXTION_FPS  // dev330 Nextion LCD used on FPS Node specifically
    #ifdef DEBUGPJ2
      Serial.println("initalising Nextion LCD for FPS");
    #endif
    
    /* Set the baudrate which is for debug and communicate with Nextion screen. */
    nexInit();
    dbSerialPrintln("Nextion nexInit() done");  // send debug info to Serial0 i.e. show up in Arduino IDE Serial Monitor Window
    
    // display initialisation text just to confirm display is working.  Will be overwritten by proper text very quickly.
    strcpy(NextionLCD_FPS_buffer, "Title Initialised");
    text0.setText(NextionLCD_FPS_buffer);
    strcpy(NextionLCD_FPS_buffer, "Instruction Initialised");
    scroll_inst.setText(NextionLCD_FPS_buffer);
    strcpy(NextionLCD_FPS_buffer, "Node Data Initialised");
    scroll0.setText(NextionLCD_FPS_buffer);

    delay(1500);

    // display starting or permanent text now.
    strcpy(NextionLCD_FPS_buffer, "Jeffos Home   Door Lock");
    text0.setText(NextionLCD_FPS_buffer);

    strcpy(NextionLCD_FPS_buffer, "Node Data >> "); // clear the buffer ready for some concats.
    strcat(NextionLCD_FPS_buffer, "PJ HA Node:");
    strcat(NextionLCD_FPS_buffer, int(NODEID));
    strcat(NextionLCD_FPS_buffer, "  SW Ver:");
    strcat(NextionLCD_FPS_buffer, VERSION);
        
    #ifdef RFNODETYPE
      strcat(NextionLCD_FPS_buffer, "  RF Node Type - Freq:");
      //lcdGen.printStr(FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
      strcat(NextionLCD_FPS_buffer," xx Mhz");
    #endif
    #ifdef ETHNODETYPE
      strcat(NextionLCD_FPS_buffer, "  ETH Node Type - ");
      strcat(NextionLCD_FPS_buffer, "MyIP:");
      for (int i = 0; i < 4; i++)
        {
        strcat(NextionLCD_FPS_buffer, int(ip[i]));
        strcat(NextionLCD_FPS_buffer, ".");
        }
    #endif  
    scroll0.setText(NextionLCD_FPS_buffer);
#endif 



#ifdef FINGER
  Serial.println("FINGER setup() started");
  // set the data rate for the sensor serial port
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);  // xxxx - remove after debugging, but maybe send an error?
  }
  Serial.println("FINGER setup() complete"); 
#endif

#ifdef EXTVAR40X
  #ifdef DEBUGPJ2
      Serial.println("initalising EXTVAR40X");
  #endif
#endif  // EXTVAR40X

#ifdef OCEANMIRROR
  #ifdef DEBUGPJ2
      Serial.println("initalising OCEANMIRROR");
  #endif
  Serial1.begin(115200); // Initialise the 2nd hw serial port for inter Arduino serial link
#endif
} // END setup()
