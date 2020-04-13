/*===== TSL2651 related functions ===========*/

#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef TSL2651 

void getTSL2651() 
  {
  /* Get a new sensor event */ 
  sensors_event_t event;
  tsl.getEvent(&event);
 
  /* Display the results (light is measured in lux) */
  if (event.light)
  {
    CONSOLE_PORT.print(event.light); CONSOLE_PORT.println(" lux");
  }
  else
  {
    /* If event.light = 0 lux the sensor is probably saturated
       and no reliable data could be generated! */
    CONSOLE_PORT.println("Sensor overload");
  }
  
  // put values into the global array.
  TSL2651Data[0] = event.light; // Visible Light
  TSL2651Data[1] = 9999;  // IR Light
}   // end getTSL2651()

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information) (Written by Adafruit)
*/
/**************************************************************************/
void displaySensorDetails(void)
{
  sensor_t sensor;
  tsl.getSensor(&sensor);
  CONSOLE_PORT.println("------------------------------------");
  CONSOLE_PORT.print  ("Sensor:       "); CONSOLE_PORT.println(sensor.name);
  CONSOLE_PORT.print  ("Driver Ver:   "); CONSOLE_PORT.println(sensor.version);
  CONSOLE_PORT.print  ("Unique ID:    "); CONSOLE_PORT.println(sensor.sensor_id);
  CONSOLE_PORT.print  ("Max Value:    "); CONSOLE_PORT.print(sensor.max_value); CONSOLE_PORT.println(" lux");
  CONSOLE_PORT.print  ("Min Value:    "); CONSOLE_PORT.print(sensor.min_value); CONSOLE_PORT.println(" lux");
  CONSOLE_PORT.print  ("Resolution:   "); CONSOLE_PORT.print(sensor.resolution); CONSOLE_PORT.println(" lux");  
  CONSOLE_PORT.println("------------------------------------");
  CONSOLE_PORT.println("");
  delay(500);
}

/**************************************************************************/
/*
    Configures the gain and integration time for the TSL2561 (Written by Adafruit)
*/
/**************************************************************************/
void configureSensor(void)
{
  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoRange(true);            /* Auto-gain ... switches automatically between 1x and 16x */
  
  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  // tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  /* Update these values depending on what you've set above! */ 
  #ifdef DEBUGPJx 
    CONSOLE_PORT.println("------------------------------------");
    CONSOLE_PORT.print  ("Gain:         "); CONSOLE_PORT.println("Auto");
    CONSOLE_PORT.print  ("Timing:       "); CONSOLE_PORT.println("13 ms");
    CONSOLE_PORT.println("------------------------------------");
  #endif
}



#endif TSL2651 /*===== TSL2651 related functions ===========*/


