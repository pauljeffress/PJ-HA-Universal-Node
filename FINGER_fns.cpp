//
// Misc FINGER related functions
//
// Much of this originated with the finger.ino sample code that came with the Adafruit library.

#include "a.h" // My global defines and extern variables to help multi file comilation.


#ifdef FINGER

crap

// ================ FPSgetFingerprintID()
// Detailed reader function, based on the Adafruit Fingerprint Sensor 
// Library (https://github.com/adafruit/Adafruit-Fingerprint-Sensor-Library
// example function called getFingerprintID().
// Modified so that it returns a few error codes or an identified finger ID num.
// Return Codes
// ------------
// 0 - 160  Successfully matched finger ID
// 990      No finger detected on sensor
// 991      Finger detected but no match found
// 992      Other error

#define FPS_NO_FINGER_DETECT 990 //see return codes above
#define FPS_FINGER_NO_MATCH 991 //see return codes above
#define FPS_OTHER_ERROR 992 //see return codes above

int FPSgetFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      #ifdef DEBUGPJ2
        Serial.println("FPS - Image taken");
      #endif
      break;
    case FINGERPRINT_NOFINGER:
      #ifdef DEBUGPJ2
        Serial.println("FPS - No finger detected");
      #endif
      return FPS_NO_FINGER_DETECT;
    default:
      #ifdef DEBUGPJ2    
        Serial.println("FPS - Unknown error");
      #endif
      return FPS_OTHER_ERROR;
  }

  // If we get to here, we have detected a finger and taken its image.

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      #ifdef DEBUGPJ2
        Serial.println("FPS - Image converted");
      #endif
      break;
    default:
      #ifdef DEBUGPJ2    
        Serial.println("FPS - Unknown error");
      #endif
      return FPS_OTHER_ERROR;
  }
  
  // If we get to here we have converted the image successfully...whatever that means.
  
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) 
    {
    #ifdef DEBUGPJ2  
      Serial.println("FPS - Found a print match!");
    #endif
    }
    else if (p == FINGERPRINT_PACKETRECIEVEERR) 
          {
          #ifdef DEBUGPJ2
            Serial.println("FPS - Communication error");
          #endif
          return FPS_OTHER_ERROR;
          } 
          else if (p == FINGERPRINT_NOTFOUND) 
                {
                #ifdef DEBUGPJ2
                  Serial.println("FPS - Did not find a match");
                #endif
                return FPS_FINGER_NO_MATCH;
                } 
                else 
                  {
                  #ifdef DEBUGPJ2
                    Serial.println("FPS - Unknown error");
                  #endif
                  return FPS_OTHER_ERROR;
                  }   
  
  // if we get to here we have got a succesful match
  #ifdef DEBUGPJ2
    Serial.print("Found ID #"); Serial.print(finger.fingerID); 
    Serial.print(" with confidence of "); Serial.println(finger.confidence); 
  #endif
  return finger.fingerID;
} // END - FPSgetFingerprintID()


// ================ getFingerprintID()
// Detailed reader function, shows whats happening every step of the way.
// Use this OR getFingerprintIDez().
// returns -1 if failed, otherwise returns ID #
int getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  //Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  //Serial.print(" with confidence of "); Serial.println(finger.confidence); 

}  // END - getFingerprintID()


// ================ getFingerprintIDez()
// Simple reader.
// Use this OR getFingerprintID().
// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
} // END - getFingerprintIDez()


// ================ FPSTurnFootlightOn()
void FPSTurnFootlightOn()
{
	#ifdef DEBUGPJ2
		Serial.println("S: FPSTurnFootlightOn");
	#endif
	FPS_FOOTLIGHT_LED_ACTUATOR_STATUS = true;  // set the status of the relevant ACTUATOR. localactions() will do the rest.
} // END - FPSTurnFootlightOn()


// ================ FPSTurnFootlightOff()
void FPSTurnFootlightOff()
{
	#ifdef DEBUGPJ2
		Serial.println("S: FPSTurnFootlightOff");
	#endif
	FPS_FOOTLIGHT_LED_ACTUATOR_STATUS = false;  // set the status of the relevant ACTUATOR. localactions() will do the rest.
} // END - FPSTurnFootlightOn()







// ================ FPSUnlockDoor()
void FPSUnlockDoor()
{
	#ifdef DEBUGPJ2
		Serial.println("S: FPSUnlockDoor");
	#endif
	FPS_DOOR_LATCH_ACTUATOR_STATUS = true;  // set the status of the relevant ACTUATOR. localactions() will do the rest.
} // END - FPSUnlockDoor()


// ================ FPSLockDoor()
void FPSLockDoor()
{
	#ifdef DEBUGPJ2
		Serial.println("S: FPSLockDoor");
	#endif
	FPS_DOOR_LATCH_ACTUATOR_STATUS = false;  // set the status of the relevant ACTUATOR. localactions() will do the rest.
} // END - FPSLockDoor()

#endif // FINGER
