//===================== SETUP ========================================

#include "a.h" // My global defines and extern variables to help multi file comilation.

void setup() {

#ifdef SLEEPY // uncomment if flash on board
/*  if (flash.initialize()) {
      CONSOLE_PORT.println("Flash Init OK!");
      flash.sleep();          // put flash (if it exists) into low power mode
      CONSOLE_PORT.println("Flash sleeping");
    } else {
      CONSOLE_PORT.println("Flash Init FAIL!");
    } */
#endif


delay(5000);  // Rather than using "while (!Serial);" a better way to ensure 
              // Serial is ready on boards like Feathers, that does not block
              // the whole sketch from running if no USB plugged in. But still gives device
              // time to get it going so I don't miss first few debug outputs.
CONSOLE_PORT.begin(SERIAL_BAUD); // Initialise the 1st hw serial port for Arduino IDE Serial Monitor
#ifdef GENERATE_SERIAL2 && (CONSOLE_PORT == Serial2)
  // Assign pins 10 & 11 SERCOM functionality
  pinPeripheral(SERIAL2_TXPIN, PIO_SERCOM);
  pinPeripheral(SERIAL2_RXPIN, PIO_SERCOM);
#endif


#ifdef DEBUG
  CONSOLE_PORT.println("DEBUG enabled at compile time.");
#endif

#ifdef DEBUGPJ1
  CONSOLE_PORT.println("DEBUGPJ1 enabled at compile time.");
#endif

#ifdef DEBUGPJ2
  CONSOLE_PORT.println("DEBUGPJ2 enabled at compile time.");
#endif


#ifdef DEBUGPJ2
  CONSOLE_PORT.print( F("Compiled: "));
  CONSOLE_PORT.print( F(__DATE__));
  CONSOLE_PORT.print( F(", "));
  CONSOLE_PORT.println( F(__TIME__));
  
  CONSOLE_PORT.println(F("PJ HA Unified Node"));
  CONSOLE_PORT.print("N ");
  CONSOLE_PORT.print(NODEID);
  CONSOLE_PORT.print(" ");
  CONSOLE_PORT.println(VERSION);
  #ifdef RFNODETYPE
    CONSOLE_PORT.println("RF Node Type");
    CONSOLE_PORT.print("Freq ");
    CONSOLE_PORT.print(915);
    CONSOLE_PORT.println(" Mhz");
  #endif
  #ifdef ETHNODETYPE
    CONSOLE_PORT.println("ETH Node Type");
    CONSOLE_PORT.print("MAC ");
    for (int i = 0; i < 6; i++)
      {
      CONSOLE_PORT.print(mac[i], HEX);
      CONSOLE_PORT.print(":");
      }
    CONSOLE_PORT.println();  
    CONSOLE_PORT.print("Node IP Addr: ");
    for (int i = 0; i < 4; i++)
      {
      CONSOLE_PORT.print(ip[i]);
      CONSOLE_PORT.print(".");
      }
    CONSOLE_PORT.println();  
    CONSOLE_PORT.print("MQTT Broker: ");
    for (int i = 0; i < 4; i++)
      {
      CONSOLE_PORT.print(mqtt_server[i]);
      CONSOLE_PORT.print(".");
      }
    CONSOLE_PORT.println();    
  #endif  
#endif




#ifdef BEEPER
  for (int i = 0; i < 4; i++)
    {
      tone(BEEPERPIN, beeperNotes[i], beeperDurations[i]);
      delay(250);
    }
#endif // BEEPER



  // Setup indicator LEDs to initial state
  #ifdef DEBUGPJ2
    CONSOLE_PORT.println("Starting LED flash"); 
  #endif
  pinMode(COMMS_LED_PIN, OUTPUT);   // set pin of IP indicator
#ifdef ETHNODETYPE  
  pinMode(MQCON, OUTPUT);           // set pin for MQTT connection indicator
#endif
  pinMode(STATUS_LED_PIN, OUTPUT);  // set pin for Status indicator
#ifdef ETHNODETYPE    
  digitalWriteHighPower(MQCON, LOW);         // switch off MQTT connection indicator
#endif
  digitalWriteHighPower(COMMS_LED_PIN, LOW); // switch off IP indicator
  digitalWriteHighPower(STATUS_LED_PIN, LOW);         // switch ON Power/Startup indicator
  // test all indicator LEDS - flash all LEDS 3 times. Then leave just STATUS LED on.
  delay(300);
#ifdef ETHNODETYPE    
  digitalWriteHighPower(MQCON, HIGH_HP);         // switch all LEDS ON
#endif
  digitalWriteHighPower(COMMS_LED_PIN, HIGH_HP);       
  digitalWriteHighPower(STATUS_LED_PIN, HIGH_HP);        
  delay(300);
#ifdef ETHNODETYPE    
  digitalWriteHighPower(MQCON, LOW);         // switch all LEDS OFF
#endif
  digitalWriteHighPower(COMMS_LED_PIN, LOW);       
  digitalWriteHighPower(STATUS_LED_PIN, LOW);        
  delay(300);
#ifdef ETHNODETYPE    
  digitalWriteHighPower(MQCON, HIGH_HP);         // switch all LEDS ON
#endif
  digitalWriteHighPower(COMMS_LED_PIN, HIGH_HP);       
  digitalWriteHighPower(STATUS_LED_PIN, HIGH_HP);        
  delay(300);
#ifdef ETHNODETYPE    
  digitalWriteHighPower(MQCON, LOW);         // switch all LEDS OFF
#endif
  digitalWriteHighPower(COMMS_LED_PIN, LOW);       
  digitalWriteHighPower(STATUS_LED_PIN, LOW);        
  delay(300);
#ifdef ETHNODETYPE    
  digitalWriteHighPower(MQCON, HIGH_HP);         // switch all LEDS ON
#endif
  digitalWriteHighPower(COMMS_LED_PIN, HIGH_HP);       
  digitalWriteHighPower(STATUS_LED_PIN, HIGH_HP);        
  delay(300);
#ifdef ETHNODETYPE    
  digitalWriteHighPower(MQCON, LOW);         // switch all LEDS OFF but leave Power LED ON
#endif
  digitalWriteHighPower(COMMS_LED_PIN, LOW);             
  delay(300);
  #ifdef DEBUGPJ2
    CONSOLE_PORT.println("Ended LED flash"); 
  #endif
#ifdef ETHNODETYPE   // Do all of the ETHNODETYPE unique setup() actions.
// Setup Ethernet port
  Ethernet.begin(mac, ip);     // start the Ethernet connection with static IP
  // Make initial connection to MQTT Broker via IP
  CONSOLE_PORT.println("MQTT connecting...");
  mqttCon = 0;          // reset connection flag
  mqttCon = mqttClient.connect(clientName);  // try first time to connect to broker.
  while(mqttCon != 1){        // retry MQTT connection every 2 seconds
      CONSOLE_PORT.println("con failed...");
      mqttCon = mqttClient.connect(clientName); // retry connection to broker
      delay(2000);          // every 2 seconds
        }
  if(mqttCon){          // Connected !
    CONSOLE_PORT.println("con with MQTT server");
    digitalWriteHighPower(MQCON, HIGH_HP);      // switch on MQTT connection indicator LED
    mqttClient.subscribe(subTopic);     // subscribe to all southbound messages
    }
    else CONSOLE_PORT.println("no con with MQTT server");
#endif  // ETHNODETYPE

#ifdef RFNODETYPE
  // Do all of the RFNODETYPE unique setup() actions.
  // xxxx - decide what LEDs will be on a standard RF Node and ensure they get pinMode intialiased and flashed here, like they do for ETH NODE
  
  #ifdef DEBUGPJ2
    CONSOLE_PORT.println("radio initialisation starting.");
  #endif

  // RadioHead radio initialisation code
   if (!manager.init())
    CONSOLE_PORT.println("init FAILED");
  else
    CONSOLE_PORT.println("init succeded");
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM (for low power module)
  // No encryption
  
  if (!driver.setFrequency(915.0))
    CONSOLE_PORT.println("setFrequency FAILED");
  else
    CONSOLE_PORT.println("setFrequency succeded");
    
  // If you are using a high power RF69 eg RFM69HW, you *must* set a Tx power with the
  // ishighpowermodule flag set like this:
  driver.setTxPower(13, true);
    
  // The encryption key has to be the same as the one in the client
  uint8_t key[] = { 0x02, 0x03, 0x07, 0x02, 0x06, 0x06, 0x01, 0x01,
                    0x02, 0x03, 0x07, 0x02, 0x06, 0x06, 0x01, 0x01};
  driver.setEncryptionKey(key); CONSOLE_PORT.println("Encryption ENABLED");
  //CONSOLE_PORT.println("Encryption DISABLED");

  #ifdef DEBUGPJ2
    CONSOLE_PORT.println("radio initialisation is done.");
  #endif

  // Below is the no longer needed code from pre RadioHead days.
  // #ifdef FEATHERM0RFM69
  //     // Hard Reset the RFM module
  //     pinMode(RFM69_RST, OUTPUT);
  //     digitalWrite(RFM69_RST, HIGH);
  //     delay(100);
  //     digitalWrite(RFM69_RST, LOW);
  //     delay(100);
  // #endif  
  // radio.initialize(FREQUENCY,NODEID,NETWORKID); // initialise radio
  // #ifdef IS_RFM69HW
  //   radio.setHighPower(); // only for RFM69HW!
  // #endif
  // radio.setPowerLevel(31); // power output ranges from 0 (5dBm) to 31 (20dBm)
  // radio.encrypt(ENCRYPTKEY); // set radio encryption
  // radio.promiscuous(promiscuousMode); // only listen to closed network
  // wakeUp = true; // set flag to send wakeup message (because we have just started booted)
  // delay(5000); // xxxx - do I need this?
  // radio.sleep();  // xxxx - do I need this?

#endif  // RFNODETYPE

// Do any device specific initialisations (note these cmds can't be used before setup()...compiler errors

#ifdef BUTTON1 // 1st Button ===============================================
  #ifdef DEBUGPJ2
    CONSOLE_PORT.print("BUT1 PIN is:");
    CONSOLE_PORT.println(BUTTON1PIN);
  #endif
  #ifdef FEATHERM0RFM69
    pinMode(BUTTON1PIN, INPUT_PULLUP); // assumption is I'm using the built in pullups available on the M0.
  #else
    pinMode(BUTTON1PIN, INPUT);
  #endif
#endif

#ifdef BUTTON2 // 2nd Button ===============================================
  #ifdef FEATHERM0RFM69
    pinMode(BUTTON2PIN, INPUT_PULLUP); // assumption is I'm using the built in pullups available on the M0.
  #else
    pinMode(BUTTON2PIN, INPUT);
  #endif
#endif

#ifdef SWITCH1 // 1st Switch ===============================================
  #ifdef DEBUGPJ2
    CONSOLE_PORT.print("SW1 PIN is:");
    CONSOLE_PORT.println(SWITCH1PIN);
  #endif
  #ifdef FEATHERM0RFM69
    pinMode(SWITCH1PIN, INPUT_PULLUP); // assumption is I'm using the built in pullups available on the M0.
  #else
    pinMode(SWITCH1PIN, INPUT);
  #endif
  checkswitches();  // Get initial state of SWITCHes at reset/powerup, set curSwitchXState variables
#endif

#ifdef SWITCH2 // 2nd Switch ===============================================
  #ifdef DEBUGPJ2
    CONSOLE_PORT.print("SW2 PIN is:");
    CONSOLE_PORT.println(SWITCH2PIN);
  #endif
  #ifdef FEATHERM0RFM69
    pinMode(SWITCH2PIN, INPUT_PULLUP); // assumption is I'm using the built in pullups available on the M0.
  #else
    pinMode(SWITCH2PIN, INPUT);
  #endif
  checkswitches();  // Get initial state of SWITCHes at reset/powerup, set curSwitchXState variables
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
  CONSOLE_PORT.print("cal PIR(s) ");
    for(int i = 0; i < calibrationTime; i++){
      CONSOLE_PORT.print(".");
      delay(1000);
      }
  CONSOLE_PORT.println("");
#endif
  
#ifdef DHTSENSOR
  delay(50);
  dht.begin(); // initialise temp/hum sensor
#endif
  
#ifdef DS18
  delay(50);
  sensors.begin(); // Onewire: Start up the library
#endif

#ifdef SOILMOISTURE1
  pinMode(SOILMOISTUREPIN1, INPUT); //Set as INPUT 
  pinMode(SOILPOWERPIN1, OUTPUT);   //Set as an OUTPUT
  digitalWrite(SOILPOWERPIN1, LOW); //Set to LOW so no power is flowing through the sensor when not in use.
#endif

#ifdef SOILMOISTURE2
  pinMode(SOILMOISTUREPIN2, INPUT); //Set as INPUT 
  pinMode(SOILPOWERPIN2, OUTPUT);   //Set as an OUTPUT
  digitalWrite(SOILPOWERPIN2, LOW); //Set to LOW so no power is flowing through the sensor when not in use.
#endif

#ifdef SLEEPY
    // Configure wake up pin as input.
    // This will consumes few uA of current.
    pinMode(wakeUpPin, INPUT);
#endif

#ifdef LEDSTRIP
   #ifdef LEDSTRIPS_REMOTE  // if LED strips are not local...
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
    CONSOLE_PORT.println("SI7021Ok");
  else
  {
    // Oops, something went wrong, this is usually a connection problem,
    // see the comments at the top of this sketch for the proper connections.
    CONSOLE_PORT.println("SI7021fail");
    #ifdef DEBUG
      while(1); // Pause forever.
    #endif
  }

  // **********************************************
  // I have commented out all of this code that reads the BMP180 on the WeatherShield as my current WeatherShield is
  // missing a BMP180 chip.  See my Evernotes. 
  
  // if (BMP180pressure.begin())
  //   CONSOLE_PORT.println("BMP180ok");
  // else
  // {
  //   // Oops, something went wrong, this is usually a connection problem,
  //   // see the comments at the top of this sketch for the proper connections.

  //   CONSOLE_PORT.println("BMP180fail");
  //   //while(1); // Pause forever.
  // }
#endif // MOTEINOWEATHER

#ifdef TSL2651
  // insert any setup() code for this specific device here.
  #ifdef DEBUGPJ2
    CONSOLE_PORT.println("TSL2651 Setup code start");
  #endif
  /* Initialise the sensor */
  if(!tsl.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    CONSOLE_PORT.print("no TSL2561");
    while(1);
  }
  /* Display some basic information on this sensor */
  #ifdef DEBUGPJx
    displaySensorDetails();
  #endif
//  #ifdef DEBUG
//    CONSOLE_PORT.println("TSL2651 disp done");
//  #endif
  /* Setup the sensor gain and integration time */
  configureSensor();
  /* We're ready to go! */
  #ifdef DEBUGPJ2
    CONSOLE_PORT.println("TSL2651 Setup code done");
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
    CONSOLE_PORT.println("RTC setup");
  #endif
  if (rtc.begin()) 
    {
    CONSOLE_PORT.println("Found RTC");
    if (rtc.isrunning()) 
      {
      CONSOLE_PORT.println("RTC is running!");
      rtcOk=true;
      }
    else
      { 
      CONSOLE_PORT.println("RTC is NOT running!");
      rtcOk=false;
      // xxxx - should trigger the send of an error MQTT msg here.
      }
    }
  else
    {
    rtcOk=false;
    CONSOLE_PORT.println("Couldn't find RTC");
    // xxxx - should trigger the send of an error MQTT msg here.
    }
#endif

#ifdef LCDGENERIC // dev320 - 329 LCD (Generic one) (#define LCDGENERIC)
  #ifdef LCDGENERIC_SFGLCD  // items specific to the SparkFun LCD-09351
    // delay(1200);///wait for the one second spalsh screen before anything is sent to the LCD.
    #ifdef DEBUGPJ2
      CONSOLE_PORT.println("initalising LCD");
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
      CONSOLE_PORT.println("initalising Nextion LCD for FPS");
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
  CONSOLE_PORT.println("FINGER setup() started");
  // set the data rate for the sensor serial port
  finger.begin(57600);
  if (finger.verifyPassword()) {
    CONSOLE_PORT.println("Found fingerprint sensor!");
  } else {
    CONSOLE_PORT.println("Did not find fingerprint sensor :(");
    while (1);  // xxxx - remove after debugging, but maybe send an error?
  }
  CONSOLE_PORT.println("FINGER setup() complete"); 
#endif

#ifdef EXTVAR40X
  #ifdef DEBUGPJ2
      CONSOLE_PORT.println("initalising EXTVAR40X");
  #endif
#endif  // EXTVAR40X

#ifdef OCEANMIRROR
  #ifdef DEBUGPJ2
      CONSOLE_PORT.println("initalising OCEANMIRROR");
  #endif
  Serial1.begin(115200); // Initialise the 2nd hw serial port for inter Arduino serial link
#endif

#ifdef VEML7700
  // insert any setup() code for this specific device here.
  #ifdef DEBUGPJ2
    CONSOLE_PORT.println("VEML7700 Setup code start");
  #endif
  /* Initialise the sensor */
  if (!veml.begin()) {
    CONSOLE_PORT.println("Sensor not found");
    while (1);
  }
  CONSOLE_PORT.println("Sensor found");

  veml.setGain(VEML7700_GAIN_1);
  veml.setIntegrationTime(VEML7700_IT_800MS);

  CONSOLE_PORT.print(F("Gain: "));
  switch (veml.getGain()) {
    case VEML7700_GAIN_1: CONSOLE_PORT.println("1"); break;
    case VEML7700_GAIN_2: CONSOLE_PORT.println("2"); break;
    case VEML7700_GAIN_1_4: CONSOLE_PORT.println("1/4"); break;
    case VEML7700_GAIN_1_8: CONSOLE_PORT.println("1/8"); break;
  }

  CONSOLE_PORT.print(F("Integration Time (ms): "));
  switch (veml.getIntegrationTime()) {
    case VEML7700_IT_25MS: CONSOLE_PORT.println("25"); break;
    case VEML7700_IT_50MS: CONSOLE_PORT.println("50"); break;
    case VEML7700_IT_100MS: CONSOLE_PORT.println("100"); break;
    case VEML7700_IT_200MS: CONSOLE_PORT.println("200"); break;
    case VEML7700_IT_400MS: CONSOLE_PORT.println("400"); break;
    case VEML7700_IT_800MS: CONSOLE_PORT.println("800"); break;
  }

  //veml.powerSaveEnable(true);
  //veml.setPowerSaveMode(VEML7700_POWERSAVE_MODE4);

  veml.setLowThreshold(10000);
  veml.setHighThreshold(20000);
  veml.interruptEnable(true);
  #ifdef DEBUGPJ2
    CONSOLE_PORT.println("VEML7700 Setup code done");
  #endif
#endif // VEML7700



} // END setup()
