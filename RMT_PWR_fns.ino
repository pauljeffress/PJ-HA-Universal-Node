//
// Misc RMT_PWR related functions
//


#ifdef RMT_PWR

// ================ RMT_PWROn()
//
void RMT_PWROn()
{
  #ifdef DEBUGPJ2
    Serial.println("RMT_PWROn()");
  #endif
  
  RMT_PWR_State = 1;
  digitalWrite(RMT_PWR_ENA_PIN, RMT_PWR_State);  // turn on remote ATX PSU.
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
