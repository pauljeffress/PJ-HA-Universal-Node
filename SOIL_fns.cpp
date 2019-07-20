/*===== SOIL Moisture related functions ===========*/

#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef SOILMOISTURE1 || SOILMOISTURE2
//This is a function used to get the soil moisture content
int readSoilMoisture(int powerPin, int moisturePin)
{
    int soilValue = 0;  // set to obvious number so I can spot error
    int sample = 0;
    digitalWrite(powerPin, HIGH);//turn power to sensor "On"
    delay(10);//wait 10 milliseconds 
    Serial.print("Starting soil samples ");
    for(int i = 0; i<5; i++)
        {
            sample = analogRead(moisturePin);//Read the SIG value form sensor 
            Serial.print(sample);
            Serial.print(" ");
            soilValue += sample; // Sum the samples
            delay(100); //wait between samples
        }
    soilValue = soilValue / 5; // average the 5 samples
    Serial.println(soilValue);
    digitalWrite(powerPin, LOW);//turn power to sensor "Off"
    return soilValue;//send current moisture value
}    


#endif // SOILMOISTURE1 || SOILMOISTURE2