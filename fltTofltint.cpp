// Subroutine to convert a Float to what I call a fltint, a Float Int :)
// In order to safely send Floats between different platforms (AVR, SAMD etc) which treat Floats
// differently, before I send them I simply multiply the by 100 and store the whole number in a signed long/32bit.
// Then on the receiving side I just regenerate the Float from the fltint by dividing it by 100 and putting that val in a Float.
// The process assumes I don't want more than 2 dec places as thats all that gets kept.

#include "a.h" // My global defines and extern variables to help multi file comilation.

//
// ======== fltTofltint()
//
long fltTofltint(float floatIn){
    
    return(floatIn * 100);  // multiply by 100 and return it as a long.

} // END - fltTofltint()
