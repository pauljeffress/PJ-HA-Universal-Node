#ifndef PJ_MY_A_H
#define PJ_MY_A_H



#include <arduino.h>

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <RFM69.h>
#include <Wire.h> // for I2C bus, used by many DEVices
#include "RTClib.h"
#include <SparkFunSerialGraphicLCD.h>//inculde the Serial Graphic LCD library
#include <SoftwareSerial.h> // used by SparkFunSerialGraphicLCD library
#include "pitches.h"  // my file with musical note definitions for BEEPER device.
#include <Adafruit_Fingerprint.h> // Designed to work with  http://www.adafruit.com/products/751 finger sensor
#include <SFE_BMP180.h>    //get it here: https://github.com/LowPowerLab/SFE_BMP180
#include <SI7021.h>        //get it here: https://github.com/LowPowerLab/SI7021
#include <Adafruit_Sensor.h>      // required for TSL2561 sensor stuff
#include <Adafruit_TSL2561_U.h>

/* DEBUG CONFIGURATION PARAMETERS */
//#define DEBUG // uncomment for debugging
//#define DEBUGPJ1 // uncomment for debugging
#define DEBUGPJ2 // uncomment for debugging
#ifdef DEBUG
  #define DEBUGPJ1 // if DEBUG thern ensure DEBUGPJ1 is enabled as it includes things DEBUG needs.
  #define DEBUGPJ2 // if DEBUG thern ensure DEBUGPJ2 is enabled as it includes things DEBUG needs.
#endif

#define SERIAL_BAUD 115200

/* NODE TYPE - must select one ONLY!!!! */
//#define ETHNODETYPE // can only be one of these types, never both.
#define RFNODETYPE   // can only be one of these types, never both.



#define VERSION "UNIvGitHub"  // this value can be queried as device 3

/* NODE CORE CONFIGURATION PARAMETERS 
****************************************************/
#define NODEID           05       // unique node ID within the closed network
#define NODEIDSTRING node05       // as per above.  
#define COMMS_LED_PIN 6          // RED - Comms traffic IP or RF for/from this node, activity indicator.
#define COMMS_LED_ON_PERIOD 1000 // How long we keep it on for, in mSec.
#define STATUS_LED_PIN 9         // BLUE - Status LED, generally just blinking away so we know node has not crashed.
/****************************************************/


/* ETH NODE TYPE CONFIGURATION PARAMETERS & LIBRARIES 
*****************************************************/
#define SUBTOPICSTR "home/eth_nd/sb/node21/#"   // MQTT topic, only used in ETH Node type
#define CLIENTNAMESTR "PJ_HA_Eth_Node_21_ShedInt"  // MQTT topic, only used in ETH Node type
#define MQCON 7          // GREEN - MQTT Connection indicator, only used in ETH Node type
/***************************************************/


/* RF NODE TYPE CONFIGURATION PARAMETERS & LIBRARIES 
****************************************************/
#define GATEWAYID 1	    // node ID of the RF Gateway is always 1 
#define NETWORKID 100	// network ID of the RF network
#define ENCRYPTKEY "xxxxxxxxxxxxxxxx" // 16-char encryption key; same as on RF Gateway!
    // Wireless settings Match frequency to the hardware version of the radio
    //#define FREQUENCY RF69_433MHZ
    //#define FREQUENCY RF69_868MHZ
#define FREQUENCY RF69_915MHZ
    //#define IS_RFM69HW // uncomment only for RFM69HW!
#define ACK_TIME 50 // max # of ms to wait for an ack
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
//     #define PIR1PIN 4   // IF MEGA DO NOT HANG A LED OFF THIS PIN too. Maga won't detect a transition if you do!
//                          // signal pin from 1st PIR if attached, else ignored.
//     #define PIRdelay delay(2000) // give the grid time to stabilize for the PIR, otherwise false triggers will occur after a send due to power dip (up to 2s?)
//     #define PIRHOLDOFF 2       // blocking period between button and PIR messages (seconds) xxxx

//#define PIR2          // Have I attached a 2nd PIR
//   #define PIR2PIN 23         // signal pin from 2nd PIR if attached, else ignored.

//#define BUTTON1       // Have I attached some buttons/switches...
//   #define BUTTON1PIN 30      // signal pin from 1st BUTTON
//#define BUTTON2
//    #define BUTTON2PIN 999      // signal pin from 2nd BUTTON

#define ACTUATOR1     // Have I attached any actuators (i.e. digital out pins connected to devices)... 
     #define ACTUATOR1PIN 5    // contol pin for 1st ACTUATOR if attached, else ignored.
     #define ACTUATOR1REVERSE  // define this if you want the output pin of this Actuator to be LOW when ON, rather than HIGH when ON.
#define ACTUATOR2
     #define ACTUATOR2PIN 3   // contol pin for 2nd ACTUATOR if attached, else ignored.
     #define ACTUATOR2REVERSE  // define this if you want the output pin of this Actuator to be LOW when ON, rather than HIGH when ON.
// #define ACTUATOR3
//     #define ACTUATOR3PIN 33    // contol pin for 3rd ACTUATOR if attached, else ignored.
//     #define ACTUATOR3REVERSE  // define this if you want the output pin of this Actuator to be LOW when ON, rather than HIGH when ON.
// #define ACTUATOR4
//     #define ACTUATOR4PIN 32    // contol pin for 4th ACTUATOR if attached, else ignored.
//     #define ACTUATOR4REVERSE  // define this if you want the output pin of this Actuator to be LOW when ON, rather than HIGH when ON.

// #define SERIALSLAVE   // Has this node got a subordinate sub node under it via hw Serial1 port?

// #define EXTENDEDBUTTON1   // Have I got any buttons somehow slaved off this node?  e.g. across serial on a sub node
// #define EXTENDEDBUTTON2
// #define EXTENDEDBUTTON3
// #define EXTENDEDBUTTON4
// #define EXTENDEDACT1    // Have I got any Actuators (i.e digital out pins) somehow slaved off this node?  e.g. across serial on a sub node
// #define EXTENDEDACT2
// #define EXTENDEDACT3
// #define EXTENDEDACT4
//#define PIXELLEDSTRIP     // PJ - my new device types
//#define DUMBLEDSTRIP
//#define XMASLIGHTS
//#define I2CLCD             // Have I attached one of my i2c LCDs to this node?
// #define TSL2651           // PJ - is there an Adafruit TSL2561 breakout board (light sensors) present?
// #define MOTEINOWEATHER    // PJ - is there a Moteino WeatherShield present? (http://lowpowerlab.com/blog/2015/01/30/weathershield-is-here/)
//   #define MOTEINO_WEATHERSHIELD_ALTITUDE 20 // Altitude in meters (my estimate for our house)
//   #define MOTEINO_WEATHERSHIELD_V_ENABLE_PIN A3 // The pin the Moteino uses to temporarily enable the voltage divider cct for analog read of the Vin/Batt level as per cct on WeatherShield.
//   #define MOTEINO_WEATHERSHIELD_V_VALUE_PIN A7 // The pin the Moteino can analog read the Vin/Batt level as per cct on WeatherShield.

//#define RMT_PWR           // PJ - are we using my remote triggered ATX PSU to power main part of this node?
    //#define RMT_PWR_ENA_PIN 3 // The pin to set high when I want to switch on a remote ATX PC power supply
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
//#define BEEPER  // is there one of my beepers attached to this node?
//  #define BEEPERPIN A0  // PWM pin that the speaker is attached to.
//  #define BEEPNUMMAX 4 // how many beeps are defined on this node.
//
//#define FINGER  // is there one of my Finger Print Scanners attached to this node?
//  #define FINGERSERIALPORT Serial1  // Which port is it attached to. Serial, Serial1..3 (Megga only) or SoftSerial etc
//                                    // Mega TX1 pin 18 <-> FPS Red wire
//                                    // Mega RX1 pin 19 <-> FPS Dark Blue wire
//                                    //             +5V <-> FPS Green wire
//                                    //             GND <-> FPS White wire
//  #define FPS_MODE_RUN		0	// operating as normal
//  #define FPS_MODE_DB			1	// allowing fingerprint database changes
//  #define FPS_FOOTLIGHT_LED_ACTUATOR_STATUS actuator3status // assign this FPS function to one of the nodes ACTUATORs.  
//  #define FPS_DOOR_LATCH_ACTUATOR_STATUS actuator2status // assign this FPS function to one of the nodes ACTUATORs.


//  #define EXTVAR40X  // is this Node interested in External Variables 00 thru 09?  i.e. Dev 400-409
//  #define EXTVAR41X  // is this Node interested in External Variables 10 thru 19?  i.e. Dev 410-419
//  #define EXTVAR42X  // is this Node interested in External Variables 20 thru 29?  i.e. Dev 420-429

//  #define OCEANMIRROR // Do I have my Ocean Mirror attached via Serial to this Node

//-------------------------------------------------------------------------


// ==============================================
// 'struct's that I had to place here not in main ino file, to assist compilation.
//
typedef struct {        // Radio packet structure max 66 bytes
  int     nodeID;       // node identifier
  int     devID;        // device identifier 0 is node; 31 is temperature, 32 is humidity
  int     cmd;          // read or write
  long    intVal;       // integer payload
  float   fltVal;       // floating payload
  char    payLoad[32];  // char array payload
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
void sendMsg();
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
extern bool send92, send93;
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

extern Message mes;
extern bool wakeUp;
extern RFM69 radio;

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

#ifdef PING1
extern int ping1Distance;
extern int ping1RangeMin;
extern int ping1RangeMax;
extern bool ping1Detect;
extern bool send64, send65, send66, send67;
extern long ping1OnMillis;
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
    extern LCD lcdGen; 
    extern int lcdGenXPos;
    extern int lcdGenYPos;
    extern int lcdGenStrLen;
    extern char lcdGenStr[LCDGENERIC_XMAX];
  #endif // LCDGENERIC_SFGLCD
  #ifdef LCDGENERIC_BLAH  // items specific to the blah LCD
  #endif // LCDGENERIC_BLAH
#endif // LCDGENERIC

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
