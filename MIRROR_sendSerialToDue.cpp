//
// ================ MIRROR_sendSerialToDue() Function ===================
// 
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef OCEANMIRROR

//
// ======= MIRROR_sendSerialToDue()
//
void MIRROR_sendSerialToDue(int sendSerDev, float sendparam) {
  // This is called whenever the Master/Mega needs to send the DUE some new info, because one of
  // the Dev4XX's it needs has changed. 
  // Custom serial protocol just for this job.
  //    Header - two byte frame header of "PJ" or 0x50, 0x4A.
  //    DevXXX - an Integer sent as two bytes.
  //    Value - a Flot sent as ? bytes.

  uint8_t sendSerDevHighByte, sendSerDevLowByte = 0;     // high and low part of int to transmit
  uint8_t sendparamLeftByte, sendparamRightByte = 0;     // left and right side of decimal place of float to transmit
  
  // convert sendSerDev integer to two bytes, as it is currently a 16 bit number.
  // I am not allowing for negative numbers, as they will not occur in my Ocean Mirror scenario.
  sendSerDevHighByte = sendSerDev / 256;
  sendSerDevLowByte = sendSerDev - (sendSerDevHighByte * 256);

  // convert sendparam Float to two bytes.
  // I am only allowing for floats that are positive, not > 256, and only two dec place.
  // It allows this conversionto stay simple and will cover what I need for my Ocean Mirror scenario.
  sendparamLeftByte = sendparam;    // will conver num to integer
  sendparamRightByte = (sendparam - sendparamLeftByte)* 10;  // will give us the first digit after the decimal place.

  #ifdef DEBUGPJ2
    Serial.println("===== Sending Serial Frame ======");
    Serial.println("PJ");
    Serial.print("sendSerDev="); Serial.print(sendSerDev); Serial.print("  High Byte="); Serial.print(sendSerDevHighByte); Serial.print("  Low Byte="); Serial.println(sendSerDevLowByte );
    Serial.print("sendparam="); Serial.print(sendparam); Serial.print("  Left Byte="); Serial.print(sendparamLeftByte); Serial.print("  Right Byte="); Serial.println(sendparamRightByte);
    Serial.println("=================================");
  #endif
  
  // Serial tx the header and data to the Due.
  Serial1.write(0x50);  
  Serial1.write(0x4A);
  Serial1.write(sendSerDevHighByte);
  Serial1.write(sendSerDevLowByte);
  Serial1.write(sendparamLeftByte);
  Serial1.write(sendparamRightByte);  

} // END - MIRROR_sendSerialToDue()


#endif // OCEANMIRROR
