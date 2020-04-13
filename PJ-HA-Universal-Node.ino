char foo;
/* PJ HA Unified Node
 * ==================
 * June 2016 - I am going to take my current RF Node (PJ_HA_computourist_node_v2_23v14) and my
 * current Eth Node (Shed_HA_Mega_ard_v4) and try to build them into one Sketch with lots
 * of DEFINE statements to allow selective builds, while retaining and maintaining one code base.
 * Sept 2016 - Migrated this project from Arduino IDE to GitHub etc.
 *
 * RF Node Type - specifics
 * ------------
 * As per previous work, this form of a node will be RF connected to an RFM_GW which will
 * relay and translate messages to and from the IP based MQTT Broker.
 * 
 * RF Message format is: nodeID/deviceID/command/integer/float/string
 *      - Depending on the type of data (integer, float or string) one of the payload variables is used
 *      - Command = 0 means write a value in the node, cmd = 1 means read a value
 *
 * Eth Node Type - specifics
 * -------------
 * As per previous work, this form of the node will directly communicate via IP to the
 * IP based MQTT Broker. No intermediate gateway required. 
 * 
 * IP/MQTT topic format is home/eth_nd/direction/nodeXX/devXXX (note: eth_nd not rfm_gw!)
 *      - where direction is: southbound (sb) towards this node and northbound (nb) towards MQTT broker
 *
 * All Node Types - commonalities
   -------------- */
// A Node talks to the MQTT Broker (directly or via a gateway) and will:
//      - send sensor data or actuator status periodically and on-demand
//      - receive commands from the MQTT Broker to set actuator status
//      - receive commands from the MQTT Broker to set onboard settings
//
// Several nodes can operate within a single network; each have a unique node ID.
// On startup the node operates with default values, set at time of compilation.
//
// ----------------------------------------------------------------------------------------------------------
// XXXXXXXX  PJ additional modes see my EverNote titled "RFM Message Structure & Device Functions - PJ Notes"
// Current defined node devices are:
// 0 uptime: read uptime of node in minutes
// 1 node: read/set - periodic transmission interval in seconds, 0 means no periodic transmission
// 2 RSSI: read - radio signal strength (RF Node Type only)
// 3 Version: read - version node software
// 4 voltage: read - battery level
// 5 ACK: read/set - acknowledge message after a 'set' request
// 6 toggle: read/set - toggle function on button press
// 7 timer: read/set - activation timer after button press in seconds, 0 means no timer
//  
//
// XXXXXXXX  PJ additional modes see my EverNote titled "RFM Message Structure & Device Functions - PJ Notes"
// ----------------------------------------------------------------------------------------------------------
//
// The button can be set to:
// - generate a message on each press (limited to one per 10 seconds) and/or
// - toggle the output ACT1 (local node function) or
// - activate the output for a fixed time period (local node function)
//
// When talking to openHab over MQTT, the following item config will pick up the PIR ON msg correctly:
// String itm_node2_pir_mqtt "PIR [%s]" {mqtt="<[mymosquitto:home/rfm_gw/nb/node02/dev41:command:on]"} // (change node to your node)
//
// Multiple debug modes are included, which outputs messages on the serial output.
//      - see DEFINEs
//
// RFM69 Library and cool Moteino stuff by Felix Rusu - felix@lowpowerlab.com
// Get the RFM69 library at: https://github.com/LowPowerLab/
//
// Huge amount of code and entire system is based on work by Computourist@gmail.com and others
// at http://homeautomation.proboards.com/board/2/openhab-rfm69-based-arduino
// I owe a huge debt of gratitude to that team. Please go and check out thier great work.
// That work is based in turn on Eric Tsai's work at
// http://www.instructables.com/id/Uber-Home-Automation-w-Arduino-Pi/
// which is what got me going in teh first place. Massive thanks, massive!
//
// Change Log
// ==========
//  Version   Date            Description of Change
//  v4_1      [21-23]-06-16   Initial file creation of the Unified Node, based on sources mentioned above.
//  v4_2-3                    Successfully tested this codebase running as an ETHNODE, plus some devices
//                            such as BUTTON1, ACTUATOR1-3 etc.
//  v4_4      [1-3]-07-16     Created new PING and RTC devices and successfully tested them. 
//                            Started on my new generic LCD device.
//  v4_5-6    [6-7]-7-16      Continued on new generic LCD device, and successfully tested it.
//  v4_7      7-7-16          Start on new BEEPER device.
//  v4_8      8-7-16          BEEPER device complete and working.
//  v4_9      9-7-16          Start on FINGER device
//  v4_10     10-7-16         Fair bit of FINGER device all working, just need to tidy up and test more
//  v4_11     12-7-16         FINGER device complete and working.
//  v4_12     14-07-16        Change IP_LED to be the COMMS_LED and set it up to be triggered at any valid RF/IP tx/rx of this node.
//                            Also changed the P_LED (Power LED) to be STATUS LED, as I have power LEDs directly across the power rail.
//                            At the same time I removed the FPS STATUS LED as its not needed now that the main node STATUS LED is in place.
//                            Also fixed send350 stuff in dosends(), as I had not put it in yet!  
//  v5_00     30-07-16        No change to 4_12, just made a major version number change
//                                as 4_12 was at a good solid point and now I want to start
//                                using that code base for my new 'Ocean Mirror' node.
//  v5_01     30-12-16        Started changing the defines to I had just what I needed for Ocean Mirror node (Moteino Mega)
//                                - had to add some of the RF only function prototypes and variable externs to a.h
//                                - got RF node working, blue LED was flashing each second, node was sending update every 120sec etc.  READ of dev000 worked.
//                                - next step is to define some devices on it.
//  v5_02     31-07-16        Start adding in the EXTVAR Devices.
//  v5_03     06-08-16        Added OCEANMIRROR stuff. Inc serial send to Due etc.
//  v5_04     07-08-16        Added in use of ExtVar401 to tx brightness and on/off to Due. Tested Ok.
//  v5_05     13-08-16        Enable RMT_PWR so Mega tells ATX PSU to supply 5v. Tested, seems to be working.
//  v Nothing 09-09-16        Migrated this project from Arduino IDE to GitHub etc. So no more version numbers
//                            All code changes will now be documented in GitHub.

#include "a.h" // My global defines and extern variables to help multi file comilation.

/* RF NODE TYPE CONFIGURATION PARAMETERS & LIBRARIES */
// These parameters are only used/required if RF Node Type else ignored.
#ifdef RFNODETYPE
    //#include <RFM69.h>
    #include <SPI.h>
    bool	promiscuousMode = false; // only listen to nodes within the closed network
    bool  wakeUp = true; // wakeup flag
    
    //RFM69 radio = RFM69(RFM69_CS, RFM69_IRQ, IS_RFM69HCW, RFM69_IRQN);
  

    // Define our RFM69 RadioHead 'driver'.
    RH_RF69 driver(RFM69_CS, RFM69_IRQ);  

    // RadioHead 'manager' to manage message delivery and receipt, using the driver declared above
    RHMesh manager(driver, CLIENT_ADDRESS);

    long lastRfTxTime = 0;  // helps gate how often rfSendMsg() is called.
    bool rfTxAllowed = true; // helps gate how often rfSendMsg() is called.
#endif

//
//==============  MQTT_SUBS
//
//    receives Southbound MQTT/IP messages from subscribed topics
//    parses MQTT topic / message and actions it.
//    
//    Note: This function is called by the PubSub library, not my code.  See the statement early in this sketch 
//          that says "PubSubClient mqttClient(mqtt_server, 1883, mqtt_subs, ethClient );", thats what registers
//          this function with the PubSub library.
//



void mqtt_subs(char* topic, byte* payload, unsigned int length) 
{ 
  // if we are here it's because the PubSub library received a 'topic' & 'payload' we are subscribed to and called this function for us.
  // so we have to check it and then do what needs to be done with it.
  CommsLEDStart = true; // set this flag so that the Comms LED will be turned on for a period and managed elsewhere.
  mqttToSend = false;       // not a valid request yet...
  error = 999;               // set it to 999 just so I can monitor it while debugging. 999 is not a real value.

  #ifdef DEBUGPJ
    Serial.println();
    Serial.println("=============================================");
    Serial.println("Southbound MQTT Topic received from Mosquitto");
    Serial.print("Topic is:");
    Serial.println(topic);
    Serial.print("Payload is:");
  #endif

  if (strlen(topic) != 28)        // is the 'topic' we received the correct length ?
                                  // originally was 27, now 28 as I have allowed for devID's > 99 hence three digit.
    {   // The 'topic' we received was not of correct length
    error = 1;
    #ifdef DEBUGPJ
        Serial.println(); // make sure I get a new line before printing below messages.
        Serial.println("mqtt_subs() - error = 1 (due to 'topic' being incorrect length)");
        //Serial.println("mqtt_subs() - wrong message format in MQTT subscription.");
    #endif
    mqtt_subs_senderror();
    }
    
  else if (length == 0) // no payload sent in the southbound MQTT msg...thats not valid so error out.
        {
        #ifdef DEBUGPJ
            Serial.println(); // make sure I get a new line before printing below messages.
            Serial.println("mqtt_subs() - error = 2 (due to length of payload = 0)");
        #endif
        error = 2;
        mqtt_subs_senderror();
        }                   

      else // topic correct length and payload present, so lets start to pull it apart...
        { 
        destNodeID = (topic[19]-'0')*10 + topic[20]-'0';  // extract target node ID from MQTT topic and store it in global 'destNodeID' for use later
        destDevID = (topic[25]-'0')*100 + (topic[26]-'0')*10 + topic[27]-'0';   // extract device ID from MQTT topic and stor it in global 'destDevID" for use later
        payload[length] = '\0';                     // terminate the payload string we received with '0'
        strPayload = String((char*)payload); // convert the payload to string, as it was a ptr to some bytes when we received it.
        #ifdef DEBUGPJ
            Serial.println(strPayload);
            Serial.println("=============================================");
        #endif
        if (destNodeID != NODEID)  // Somehow we have go this far but the MQTT topic is not for this Node!
          {
          #ifdef DEBUGPJ
            Serial.println(); // make sure I get a new line before printing below messages.
            Serial.println("mqtt_subs() - error = 10 (Somehow we have go this far but the MQTT topic is not for this Node!");
          #endif
          error = 10;
          mqtt_subs_senderror();       
          }
        else
          {
          // use what we have extracted and get to work on the MQTT instruction
          mes.devID = destDevID;
          mes.cmd = 0;          // default command is '0/SET/WRITE' value, it gets changed to 1/GET/READ below if nesc.
          if (strPayload == "READ") mes.cmd = 1;    // if the payload was 'READ' then change the command flag to 1 (i.e. GET/READ) else leave it at 0 (SET/WRITE)
          else
            { // xxxx - I need to have a close look at the below, is it even required? Consider removing it and the variables it uses.
              //
              // If it's not a READ, then its a WRITE, so classify the payload (Int, Real, String etc) based on the DevID. 
              // This allows us to extract it from the payload string correctly at the Node and WRITE it to the DeviceID.
              // Only one of the below four lines should match.
              
              StatMess = ( destDevID==5 || destDevID==6 || destDevID==8 || (destDevID>=16 && destDevID<32));  // Set global 'StatMess' to true based on devID.
                                                                                                            //    These special Integer destDevIDs can only be 0 or 1 
                                                                                                            //    not any int value.
              RealMess = (( destDevID==0 || destDevID==2 || destDevID==3 || destDevID==4 || (destDevID>=40 && destDevID<72))&& mes.cmd==1); // set global 'RealMess' to true based on devID.
                                                                                                                                          //    These destDevIDs can only be READ, not SET. 
                                                                                                                                          //    Makes no sense to SET them.
              IntMess = (destDevID==1 || destDevID==7 || (destDevID >=32 && destDevID <40) || (destDevID>=100 && destDevID<=116));          // set global 'IntMess' to true based on devID.
              StrMess = (destDevID==72);   // set global 'StrMess' to true based on devID.
            }
          
          mqtt_subs_switch();  // action the MQTT message
          }
      }

} // END - mqttSubs()


/* ETH NODE TYPE CONFIGURATION PARAMETERS & LIBRARIES */
// These parameters are only used/required if Eth Node Type else ignored.
#ifdef ETHNODETYPE
    #include <SPI.h>
    #include <Ethernet.h>
    #include <PubSubClient.h>
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x00, 0x23 };  // MAC address for ethernet << MUST CHANGE per node!!!!!>>
    byte mqtt_server[] = { 192, 168, 200, 241};   // MQTT broker address
    byte ip[] = { 192, 168, 200 , 238 };      // Gateway address (if DHCP fails) << MUST CHANGE per node.
    String strPayload;  // used in mqtt_subs() etc, when ingesting a new MQTT message.
    //void mqtt_subs(char* topic, byte* payload, unsigned int length);  // I had to put this prototype here or else I could not compile
    EthernetClient ethClient;
    PubSubClient mqttClient(mqtt_server, 1883, mqtt_subs, ethClient );
    int   destNodeID;       // The extracted destination Node ID from received IP/MQTT packet MQTT topic.
    int   destDevID;        // The extracted destination Device ID from received IP/MQTT packet MQTT topic
    int   error;            // Syntax error code

    bool  mqttCon = false;  // MQTT broker connection flag
    bool  respNeeded = false;       // MQTT message flag in case of radio connection failure
    bool  mqttToSend = false;       // Flag that is set when we have a southbound RF message to send out to remote Node during main loop() run through.
    bool  verbose = true;           // generate error messages
    bool  IntMess, RealMess, StatMess, StrMess; // types of messages

    char  *subTopic = SUBTOPICSTR;   // MQTT subscription topic ; direction is southbound
    char  *clientName = CLIENTNAMESTR;      // MQTT system name of gateway/node
    char  buff_topic[30];       // MQTT publish topic string
    char  buff_mess[32];        // MQTT publish message string
#endif



//-------------------------------------------------------------------------
// GENERAL STARTUP VARIABLES & DEFAULTS 
//-------------------------------------------------------------------------
long  TXinterval = 600; // periodic transmission interval in seconds (This is Dev001 for this Node)
long  TIMinterval = 20; // timer interval in seconds (This is Dev007 for this Node)
bool  toggleOnButton = true; // toggle output on button (This is Dev006 for this Node)
bool	ackButton = false; // flag for message on button press
long	lastPeriod = -1; // timestamp last transmission
long  lastBtnPress = -1; // timestamp last buttonpress
long  lastPIR = -1; // timestamp last PIR detection
long	lastMinute = -1; // timestamp last minute
long	upTime = 0; // uptime in minutes
float	hum, temp; // humidity, temperature
int	  signalStrength; // radio signal strength
bool  setAck = true; // send ACK message on 'SET' request (This is Dev005 for this node)
bool	send0, send1, send2, send3, send4, send5, send6, send7, send11, send12, send48, send49, send50, send92, send93, send94, send95 = false; // message triggers
bool  CommsLEDStart = false;    // Do we need to turn on the Comms LED?.
bool  CommsLEDStatus = false; // Comms LED current status...used in it's on/off logic.
long  CommsLEDonMillis;       // timestamp when Comms LED was turned on. Used to keep track of when to turn it off.
bool  StatusLEDStatus = false;  // Status LED current status...used in it's on/off logic.
long  StatusLEDonMillis;        // timestamp when Status LED was turned on. Used to keep track of when to turn it off.

bool  msgBlock = false; // flag to hold button messages to prevent overload

Message mes;

uint8_t radioDataBuf[RH_MESH_MAX_MESSAGE_LEN]; //RadioHead tx/rx data
//-------------------------------------------------------------------------



//-------------------------------------------------------------------------
// Device specific defines and variables
//-------------------------------------------------------------------------
// Prep additional DEFINES and variables based on the devices that are attached to node.



#ifdef DHTSENSOR // DHT11/22 sensor ================================================
  DHT dht(DHTPIN, DHTTYPE); // initialise temp/humidity sensor
#endif  // DHTSENSOR

#ifdef DS18 // OneWire DS18S20, DS18B20, DS1822 Temperature ==============
  // insert any pre setup() code for this specific device here.
  #include <OneWire.h>
  #include <DallasTemperature.h>
  #define ONE_WIRE_BUS 3 // Data wire is plugged into pin 3 on the Arduino
  //DeviceAddress Probe01 = { 0x28, 0x9C, 0x7C, 0x28, 0x06, 0x00, 0x00, 0x23 }; // sensor A
  //DeviceAddress Probe02 = { 0x28, 0x2B, 0x35, 0x28, 0x06, 0x00, 0x00, 0xBB }; // sensor B
  OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
  DallasTemperature sensors(&oneWire); // Pass our oneWire reference to Dallas Temperature.
#endif  // DS18

#ifdef SOILMOISTURE1
  bool send68 = false;
#endif

#ifdef SOILMOISTURE2
  bool send69 = false;
#endif

#ifdef SLEEPY
  #include <SPIFlash.h> for devices with onboard flash
  #include <LowPower.h> //https://github.com/rocketscream/Low-Power
  // Sleep the flash mem on either Anarduino or moteino, uncomment where applicable see also Flash code in setup section below
  // SPIFlash flash(5, 0); // flash(SPI_CS, MANUFACTURER_ID)  // Anarduino: Flash SPI_CS = 5, ID = 0xEF30 (Winbond 4Mbit flash)  
  // SPIFlash flash(8, 0xEF30); // flash(SPI_CS, MANUFACTURER_ID) // Moteino: Flash SPI_CS = 8, ID = 0xEF30 (Winbond 4Mbit flash)
  // For the PIR Use pin as defined in the PIR section as wake up pin
  const int wakeUpPin = PirInput;
  
  void wakeUpFromSleepy()
  {
      // Just a handler for the pin interrupt.
  }
#endif  // SLEEPY

#ifdef PIR1 
    bool send41 = false;
    int   calibrationTime = 2;        //seconds - the time we give the sensor to calibrate (10-60 secs according to the datasheet)
    int   Pir1Input = PIR1PIN; // PIR pin if also on sleepy this must be either PIN2 (attachInterrupt(0) or PIN3 (attachInterrupt(1)) for the interrupt to work
    long  lastPIR1 = -1; // timestamp last PIR1 detection. We initialise it to this negative value, so when checkPIR()
                                   //  is called the first time within the first PIRHOLDOFF period, it still triggers
    bool  curPIR1State = true; // current PIR state
#endif

#ifdef PIR2
    bool send43 = false;
    int   Pir2Input = PIR2PIN; // PIR pin if also on sleepy this must be either PIN2 (attachInterrupt(0) or PIN3 (attachInterrupt(1)) for the interrupt to work
    long  lastPIR2 = -1;  // timestamp last PIR2 detection
    bool  curPIR2State = true; // current PIR state
#endif

#ifdef BUTTON1
    bool send40 = false;
    boolean button1status = OFF;  // consider it as OFF until first read of the button  xxxx - is this used anywhere?
    long  lastBtn1Press = -1; // timestamp last buttonpress
    bool  curBtn1State = true; // current button state
    bool  lastBtn1State = true; // last button state
    bool  timerOnButton1 = false; // timer output on button press
    bool  ackButton1 = false; // flag for message on button press
    bool  toggleOnButton1 = false; // toggle related actuator output on button press
#endif

#ifdef BUTTON2
    bool send42 = false;
    boolean button2status = OFF;
    long  lastBtn2Press = -1; // timestamp last buttonpress
    bool  curBtn2State = true; // current button state
    bool  lastBtn2State = true; // last button state
    bool  timerOnButton2 = false; // timer output on button press
    bool  ackButton2 = false; // flag for message on button press
    bool  toggleOnButton2 = false; // toggle related actuator output on button press
#endif

#ifdef SWITCH1
    bool send44 = false;
    bool curSwitch1State; // current switch state
#endif

#ifdef SWITCH2
    bool send45 = false;
    bool curSwitch2State; // current switch state
#endif

#ifdef ACTUATOR1 
  bool	send16 = false;
  boolean actuator1status = OFF;  // initialise it OFF
  bool  lastactuator1status = OFF;  // initialise it OFF
#endif
#ifdef ACTUATOR2 
  bool	send17 = false;
  boolean actuator2status = OFF;
  bool  lastactuator2status = OFF;  // initialise it OFF  
#endif
#ifdef ACTUATOR3 
  bool	send18 = false;
  boolean actuator3status = OFF; 
  bool  lastactuator3status = OFF;  // initialise it OFF
#endif
#ifdef ACTUATOR4 
  bool	send19 = false;
  boolean actuator4status = OFF;
  bool  lastactuator4status = OFF;  // initialise it OFF
#endif



#ifdef EXTENDEDBUTTON1 // 1st Extended Button on sub arduino ===============================================
  boolean extendedbutton1tapped = OFF;  // consider it as OFF until first read of the button
#endif

#ifdef EXTENDEDBUTTON2 // 2nd Extended Button on sub arduino ===============================================
  boolean extendedbutton2tapped = OFF;  // consider it as OFF until first read of the button
#endif

#ifdef EXTENDEDBUTTON3 // 3rd Extended Button on sub arduino ===============================================
  boolean extendedbutton3tapped = OFF;  // consider it as OFF until first read of the button
#endif

#ifdef EXTENDEDBUTTON4 // 4th Extended Button on sub arduino ===============================================
  boolean extendedbutton4tapped = OFF;  // consider it as OFF until first read of the button
#endif

#ifdef LEDSTRIP // dev2xx
  //int LEDStripsRemote = LEDSTRIPS_REMOTE;
  int LEDStrip1RedValue, LEDStrip1GreenValue, LEDStrip1BlueValue, LEDStrip1BrightnessValue = 0;    // initialise R,G,B & brightness values for this strip.
  #ifdef LEDSTRIP1_TYPE && !LEDSTRIPS_REMOTE  // is it a PIXEL type AND is LEDSTRIP local or remote?
    int LEDStrip1Type = LEDSTRIP1_TYPE;  // is it a PIXEL or DUMB LED strip.  
    CRGB pixelledstrip1_leds[LEDSTRIP1_NUMPIXELS]; // This is an array of leds.  One item for each led in your strip. Part of FastLED library.
  #endif  
  bool send200, send201, send202, send203, send204, send221, send231, send291, send299 = false;  // northbound message trigger
  int serSent200=-1, serSent201=-1, serSent202=-1, serSent203=-1, serSent204=-1, serSent221=-1, serSent231=-1, serSent291=-1, serSent299=-1; // copy of last value sent over serial for each DEVice. 
                                                                                  // default value, easy to spot and know it was set at init.
                                                                                  // will also cause all currentXXX to be sent once around init time.
  int current200, current201=0, current202=0, current203=0, current204=1, current221=1, current231=1, current291=1, current299 = 0; // What I need them to be when first sent to subordinate. 
                                                                                  
                                                                                 
#endif  // LEDSTRIP

#ifdef I2CLCD // ====================================
              // Code to initialise the I2C LCD if attached. "DFRobot 508040" part.
  #include <Wire.h>
  #include <LiquidCrystal_I2C.h>
  LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#endif

#ifdef XMASLIGHTS
  // insert any pre setup() code for this specific device here.
  int XmasLightsMode = 3; // the value we will send over the bus to the Xmas Lights Arduino.
  #define READY_OUT_PIN 2       // LOW = data on DATA_OUT pins is not ready.
  #define DATA_OUT_16_PIN 3     // LOW = 0, HIGH = 1
  #define DATA_OUT_8_PIN 4      // LOW = 0, HIGH = 1
  #define DATA_OUT_4_PIN 5      // LOW = 0, HIGH = 1
  #define DATA_OUT_2_PIN 6      // LOW = 0, HIGH = 1
  #define DATA_OUT_1_PIN 7      // LOW = 0, HIGH = 1
  bool send100 = false;  // northbound message trigger
#endif  // XMASLIGHTS

#ifdef MOTEINOWEATHER
  // insert any pre setup() code for this specific device here.
  
  SI7021 SI7021sensor;
  SFE_BMP180 BMP180pressure;
  float WeatherShieldData[3] = {11, 22, 33};  // these defaults just make it easy to see if value not set correctly later.
  bool send51, send54, send55 = false;  // northbound message triggers
#endif // MOTEINOWEATHER

#ifdef TSL2651
  // insert any pre setup() code for this specific device here.
  // Note: I relied heavily on the Adafruit 'sensorapi.ino' example that came with TSL2561 library.

  //                                                      \/ I2C Addr(see .h)  \/ Sensor ID (see .h)
  Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
  float TSL2651Data[2] = {11, 22};  // these defaults just make it easy to see if value not set correctly later.
  bool send52, send53 = false;  // northbound message triggers
#endif // TSL2651

#ifdef RMT_PWR
  int   RMT_PWR_State = 0; // status of remote ATX PSU, according to this node. Set it to off by default.
#endif  //RMT_PWR

#ifdef PING1x  // dev064 - distance sensor cm reading (RO - tells you last sensor sample)
  int ping1Distance=0;
  int ping1RangeMin=PING1MIN; // set to a starting default, but can be updated as dev065 later.
  int ping1RangeMax=PING1MAX; // set to a starting default, but can be updated as dev066 later.
  bool ping1Detect = false;   // set to a starting default, but can be updated as dev067 later.
  bool send64, send65, send66, send67 = false;  // northbound message triggers
  long ping1OnMillis=0; // used to control PING holdoff after a detection.
#endif

#ifdef RTC  // dev300-309 - Real Time Clock, that you can read or set.
  #include <Wire.h>
  #include "RTClib.h" // Adafruit RTClib
  RTC_DS1307 rtc;
  char rtcDaysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  bool rtcOk = false;  // is set during setup(), depending on if the RTC is present/connected properly.
  // prep variables for use later.  Set them by default to 1-1-1980, 1:01:01
  int rtcUnixTimeHigh=0;
  int rtcUnixTimeLow=0;
  uint16_t rtcYear=1980;
  uint8_t rtcMonth=1;
  uint8_t rtcDay=1;
  uint8_t rtcDayOfTheWeek=0;
  uint8_t rtcHour=1;
  uint8_t rtcMinute=1;
  uint8_t rtcSecond=1;
  DateTime now; // a structure used when we take a reading from the clock.
  bool send300, send301, send302, send303, send304, send305, send306, send307, send308 = false;  // northbound message triggers
#endif  // RTC

#ifdef LCDGENERIC // dev320 - 329 LCD (Generic one) (#define LCDGENERIC)
  bool send320, send321, send322, send323, send324 = false;  // northbound message triggers
  #ifdef LCDGENERIC_SFGLCD  // items specific to the SparkFun LCD-09351
    LCD lcdGen; //Create an instance of the Sparkfun GLCD class named LCD. We will use this instance to call all the 
                //subsequent LCD functions.
    int lcdGenXPos = 0; // X Pos to write to LCD, in chars not pixels. Top left is (0,0)
    int lcdGenYPos = 0; // Y Pos to write to LCD, in chars not pixels
    int lcdGenStrLen = 4; // to match the dummy text I put in it at startup below. 
    char lcdGenStr[LCDGENERIC_XMAX] = "Junk"; // dummy text to help with debugging.
  #endif // LCDGENERIC_SFGLCD
  #ifdef LCDGENERIC_BLAH  // items specific to the blah LCD
  #endif // LCDGENERIC_BLAH
#endif // LCDGENERIC

#ifdef LCDNEXTION_FPS // dev330 Nextion LCD used on FPS Node specifically
  bool send330 = false;  // northbound message triggers
  NexText text0 = NexText(0, 2, "text0");             // The big font title text on the Nextion LCD
  NexText scroll_inst = NexText(0, 3, "scroll_inst");           // The scrolling instruction to user text on the Nextion LCD
  NexText scroll0 = NexText(0, 1, "scroll0");   // The scrolling line of node data across bottom of Nextion LCD
  char NextionLCD_FPS_buffer[LCDNEXTION_FPS_BUFSIZE] = {0};  // create a char buffer for use with writting text to Nextion LCD.
#endif

#ifdef BEEPER // dev340 - 349 BEEPER
  bool send340= false;  // northbound message triggers. xxxx - these are WO registers, I don't need sendXXX flags for them!!!'
  int beeperCommand = 0; // used to select which of this nodes BEEPs to play, when instructed via MQTT command.
  int beeperNotes[4] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4};  // notes are defined in pitches.h
  int beeperDurations[4] = {250, 250, 250, 250}; // in mSec
#endif // BEEPER

#ifdef FINGER // dev350 - FINGER - FPS lookup occurred and the result is ...(RO) (integer).
  bool send350= false;  // northbound message triggers. 
  Adafruit_Fingerprint finger = Adafruit_Fingerprint(&FINGERSERIALPORT);
  int fingerLastDetectResult = 999; // stores the result of last scan when a finger was detected. 999 just helps debugging.
  int EnableFPSRunModeStateMachine = false; // flag that says FPS RUN MODE state machine should start and stay running until this is no longer true;
  int EnableFPSDBModeStateMachine = false; // flag that says FPS DB MODE state machine should start and stay running until this is no longer true;
#endif // FINGER

#ifdef EXTVAR40X  // dev400-409 - External Variables.
  bool send400, send401, send402, send403, send404, send405, send406, send407, send408, send409 = false;  // northbound message triggers
  float extVar400, extVar401, extVar402, extVar403, extVar404, extVar405, extVar406, extVar407, extVar408, extVar409 = 0.0;  // default value, easy to troubleshoot. 
#endif  // EXTVAR40X

#ifdef OCEANMIRROR  // Do this stuff if only if I have my Ocean Mirror attached via Serial to this Node
  float mirrorDev400, mirrorDev401, mirrorDev402, mirrorDev403, mirrorDev404, mirrorDev405, mirrorDev406, mirrorDev407, mirrorDev408, mirrorDev409 = 0.0;  // default value, easy to troubleshoot. 
#endif

// next after here setup() is run and then loop() is run.


