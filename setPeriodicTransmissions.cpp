//
// ================ setPeriodicTransmissions()
//

#include "a.h" // My global defines and extern variables to help multi file comilation.

void setPeriodicTransmissions()
  {
  #ifdef DEBUG
      Serial.println("Flag periodics");
  #endif
  
  #ifndef SLEEPY // no timer if sleepy, just send when woken up
    if (TXinterval > 0)
      {
      int currPeriod = millis()/(TXinterval*1000);
      if (currPeriod != lastPeriod) { // interval elapsed ?
        lastPeriod = currPeriod;
        // the 'sendX = true' code below gets executed here, have a look at the IF brackets
  #endif

  // list of sensordata to be sent periodically.. remove comment to include parameter in transmission
  #ifdef SLEEPY
    if  (curPIRState != HIGH) { // not high, no presence, we can send stats
      if (wakeUp != true)  // don't increase uptime counter when just started up
        {
        upTime = upTime + (TXinterval/60); // upTime in SLEEPY is measuring cumulative TXintervals, neglecting real uptime, cant use millis when sleeping
        }
  #endif

        //No matter what type of node, send these few.
        send0 = true; // send uptime
        send1 = true; // send transmission interval
        send3 = true; // node sw version
        send4 = true; // voltage level
        send5 = true; // Ack status
        //send11 = true; // Compile date
        //send12 = true; // Compile file
        // Send the remainder if they are actually used on this node.
        
        #ifdef RFNODETYPE
            send2 = true; // signal strength
        #endif

        #ifdef BUTTON1 or BUTTON2
          send6 = true; // Button toggle mode
          send7 = true; // Button timer value
        #endif 

        #ifdef ACTUATOR1
            send16 = true; // ACTUATOR1 status
        #endif 
        #ifdef ACTUATOR2
            send17 = true; // ACTUATOR2 status
        #endif 
        #ifdef ACTUATOR3
            send18 = true; // ACTUATOR3 status
        #endif 
        #ifdef ACTUATOR4
            send19 = true; // ACTUATOR4 status
        #endif   

        #ifdef BUTTON1
            send40 = true; // Button1 status
        #endif
        #ifdef BUTTON2
            send42 = true; // Button2 status
        #endif         

        #ifdef PIR1
            send41 = true; // PIR1 status
        #endif 
        #ifdef PIR2
            send43 = true; // PIR2 status
        #endif 

        #ifdef TSL2651
          send52 = true;  // TSL2651 visible light level
          send53 = true;  // TSL2651 IR light level
        #endif

        #ifdef MOTEINOWEATHER
          send51 = true;  // pressure reading
          send54 = true;  // humidity reading
          send55 = true;  // temperature reading
        #endif
        
        #ifdef PING1x
            send64 = true; 
            send65 = true;
            send66 = true;
            send67 = true;
        #endif

        #ifdef LEDSTRIP
            send200 = true;
            send201 = true;
            send202 = true;
            send203 = true;
            send204 = true;
            send221 = true;
            send231 = true;
            send291 = true;
        #endif

        
  #ifdef SLEEPY
      }
  #endif

        #ifdef DHT
          send48 = true; // send temperature DHT or DS18
          send49 = true; // send humidity
        #endif
        
        #ifdef DS18
          send48 = true; // send temperature DHT or DS18
          send50 = true; // send temperature DS18 sensor 2
        #endif
  
  #ifndef SLEEPY
  }
  }
  #endif

  } // End of setPeriodicTransmissions()
