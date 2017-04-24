//
// ================ sendDevValueToSerial() Function ===================
// 
//
#include "a.h" // My global defines and extern variables to help multi file comilation.


#ifdef LEDSTRIPS_REMOTE  // OR in any other #defines that may need this function.

//
// ======= sendDevValueToSerial()
//
void sendDevValueToSerial(int sendSerDev, int sendSerType, int sendSerInt, float sendSerFloat) {
  // This is called whenever a Node needs to send a DEVice value to another system attached to the Node via serial.
  // ASSUMPTION - the Node is using Serial1 to connect to the other system.
  // ASSUMPTION - all values are positive.
  // This routine is usually called when the Node receives a new or changed value for a DEVice.
  // Custom serial protocol just for this job.
  //  Frame Structure
  //    Header - two bytes - frame header of "PJ" or 0x50, 0x4A.
  //      DevXXX (sendSerDev) - two bytes - the DEVice number as an Integer, sent as two bytes.
  //      DEVice type (sendSerType) - one byte - 0 = integer, 1 = real.  No other types supported as yet.
  //      IntValue (sendSerInt) - two bytes - value of the DEVice if its an Integer DEVice. Otherwise set to zero.
  //      FltValue (sendSerFloat)- two bytes - value of the DEVice if it is a Float DEVice.  Otherwise set to zero.
  //    Trailer - two bytes - frame trailer of "pj" or 0x70, 0x6A.
  //    

  uint8_t sendSerDevHighByte, sendSerDevLowByte = 0;     // high and low part of integer to transmit
  uint8_t sendSerTypeByte = 0;                           // byte to transmit
  uint8_t sendSerIntHighByte, sendSerIntLowByte = 0;     // high and low part of integer to transmit
  uint8_t sendSerFloatLeftByte, sendSerFloatRightByte = 0;  // left and right side of decimal place of float to transmit
  
  // convert sendSerDev integer to two bytes, as it is currently a 16 bit number.
  // I am not allowing for negative numbers here.
  sendSerDevHighByte = sendSerDev / 256;
  sendSerDevLowByte = sendSerDev - (sendSerDevHighByte * 256);

  // convert sendSerType 16 bit integer to single 8 bit byte, as its values are always > 255.
  sendSerTypeByte = sendSerType;

  // convert sendSerInt integer to two bytes, as it is currently a 16 bit number.
  // I am not allowing for negative numbers here.
  sendSerIntHighByte = sendSerInt / 256;
  sendSerIntLowByte = sendSerInt - (sendSerIntHighByte * 256);

  // convert sendSerFloat Float to two bytes.
  // I am only allowing for floats that are positive, not > 256, and only two dec place.
  // It allows this conversion to stay simple and will cover what I need for most scenarios.
  sendSerFloatLeftByte = sendSerFloat;    // will conver num to integer
  sendSerFloatRightByte = (sendSerFloat - sendSerFloat)* 10;  // will give us the first digit after the decimal place.

  #ifdef DEBUGPJ2
    Serial.println("===== Sending Serial Frame ======");
    Serial.println("Header=PJ");
    Serial.print("sendSerDev="); Serial.print(sendSerDev); Serial.print("  High Byte="); Serial.print(sendSerDevHighByte); Serial.print("  Low Byte="); Serial.println(sendSerDevLowByte );
    Serial.print("sendSerType="); Serial.print(sendSerType); Serial.print("  Single Byte="); Serial.println(sendSerTypeByte);
    Serial.print("sendSerInt="); Serial.print(sendSerInt); Serial.print("  High Byte="); Serial.print(sendSerIntHighByte); Serial.print("  Low Byte="); Serial.println(sendSerIntLowByte );
    Serial.print("sendSerFloat="); Serial.print(sendSerFloat); Serial.print("  Left Byte="); Serial.print(sendSerFloatLeftByte); Serial.print("  Right Byte="); Serial.println(sendSerFloatRightByte);
    Serial.println("Trailer=pj");
    Serial.println("=================================");
  #endif
  
  // Serial tx the frame to the other system.
  Serial1.write(0x50);  
  Serial1.write(0x4A);
  Serial1.write(sendSerDevHighByte);
  Serial1.write(sendSerDevLowByte);
  Serial1.write(sendSerTypeByte);
  Serial1.write(sendSerIntHighByte);
  Serial1.write(sendSerIntLowByte);  
  Serial1.write(sendSerFloatLeftByte);
  Serial1.write(sendSerFloatRightByte);  
  Serial1.write(0x70);  
  Serial1.write(0x6A);

} // END - sendDevValueToSerial()


#endif 
