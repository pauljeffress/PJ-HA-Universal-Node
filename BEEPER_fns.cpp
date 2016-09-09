//
// Misc BEEPER related functions
//

// ================ ToneBeep()
//



#ifdef BEEPER

void ToneBeep(int note, int duration)
{
	#ifdef DEBUGPJ2
		Serial.println("S:ToneBeep()");
	#endif
	
    tone(BEEPERPIN, note, duration);

	// delay(pauseBetweenNotes);
	// stop the tone playing:
	//	noTone(SPEAKER_PIN);
	
} // ToneBeep()

#endif BEEPER
