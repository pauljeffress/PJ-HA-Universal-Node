/*===== WEATHERSHIELD related functions ===========*/

#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef MOTEINOWEATHER 

void getWeatherShield() 
  {
  int temperature = SI7021sensor.getCelsiusHundredths();
  int humidity = SI7021sensor.getHumidityPercent();
  // this driver should work for SI7020 and SI7021, this returns 20 or 21
  //int deviceid = SI7021sensor.getDeviceId();  // PJ 18-6-16 commented out as it
  // was hanging when called and I don't need this value.
  #ifdef DEBUGPJ2
    Serial.println("*Si7021*");
    Serial.print("C:");Serial.print(temperature);
    Serial.print(" H:");Serial.print(humidity);Serial.print("% ");
  //  Serial.print("DID: ");Serial.print(deviceid); // PJ 18-6-16 commented out see few lines above for reason.
  #endif
  
  //*************** READING BATTERY VOLTAGE *********************
  //turn MOSFET ON and read voltage, should give a valid reading
  pinMode(MOTEINO_WEATHERSHIELD_V_ENABLE_PIN, OUTPUT);
  digitalWrite(MOTEINO_WEATHERSHIELD_V_ENABLE_PIN, LOW);
  #ifdef DEBUGPJ2
    Serial.print(" BATT:");
    Serial.println(analogRead(MOTEINO_WEATHERSHIELD_V_VALUE_PIN));
  #endif
  pinMode(MOTEINO_WEATHERSHIELD_V_ENABLE_PIN, INPUT); //put MOTEINO_WEATHERSHIELD_V_ENABLE_PIN in HI-Z mode (to allow mosfet gate pullup to turn it OFF)
  //*************** READING BATTERY VOLTAGE *********************
    

// **********************************************
// I have commented out all of this code that reads the BMP180 on the WeatherShield as my current WeatherShield is
// missing a BMP180 chip.  See my Evernotes.

//   char status;
//   double T,P,p0,a;

//   // Loop here getting pressure readings every 10 seconds.

//   // If you want sea-level-compensated pressure, as used in weather reports,
//   // you will need to know the altitude at which your measurements are taken.
//   // We're using a constant called ALTITUDE in this sketch:
//   #ifdef DEBUGPJ2
//   Serial.println("*BMP180*");
//   Serial.print("provided alt:");
//   Serial.print(MOTEINO_WEATHERSHIELD_ALTITUDE);
//   Serial.println("m");
//   #endif
//   // If you want to measure altitude, and not pressure, you will instead need
//   // to provide a known baseline pressure. This is shown at the end of the sketch.
//   // You must first get a temperature measurement to perform a pressure reading.
//   // Start a temperature measurement:
//   // If request is successful, the number of ms to wait is returned.
//   // If request is unsuccessful, 0 is returned.
//   status = BMP180pressure.startTemperature();
//   if (status != 0)
//   {
//     // Wait for the measurement to complete:
//     delay(status);

//     // Retrieve the completed temperature measurement:
//     // Note that the measurement is stored in the variable T.
//     // Function returns 1 if successful, 0 if failure.

//     status = BMP180pressure.getTemperature(T);
//     if (status != 0)
//     {
//       #ifdef DEBUGPJ2
//       // Print out the measurement:
//       Serial.print("C: ");
//       Serial.print(T,2);
//       Serial.println("deg");
//       #endif
      
//       // Start a pressure measurement:
//       // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
//       // If request is successful, the number of ms to wait is returned.
//       // If request is unsuccessful, 0 is returned.

//       status = BMP180pressure.startPressure(3);
//       if (status != 0)
//       {
//         // Wait for the measurement to complete:
//         delay(status);

//         // Retrieve the completed pressure measurement:
//         // Note that the measurement is stored in the variable P.
//         // Note also that the function requires the previous temperature measurement (T).
//         // (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
//         // Function returns 1 if successful, 0 if failure.

//         status = BMP180pressure.getPressure(P,T);
//         if (status != 0)
//         {
//           #ifdef DEBUGPJ2
//           // Print out the measurement:
//           Serial.print("abs pressure: ");
//           Serial.print(P,2);
//           Serial.print(" mb, ");
//           Serial.print(P*0.0295333727,2);
//           Serial.println(" inHg");
//           #endif
//           // The pressure sensor returns abolute pressure, which varies with altitude.
//           // To remove the effects of altitude, use the sealevel function and your current altitude.
//           // This number is commonly used in weather reports.
//           // Parameters: P = absolute pressure in mb, ALTITUDE = current altitude in m.
//           // Result: p0 = sea-level compensated pressure in mb

//           p0 = BMP180pressure.sealevel(P,MOTEINO_WEATHERSHIELD_ALTITUDE); // we're at 1655 meters (Boulder, CO)
//           #ifdef DEBUGPJ2
//           Serial.print("relative (sea-level) pressure: ");
//           Serial.print(p0,2);
//           Serial.print(" mb/HPa, ");
//           #endif
//           // On the other hand, if you want to determine your altitude from the pressure reading,
//           // use the altitude function along with a baseline pressure (sea-level or other).
//           // Parameters: P = absolute pressure in mb, p0 = baseline pressure in mb.
//           // Result: a = altitude in m.

// //          a = BMP180pressure.altitude(P,p0);
// //          Serial.print("computed altitude: ");
// //          Serial.print(a,0);
// //          Serial.print(" meters, ");
// //          Serial.print(a*3.28084,0);
// //          Serial.println(" feet");
//         }
//         #ifdef DEBUGPJ2
//         else Serial.println("error retrieving pressure measurement\n");
//         #endif
//       }
//       #ifdef DEBUGPJ2
//       else Serial.println("error starting pressure measurement\n");
//       #endif
//     }
//     #ifdef DEBUGPJ2
//     else Serial.println("error retrieving temp measurement\n");
//     #endif
//   }
//   #ifdef DEBUGPJ2
//   else Serial.println("error starting temp measurement\n");
//   #endif
  
  // put values into the global array.
//   WeatherShieldData[0] = p0; // Hpa
  WeatherShieldData[0] = 999;   // see notes above about missing BMP180 sensor, so I'm just setting a dummy value here for now.
  WeatherShieldData[1] = humidity;  // %
  WeatherShieldData[2] = temperature;   // degC

  
}   // end getWeatherShield()

#endif MOTEINOWEATHER /*===== WEATHERSHIELD related functions ===========*/


