//
// ================ LEDSTRIP Functions ===================
// 
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef LEDSTRIP

void setupLEDStrips() // gets called during setup()
  {
    #ifdef DEBUGPJ
      Serial.println("Initialisising LEDs");
      //delay(5000);
    #endif
   
    #ifdef RMT_PWR
      RMT_PWROn();  // If required, fire up the RMT_PWR supply for the LEDS during this setup test script.
    #endif

    // LED Strip 1 stuff
    if (LEDStrip1Type) // if its a PIXEL LED Strip...
      {
      // tell FastLED there's blah pixels of type blah on pin blah
      FastLED.addLeds<WS2811, LEDSTRIP1_DATAPIN, GRB>(pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS);
      // test LED strip...
      #ifdef DEBUGPJ
        Serial.println("All White");
      #endif
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB(50,50,50)); 
      FastLED.show();
      delay(2000);
      #ifdef DEBUGPJ
        Serial.println("LEDS - off");
      #endif
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB::Black); 
      FastLED.show();
      delay(500);
      // TEST LED Strip by moving a single white led 
      #ifdef DEBUGPJ
        Serial.println("TEST LED Strip by moving a single white led");
      #endif
      for(int whiteLed = 0; whiteLed < LEDSTRIP1_NUMPIXELS; whiteLed = whiteLed + 1) 
        {
        // Turn our current led on to white, then show the leds
        pixelledstrip1_leds[whiteLed] = CRGB::White;
        // Show the leds (only one of which is set to white, from above)
        FastLED.show();
        // Wait a little bit
        delay(100);
        // Turn our current led back to black for the next loop around
        pixelledstrip1_leds[whiteLed] = CRGB::Black;
        }
      // set all to OFF/Black now
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB::Red); 
      FastLED.show();
      delay(2000);
      // show full RED, then GREEN, then BLUE.
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB::Green); 
      FastLED.show();
      delay(2000);
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB::Blue); 
      FastLED.show();
      delay(2000);  
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB::Black); 
      FastLED.show();
      }
    else  // if its a DUMB LED strip...
      {
      // NOT IMPLEMENTED YET
      }

  // LED Strip 2 stuff - NOT IMPLEMENTED YET

  #ifdef DEBUGPJ
    Serial.println("LEDs Initialised");
  #endif

  #ifdef RMT_PWR
    RMT_PWROff();  // If required, turn off  the RMT_PWR supply for the LEDS now that this setup test script is finished.
  #endif
   
  }  // END - setupLEDStrips()


void updateStaticLEDStrip(int stripnum) // update the strip to use most recently set parameters.
  {
    #ifdef DEBUGPJ2
      Serial.print("updating Strip: ");
      Serial.print(stripnum);
    #endif  
    #ifdef RMT_PWR
      RMT_PWROn();  // If required, fire up the RMT_PWR supply for the LEDS.
    #endif 
    FastLED.setBrightness(LEDStrip1BrightnessValue);
    fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB(LEDStrip1RedValue,LEDStrip1GreenValue,LEDStrip1BlueValue)); 
    FastLED.show();
    #ifdef RMT_PWR
      if ((LEDStrip1BrightnessValue == 0) || ((LEDStrip1RedValue == 0)  && (LEDStrip1GreenValue == 0) && (LEDStrip1BlueValue == 0)))
        {
        RMT_PWROff();  // If Brightness turned to zero or all three colours are zero then turn off  the RMT_PWR supply for the LEDS.
        }
    #endif
  }

void setAnimatedLEDStripMode(int stripnum, int stripmode)  // action the required mode for the strip.
  {
   #ifdef DEBUGPJ2
    Serial.print("Strip: ");
    Serial.print(stripnum);
    Serial.print(" Mode: ");
    Serial.print(stripmode);
    Serial.println(" ");
    #endif

  // set a LED strip to the right mode
  switch (stripmode) 
    {
    case (0): // All Off - Black
      #ifdef DEBUGPJ2
        Serial.println("LED=OFF");
      #endif
      RMT_PWROn(); // Turns on RMT_PWR unit if its not already on.
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB::Black); 
      FastLED.show();
      RMT_PWROff(); // Turns off RMT_PWR unit.
      break;

    case (1): // White 100%
      #ifdef DEBUGPJ2
        Serial.println("LED=WHITE100%");
      #endif
      RMT_PWROn(); // Turns on RMT_PWR unit if its not already on.
      FastLED.setBrightness(250);
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB::White); 
      FastLED.show();
      break;
      
    case (2): // White 75%
      #ifdef DEBUGPJ2
        Serial.println("LED=WHITE75%");
      #endif
      RMT_PWROn(); // Turns on RMT_PWR unit if its not already on.
      FastLED.setBrightness(196);
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB::White); 
      FastLED.show();
      break;

    case (3): // White 50%
      #ifdef DEBUGPJ2
        Serial.println("LED=WHITE50%");
      #endif
      RMT_PWROn(); // Turns on RMT_PWR unit if its not already on.
      FastLED.setBrightness(128);
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB::White); 
      FastLED.show();
      break;
      
    case (4): // White 25%
      #ifdef DEBUGPJ2
        Serial.println("LED=WHITE25%");
      #endif
      RMT_PWROn(); // Turns on RMT_PWR unit if its not already on.
      FastLED.setBrightness(64);
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB::White); 
      FastLED.show();
      break;

    case (11): // Red 100%
      #ifdef DEBUGPJ2
        Serial.println("LED=Red100%");
      #endif
      RMT_PWROn(); // Turns on RMT_PWR unit if its not already on.
      FastLED.setBrightness(250);
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB::Red); 
      FastLED.show();
      break;

    case (21): // Green 100%
      #ifdef DEBUGPJ2
        Serial.println("LED=Green100%");
      #endif
      RMT_PWROn(); // Turns on RMT_PWR unit if its not already on.
      FastLED.setBrightness(250);
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB::Green); 
      FastLED.show();
      break;
      
    case (31): // Blue 100%
      #ifdef DEBUGPJ2
        Serial.println("LED=Blue100%");
      #endif
      RMT_PWROn(); // Turns on RMT_PWR unit if its not already on.
      FastLED.setBrightness(250);
      fill_solid( pixelledstrip1_leds, LEDSTRIP1_NUMPIXELS, CRGB::Blue); 
      FastLED.show();
      break;
      
  default:  // i.e. we have been sent request to use a mode that we have not implemented.
      #ifdef DEBUGPJ2
        Serial.println("Mode not implemented in setPixelLEDStripMode()");
      #endif

    } // end of switch statement
  } // End - setAnimatedLEDStripMode()


#endif /*===== LEDSTRIP related functions ===========*/


