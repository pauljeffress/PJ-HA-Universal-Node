#ifndef PJ_MY_A_H
#define PJ_MY_A_H



#include <arduino.h>

#define RH_MESH_MAX_MESSAGE_LEN 80  // Max size of packet for RadioHead
#define HARFPACKSIZE 52 // Proper packet size for one of my home automation RF packets
#include <RHMesh.h>
#include <RH_RF69.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <Wire.h> // for I2C bus, used by many DEVices
#include "RTClib.h"


#include "pitches.h"  // my file with musical note definitions for BEEPER device.
#include <Adafruit_Fingerprint.h> // Designed to work with  http://www.adafruit.com/products/751 finger sensor
#include <SFE_BMP180.h>    //get it here: https://github.com/LowPowerLab/SFE_BMP180
#include <SI7021.h>        //get it here: https://github.com/LowPowerLab/SI7021
#include <Adafruit_Sensor.h>      // required for TSL2561 sensor stuff
#include <Adafruit_TSL2561_U.h>
//#include "Nextion.h"              //get it here: https://github.com/itead/ITEADLIB_Arduino_Nextion 
//  no longer including this lib from here, I have moved its include down to where I actually #define LCDNEXTION_FPS etc.



/* DEBUG CONFIGURATION PARAMETERS */
#define DEBUG // uncomment for debugging
//#define DEBUGPJ1 // uncomment for debugging
#define DEBUGPJ2 // uncomment for debugging
#ifdef DEBUG
  #define DEBUGPJ1 // if DEBUG thern ensure DEBUGPJ1 is enabled as it includes things DEBUG needs.
  #define DEBUGPJ2 // if DEBUG thern ensure DEBUGPJ2 is enabled as it includes things DEBUG needs.
#endif

#define SERIAL_BAUD 115200

#define STRPAYLOADSIZE 32   // How many chars in the String Payload we send? (must match in GW and Node!!!!!)

/* NODE TYPE - must select one ONLY!!!! */
//#define ETHNODETYPE // can only be one of these types, never both.
#define RFNODETYPE   // can only be one of these types, never both.

#define FEATHERM0       // uncomment if we are using an Adafruit Feather M0
#define FEATHERM0RFM69  // also uncomment if we are using an Adafruit FeatherM0-RFM69 or similar
                        // Search code to see where this is used. 1st thing I found I needed to use it
                        // for was to execute the RFM hard reset during setup(), as the Feather has a pin
                        // wired to the RFM Reset pin to do it :)
    #define IS_RFM69HCW true // set to 'true' if you are using an RFM69HCW module
    // for Feather M0 Radio
    #define RFM69_CS 8
    #define RFM69_IRQ 3
    #define RFM69_IRQN 3 // Pin 3 is IRQ 3!
    #define RFM69_RST 4

    /* Feather m0 w/wing
    #define RFM69_RST 11 // "A"
    #define RFM69_CS 10 // "B"
    #define RFM69_IRQ 6 // "D"
    #define RFM69_IRQN digitalPinToInterrupt(RFM69_IRQ )
    */

#define VERSION "UNIvGitHub"  // this value can be queried as device 3

/* NODE CORE CONFIGURATION PARAMETERS 
****************************************************/

#define CLIENT_ADDRESS    7       // RadioHead Mesh Addressing
#define debug_mode 1              // Set debug_mode to 1 for Serial Monitor (RH lib?)
#define NODEID           07       // unique node ID within the closed network
#define NODEIDSTRING node07       // as per above.  
#define COMMS_LED_PIN A4          // RED - Comms traffic IP or RF for/from this node, activity indicator.
                                   // DO NOT USE D10-D13 on a Moteino (non mega) as they are in use for RFM69 SPI!
                                   // The onboard RED LED on Feathers is D13.
#define COMMS_LED_ON_PERIOD 1000 // How long we keep it on for, in mSec.
#define STATUS_LED_PIN A5              // BLUE - Status LED, generally just blinking away so we know node has not crashed.
#define STATUS_LED_CYCLE_PERIOD 5000   // (mSecs) Under normal circumstances how often should we flash the STATUS LED?
#define STATUS_LED_ON_PERIOD 100       // (mSecs) How long we keep it on for per blink, in mSec.

/****************************************************/


/* ETH NODE TYPE CONFIGURATION PARAMETERS & LIBRARIES 
*****************************************************/
//#define SUBTOPICSTR "home/eth_nd/sb/node23/#"   // MQTT topic, only used in ETH Node type
//#define CLIENTNAMESTR "PJ_HA_Eth_Node_23_HouseFPS"  // MQTT topic, only used in ETH Node type
//#define MQCON 24          // GREEN - MQTT Connection indicator, only used in ETH Node type
/***************************************************/


/* RF NODE TYPE CONFIGURATION PARAMETERS & LIBRARIES 
****************************************************/
#define GATEWAYID 1	    // node ID of the RF Gateway is always 1 
#define NETWORKID 111	// network ID of the RF network
#define CANRFTX_DELAY 200 // mSec - gates how often rfSendMsg() is called.
// Note: Many additional RF Parameters are can be found within setup().
/***************************************************/


// define types of Serial messages, this is the protocol used for slaveserial type devices.
#define HELLO 1
#define KEYPRESSED 2
#define TOGGLEEXTENDEDACTUATOR 3
#define EXTENDEDACTUATORON 4
#define EXTENDEDACTUATOROFF 5
#define SERIALWATCHDOGTIMEOUT 30  // How long (in secs) before we consider serial comms lost

// Error Dev Types (Dev9x etc)
#define DEV92ERRORSTR "Error message - You tried to READ from a WO device"
#define DEV93ERRORSTR "Error message - You tried to WRITE to a RO device"
#define DEV94ERRORSTR "Error message - You tried to READ a WRITEONLY device"
#define DEV95ERRORSTR "Error message - Value to be WRITTEN is out of range"

// misc defines 
#define HOLDOFF 2000      // blocking period between button and PIR messages xxxx


// PJ Specific/additional DEFINES
// ==============================
#define OFF 0                     // xxxx
#define ON  1
// ==============================




//-------------------------------------------------------------------------
// NODE devices selection (actuators and sensors etc)
//-------------------------------------------------------------------------
// What kind of devices are enabled on this node?: (add PIN config in applicable segments below)
//#define DHT
//#define DS18
//#define SLEEPY //node on batteries? can be used with either DS18 or PIR (not both due watchdog interference)

// #define PIR1          // Have I attached a PIR
//      #define PIR1PIN 4   // IF MEGA DO NOT HANG A LED OFF THIS PIN too. Maga won't detect a transition if you do!

//                           // signal pin from 1st PIR if attached, else ignored.
//      #define PIRdelay delay(2000) // give the grid time to stabilize for the PIR, otherwise false triggers will occur after a send due to power dip (up to 2s?)
//      #define PIRHOLDOFF 2       // blocking period between button and PIR messages (seconds) xxxx

//#define PIR2          // Have I attached a 2nd PIR
//   #define PIR2PIN 23         // signal pin from 2nd PIR if attached, else ignored.

//#define BUTTON1       // Have I attached a button (momentary switches)
//  #define BUTTON1PIN 999      // signal pin from 1st BUTTON
//#define BUTTON2
//    #define BUTTON2PIN 999      // signal pin from 2nd BUTTON

//#define SWITCH1       // Have I attached a switch (ON/OFF capable)
    //#define SWITCH1PIN A5      // signal pin from 1st SWITCH
//#define SWITCH22
//    #define SWITCH2PIN 999      // signal pin from 2nd SWITCH

#define ACTUATOR1     // Have I attached any actuators (i.e. digital out pins connected to devices)... 
      #define ACTUATOR1PIN A1    // contol pin for 1st ACTUATOR if attached, else ignored.
      #define ACTUATOR1REVERSE  // define this if you want the output pin of this Actuator to be LOW when ON, rather than HIGH when ON.
 #define ACTUATOR2
      #define ACTUATOR2PIN A2   // contol pin for 2nd ACTUATOR if attached, else ignored.
      #define ACTUATOR2REVERSE  // define this if you want the output pin of this Actuator to be LOW when ON, rather than HIGH when ON.
// #define ACTUATOR3
//     #define ACTUATOR3PIN 6    // contol pin for 3rd ACTUATOR if attached, else ignored.
//     #define ACTUATOR3REVERSE  // define this if you want the output pin of this Actuator to be LOW when ON, rather than HIGH when ON.
// #define ACTUATOR4
//     #define ACTUATOR4PIN 7    // contol pin for 4th ACTUATOR if attached, else ignored.
//     #define ACTUATOR4REVERSE  // define this if you want the output pin of this Actuator to be LOW when ON, rather than HIGH when ON.

//#define SERIALSLAVE   // Has this node got a subordinate sub node under it via hw Serial1 port?

// #define EXTENDEDBUTTON1   // Have I got any buttons somehow slaved off this node?  e.g. across serial on a sub node
// #define EXTENDEDBUTTON2
// #define EXTENDEDBUTTON3
// #define EXTENDEDBUTTON4
// #define EXTENDEDACT1    // Have I got any Actuators (i.e digital out pins) somehow slaved off this node?  e.g. across serial on a sub node
// #define EXTENDEDACT2
// #define EXTENDEDACT3
// #define EXTENDEDACT4

//#define XMASLIGHTS
//#define I2CLCD             // Have I attached one of my i2c LCDs to this node?
// #define TSL2651           // PJ - is there an Adafruit TSL2561 breakout board (light sensors) present?
// #define MOTEINOWEATHER    // PJ - is there a Moteino WeatherShield present? (http://lowpowerlab.com/blog/2015/01/30/weathershield-is-here/)
//   #define MOTEINO_WEATHERSHIELD_ALTITUDE 20 // Altitude in meters (my estimate for our house)
//   #define MOTEINO_WEATHERSHIELD_V_ENABLE_PIN A3 // The pin the Moteino uses to temporarily enable the voltage divider cct for analog read of the Vin/Batt level as per cct on WeatherShield.
//   #define MOTEINO_WEATHERSHIELD_V_VALUE_PIN A7 // The pin the Moteino can analog read the Vin/Batt level as per cct on WeatherShield.

//#define RMT_PWR           // PJ - are we using my remote triggered ATX PSU to power main part of this node?
//     #define RMT_PWR_ENA_PIN 5  // The pin to set high when I want to switch on a remote ATX PC power supply
                                // that is providing the power for the actuator/LED etc, beyond just the 
                                // power for the Moteino/Arduino itself.
                                // Chose this pin for now, but may conflict with actuators of other types. 

//#define PING1x         // Have I got a PING/HC-SR04 distance sensor attached to this node?
//    #define PING1TRIGGERPIN A12
//    #define PING1ECHOPIN A11
//    #define PING1MIN 10     // range min, in cm, to consider a detection.
//    #define PING1MAX 20    // range max, in cm, to consider a detection.
//    #define PING1HOLDTIME 5   // seconds - to wait before attempting another PING after a detection.
//
//#define RTC     // is there some sort of RTC attached to this node.
//
//#define LCDGENERIC  // is there one of my generic conforming LCDs attached to this node.
//  #define LCDGENERIC_SFGLCD   // am I using a SparkFun graphical LCD-09351 as my generic LCD on this node. (PICK only ONE!)
//    #define LCDGENERIC_BLAH     // am I using a blah LCD as my generic LCD on this node? (PICK only ONE!)
//  #define LCDGENERIC_BRIGHTNESS 50  // Brightness in range 0 - 100, if LCD has a backlight. 
//  #ifdef LCDGENERIC_SFGLCD  // items specific to the SparkFun LCD-09351
//    // Important Note: The SparkFunSerialGraphicLCD.h library ASSUMES we need SoftSerial, because it was developed
//    //                 for a Uno only!  It also assumes you have selected D3 as the TX to the LCD's RX.  You don't
//    //                 even really need to connect D2 as the RX to the LCD's TX as it doesn't send anything back.
//    #define LCDGENERIC_XMAX 20 // Number of char columns on this particular LCD 
//    #define LCDGENERIC_YMAX 8  // Number of char rows on this particular LCD
//  #endif // LCDGENERIC_SFGLCD
//  #ifdef LCDGENERIC_BLAH  // items specific to the blah LCD
//    #define LCDGENERIC_XMAX 16 // Number of columns on this particular LCD 
//    #define LCDGENERIC_YMAX 2  // Number of rows on this particular LCD 
//  #endif // LCDGENERIC_BLAH
//

//#define LCDNEXTION_FPS  // have I got a Nextion LCD setup to work on an FPS node, this node?
//    #include "Nextion.h"              //get it here: https://github.com/itead/ITEADLIB_Arduino_Nextion 
//    #define LCDNEXTION_FPS_BUFSIZE 100  // Size of the character buffer, make sure its big enough for any strings you send to the Nextion. 

//#define BEEPER  // is there one of my beepers attached to this node?
//  #define BEEPERPIN A0  // PWM pin that the speaker is attached to.
//  #define BEEPNUMMAX 4 // how many beeps are defined on this node.

//#define FINGER  // is there one of my Finger Print Scanners attached to this node?
// #define FINGERSERIALPORT Serial1  // Which port is it attached to. Serial, Serial1..3 (Megga only) or SoftSerial etc
                                   // Mega TX1 pin 18 <-> FPS Red wire
                                   // Mega RX1 pin 19 <-> FPS Dark Blue wire
                                   //             +5V <-> FPS Green wire
                                   //             GND <-> FPS White wire
// #define FPS_MODE_RUN		0	// operating as normal
// #define FPS_MODE_DB			1	// allowing fingerprint database changes
// #define FPS_FOOTLIGHT_LED_ACTUATOR_STATUS actuator3status // assign this FPS function to one of the nodes ACTUATORs.  
// #define FPS_DOOR_LATCH_ACTUATOR_STATUS actuator2status // assign this FPS function to one of the nodes ACTUATORs.


//  #define EXTVAR40X  // is this Node interested in External Variables 00 thru 09?  i.e. Dev 400-409
//  #define EXTVAR41X  // is this Node interested in External Variables 10 thru 19?  i.e. Dev 410-419
//  #define EXTVAR42X  // is this Node interested in External Variables 20 thru 29?  i.e. Dev 420-429

//  #define OCEANMIRROR // Do I have my Ocean Mirror attached via Serial to this Node

// #define LEDSTRIP
//    #define STATIC_ONE_COLOUR 0 // see description of DEV2xx's as there are three overall modes.
//    #define STATIC_PATTERN    1 
//    #define DYNAMIC_PATTERN   2
//    #define STATIC_PATTERN_MAX  0   // how many different sub modes of STATIC_PATTERN are configured in my code
//    #define DYNAMIC_PATTERN_MAX  0  // how many different sub modes of DYNAMIC_PATTERN are configured in my code
  
//   #define LEDSTRIPS_REMOTE         // see DEV299 
                                     // If its defined then it means LEDs are on subordinate MCU.
                                     // If its not defined then it means LEDs are on local MCU.
                                     // Need to set this correctly right up front as a number of subsequent LEDSTRIP DEVice
                                     // activities need to know, before this could be set by MQTT from somewhere else. Also need
                                     // to know it and have it correct in case Node restarts and has no comms. We need the LED initialisation
                                     // to still proceed properly.
  // #define LEDSTRIP1_TYPE       1     // 0 = DUMBLEDSTRIP type, 1 = PIXELLEDSTRIP type. No other value is valid.
  //                                    //       not used if LEDSTRIPS_REMOTE defined
  // #define LEDSTRIP1_NUMPIXELS  75   // num WS28xx controller chips on this strip. Not used for DUMBLEDSTRIPs.
  //                                    //       not used if LEDSTRIPS_REMOTE defined
  // #define LEDSTRIP1_DATAPIN    6     // PIN used for data feed to this strip. Not used for DUMBLEDSTRIPs.
                                     // DO NOT USE D10-D13 on a Moteino (non mega) as they are in use for RFM69 SPI!
                                     //       not used if LEDSTRIPS_REMOTE defined
  //#define LEDSTRIP1_DUMB_R_PIN  xx     // What pin are all the DUMBLEDSTRIPs Red LEDs driven from? Not used for PIXELLEDSTRIPs.
                                         //       not used if LEDSTRIPS_REMOTE defined
  //#define LEDSTRIP1_DUMB_G_PIN  xx     // What pin are all the DUMBLEDSTRIPs Green LEDs driven from? Not used for PIXELLEDSTRIPs.
                                         //       not used if LEDSTRIPS_REMOTE defined
  //#define LEDSTRIP1_DUMB_B_PIN  xx     // What pin are all the DUMBLEDSTRIPs Blue LEDs driven from? Not used for PIXELLEDSTRIPs.
                                         //       not used if LEDSTRIPS_REMOTE defined

//-------------------------------------------------------------------------


// ==============================================
// 'struct's that I had to place here not in main ino file, to assist compilation.
//
typedef struct {			// Radio packet structure max 66 bytes (only transmitted between RF GW <> Nodes, not over IP)
  long  nodeID;			// FROM node. Using a long as its 32bits on AVR and ARM. 
  long	devID;			// device identifier 0 is node; 31 is temperature, 32 is humidity
  long	cmd;			  // read or write
  long	intVal;			// integer payload
  long	fltintVal;	// floating point payload, but multiplied by 100 and transported as a long.  Works on ARM and AVR the same.
  char	payLoad[STRPAYLOADSIZE];	// char array, String payload
  } Message;



// ============================================
// Function prototypes when required;
void sendInteger(int sendNodeID, int sendDevID, int sendInt);
void sendStatusInteger(int sendNodeID, int sendDevID, int sendInt);
void sendString(int sendNodeID, int sendDevID, char* sendStr);
void sendReal(int sendNodeID, int sendDevID, float sendR);
void housekeeping();
void mqttinbound();
void mqtt_subs();
void mqtt_subs_senderror();
void mqtt_subs_switch();
void localactions();
void setPeriodicTransmissions();
void dosends();
void rfSendMsg();
void parseCmd();
void txRadio();
void sendserialtoslave();
void checkonserialfromslave();
bool receiveData();
void checkbuttons();
bool checkPINGs();
void writeLCDGEN();
void localwriteLCDGEN(int xpos, int ypos, char* theStr);
void ToneBeep(int note, int duration);
int getFingerprintID();
int getFingerprintIDez();
int FPSgetFingerprintID();
void FPSRunModeSM();
void FPSRunStart();
void FPSRunStartIdentify();
void FPSRunDoIdentify();
void FPSRunDoorUnlocker();
void FPSRunDoorUnlocked();
void FPSRunDoorLocker();
void FPSRunExit();
void FPSTurnFootlightOn();
void FPSTurnFootlightOff();
void FPSUnlockDoor();
void FPSLockDoor();
void MIRROR_sendSerialToDue(int sendSerDev, float sendparam);
void RMT_PWROn();
void RMT_PWROff();
String flashStringBuilder (const __FlashStringHelper *data);
void sendserialtoslave(uint8_t sendsertype, uint8_t sendparam);
void checkPIRs();
void getWeatherShield();
void getTSL2651();
void displaySensorDetails(void);
void configureSensor(void);
void writeLCDNEXTION_FPS_instruction(char* theStr);  
void sendDevValueToSerial(int sendSerDev, int sendSerType, int sendSerInt, float sendSerFloat);
void setStaticOneColourLEDStrip(int stripnum);
void setStaticPatternLEDStripMode(int stripnum, int stripmode);
void setDynamicPatternLEDStripMode(int stripnum, int stripmode);
void checkswitches();
long fltTofltint(float floatIn);

// =============================================
// Global variables as 'externs' so individual files can compile if they use them.
extern long    TXinterval;
extern long    TIMinterval;
extern bool    toggleOnButton;
//bool	ackButton = false; // flag for message on button press
extern long	lastPeriod;
//long    lastBtnPress = -1; // timestamp last buttonpress
//long    lastPIR = -1; // timestamp last PIR detection
extern long	lastMinute;
extern long	upTime;
extern float hum, temp;
extern int signalStrength;
extern bool promiscuousMode;
extern bool setAck;
extern bool	send0, send1, send2, send3, send4, send5, send6, send7, send48, send49;
extern bool	send16, send17, send18, send19, send40, send42, send50;
extern bool send92, send93, send94, send95; 
extern bool send11, send12; // compilation info
extern bool actuator1status, actuator2status, actuator3status, actuator4status; 
extern bool lastactuator1status, lastactuator2status, lastactuator3status, lastactuator4status; 
extern bool extendedbutton1tapped, extendedbutton2tapped, extendedbutton3tapped, extendedbutton4tapped;
extern byte mac[];
extern byte mqtt_server[];  
extern byte ip[];      
extern String strPayload; 
    //void mqtt_subs(char* topic, byte* payload, unsigned int length);  // I had to put this prototype here or else I could not compile
extern EthernetClient ethClient;
extern PubSubClient mqttClient;
extern int   destNodeID;      
extern int   destDevID;    
extern int   error;          
extern bool  CommsLEDStart;
extern bool  CommsLEDStatus;
extern long  CommsLEDonMillis;
extern bool  StatusLEDStatus;
extern long  StatusLEDonMillis;
extern bool  mqttCon;  
//    bool  respNeeded = false;       // MQTT message flag in case of radio connection failure
extern bool  mqttToSend;
extern bool  verbose;         
extern bool  IntMess, RealMess, StatMess, StrMess; 
extern long  onMillis;    
extern char  *subTopic;
extern char  *clientName;
extern char  buff_topic[30];      
extern char  buff_mess[32]; 
extern bool  msgBlock;      
extern uint8_t radioDataBuf[RH_MESH_MAX_MESSAGE_LEN];
extern Message mes;
extern bool wakeUp;
extern long lastRfTxTime;
extern bool rfTxAllowed;
//extern RFM69 radio;
extern RH_RF69 driver;  
extern RHMesh manager;

#ifdef BUTTON1
extern bool send40;
extern boolean button1status;  
extern long  lastBtn1Press; 
extern bool  curBtn1State; 
extern bool  lastBtn1State; 
extern bool  timerOnButton1; 
extern bool  ackButton1; 
extern bool  toggleOnButton1; 
#endif

#ifdef BUTTON2
extern bool send42;
extern boolean button2status;  
extern long  lastBtn2Press; 
extern bool  curBtn2State; 
extern bool  lastBtn2State; 
extern bool  timerOnButton2; 
extern bool  ackButton2; 
extern bool  toggleOnButton2;
#endif

#ifdef PIR1
  extern bool send41;
  extern int  calibrationTime;
  extern int  Pir1Input;
  extern long lastPIR1;
  extern bool curPIR1State;
#endif

#ifdef PIR2
  extern bool send43;
  extern int  Pir2Input;
  extern long lastPIR2;
  extern bool curPIR2State;
#endif

#ifdef SWITCH1
  extern bool send44;
  extern bool  curSwitch1State; 
#endif

#ifdef SWITCH2
  extern bool send45;
  extern bool  curSwitch2State; 
#endif

#ifdef PING1x
extern int ping1Distance;
extern int ping1RangeMin;
extern int ping1RangeMax;
extern bool ping1Detect;
extern bool send64, send65, send66, send67;
extern long ping1OnMillis;
#endif

#ifdef LEDSTRIP
  // required libraries required for this DEVice.
  #include <FastLED.h>
  #include <colorutils.h>
  // extern any global variables used by this DEVice.
  //extern int LEDStripsRemote;
  extern int LEDStrip1Type;
  extern int LEDStrip1RedValue, LEDStrip1GreenValue, LEDStrip1BlueValue, LEDStrip1BrightnessValue;
  extern bool send200, send201, send202, send203, send204, send221, send231, send291, send299;
  extern int serSent200, serSent201, serSent202, serSent203, serSent204, serSent221, serSent231, serSent291, serSent299; 
  extern int current200, current201, current202, current203, current204, current221, current231, current291, current299;
  #ifdef LEDSTRIP1_TYPE && !LEDSTRIPS_REMOTE  // is it a PIXEL type AND is LEDSTRIP local or remote?
    extern  CRGB pixelledstrip1_leds[LEDSTRIP1_NUMPIXELS];
  #endif
  // prototype any unique functions created for this DEVice.
  void setupLEDStrips();
  void setStaticOneColourLEDStrip(int stripnum);
#endif

#ifdef RTC
extern RTC_DS1307 rtc;
extern char rtcDaysOfTheWeek[7][12];
extern bool rtcOk;
extern int rtcUnixTimeHigh;
extern int rtcUnixTimeLow;
extern uint16_t rtcYear;
extern uint8_t rtcMonth;
extern uint8_t rtcDay;
extern uint8_t rtcDayOfTheWeek;
extern uint8_t rtcHour;
extern uint8_t rtcMinute;
extern uint8_t rtcSecond;
extern DateTime now;
extern bool send300, send301, send302, send303, send304, send305, send306, send307, send308;
#endif

#ifdef LCDGENERIC // dev320 - 329 LCD (Generic one) (#define LCDGENERIC)
extern bool send320, send321, send322, send323, send324;
  #ifdef LCDGENERIC_SFGLCD  // items specific to the SparkFun LCD-09351
    #include <SparkFunSerialGraphicLCD.h>//inculde the Serial Graphic LCD library
    #include <SoftwareSerial.h> // used by SparkFunSerialGraphicLCD library
    extern LCD lcdGen; 
    extern int lcdGenXPos;
    extern int lcdGenYPos;
    extern int lcdGenStrLen;
    extern char lcdGenStr[LCDGENERIC_XMAX];
  #endif // LCDGENERIC_SFGLCD
  #ifdef LCDGENERIC_BLAH  // items specific to the blah LCD
  #endif // LCDGENERIC_BLAH
#endif // LCDGENERIC

#ifdef LCDNEXTION_FPS  // dev330 Nextion LCD used on FPS Node specifically
  extern bool send330;
  extern NexText text0; 
  extern NexText scroll_inst;
  extern NexText scroll0;
  extern char NextionLCD_FPS_buffer[LCDNEXTION_FPS_BUFSIZE];
#endif

#ifdef BEEPER
  extern bool send340;
  extern int beeperCommand;
  extern int beeperNotes[4];
  extern int beeperDurations[4];
#endif // BEEPER

#ifdef FINGER
  extern bool send350; 
  extern Adafruit_Fingerprint finger;
  extern int fingerLastDetectResult;
  extern int EnableFPSRunModeStateMachine;
  extern int EnableFPSDBModeStateMachine;
  extern int FPSRunState;
  extern int FPSIdentifyStatus;
  extern unsigned long FPSRunIdentifyTimer;
  extern unsigned long FPSRunUnlockedTimer;
#endif // FINGER

#ifdef EXTVAR40X
  extern bool send400, send401, send402, send403, send404, send405, send406, send407, send408, send409;
  extern float extVar400, extVar401, extVar402, extVar403, extVar404, extVar405, extVar406, extVar407, extVar408, extVar409; 
#endif  // EXTVAR40X

#ifdef OCEANMIRROR  // Do this stuff if only if I have my Ocean Mirror attached via Serial to this Node
  extern float mirrorDev400, mirrorDev401, mirrorDev402, mirrorDev403, mirrorDev404, mirrorDev405, mirrorDev406, mirrorDev407, mirrorDev408, mirrorDev409; 
#endif

#ifdef RMT_PWR
  extern int   RMT_PWR_State;
#endif

#ifdef MOTEINOWEATHER
  extern float WeatherShieldData[3];
  extern bool send51, send54, send55;
  extern SI7021 SI7021sensor;
  extern SFE_BMP180 BMP180pressure;
#endif

#ifdef TSL2651
  extern Adafruit_TSL2561_Unified tsl;
  extern float TSL2651Data[2];
  extern bool send52, send53;
#endif



#endif // PJ_MY_A_H
