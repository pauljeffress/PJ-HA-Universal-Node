//
// ================ RMT_PWR Functions ===================
// 
//
#include "a.h" // My global defines and extern variables to help multi file comilation.

#ifdef RMT_PWR

// ================ RMT_PWROn()
//
void RMT_PWROn()
{
  #ifdef DEBUGPJ2
    Serial.println("RMT_PWROn()");
  #endif
  
  //RMT_PWR_State = 1;
  //digitalWrite(RMT_PWR_ENA_PIN, RMT_PWR_State);  // turn on remote ATX PSU.

  // we need the remote PSU to be switched on if it isn't already. It may already 
  //   have been requested for another sensor/actuator on this node, so don't
  //   execute the below code unless you have to as there is no point in having
  //   multiple delays!
  if (!RMT_PWR_State) 
    {
    RMT_PWR_State = 1;
    digitalWrite(RMT_PWR_ENA_PIN, RMT_PWR_State);  // turn on remote ATX PSU.
    delay(1000);   // wait 1 sec for remote PSU to come up and stabilise.
    }
} 

// ================ RMT_PWROff()
//
void RMT_PWROff()
{
  #ifdef DEBUGPJ2
    Serial.println("RMT_PWROff()");
  #endif
  
  RMT_PWR_State = 0;
  digitalWrite(RMT_PWR_ENA_PIN, RMT_PWR_State);  // turn off remote ATX PSU.
} 

#endif RMT_PWR
