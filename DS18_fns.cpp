#ifdef DS18 /*===== DS18 related functions ===========*/

void DS18_check()
  {
  #ifndef SLEEPY // if not in sleep mode we have a sample timer in place, otherwise just sample on waking
    if (TXinterval > 0)
    {
    int currPeriod = millis()/(TXinterval*1000);
      if (currPeriod != lastPeriod) { // interval elapsed ?
  #endif
      #ifdef DEBUG
      Serial.print(" Requesting temperatures..."); // call sensors.requestTemperatures() to issue a global temperature request to all devices on the bus
      #endif
      sensors.requestTemperatures(); // Send the command to get temperatures
      #ifdef DEBUG
      Serial.println("DONE");
      Serial.print("Temperature for DS18 device 48 is: ");
      Serial.println(sensors.getTempCByIndex(0)); // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
      Serial.print("Temperature for DS18 Device 50 is: ");
      Serial.println(sensors.getTempCByIndex(1)); // Why "byIndex"? 
      #endif
      delay(50);
  #ifndef SLEEPY
      }
    }
  #endif
  }  // End of DS18_check()

void DS18_sleepyStuff()
  {
  #ifdef SLEEPY //////// SLEEP with Watchdog Timer ////////////
  radio.sleep(); // sleep radio to save power
  // Enter power down state for 8 s with ADC and BOD module disabled Moteino (with RFM12B, no SPI Flash): 6 uA @ Vin = 5 V, 4 V, and 3.3 V
  // with no red power LED, Arduino Pro Mini takes 20 uA @ Vin=4.0, 14 @ 3.3V
      Serial.println(" going to sleep in 1 second");
      delay(1000);
      for (byte i = 0; i < 75; ++i) { // sleep 10 mins 75x8
      LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); }
  /////////////////do stuff////////////////////////////
  #endif
  } // End of DS18_sleepyStuff()
  
#endif /*===== DS18 related functions ===========*/
