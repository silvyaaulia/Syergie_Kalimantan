#include "Syergie.h"

#include "Arduino.h"

//--------------------------------------------------------------------------------------
// Sets the pins to be used for current sensors
//--------------------------------------------------------------------------------------
void Proximity::pinRPM(unsigned int _inPinPro)
{
  inPinPro = _inPinPro;
  prevmillis = 0;
  prevstate = HIGH;
  fanoff = true; 
}

//--------------------------------------------------------------------------------------
int Proximity::calcRPM()
{
  currentRead = analogRead(inPinPro);
  if (currentRead < 880) {
    currentstate = LOW;
  } else {
    currentstate = HIGH;
  }
  //currentstate = digitalRead(inPinPro);
  if( prevstate != currentstate) // If there is change in input
   {
     if( currentstate == LOW ) // If input only changes from HIGH to LOW
       {
         duration = ( micros() - prevmillis ); // Time difference between revolution in microsecond
         prevmillis = micros(); // store time for nect revolution calculation
         fanoff = true;
       }
     else
      {
         rpm = (312000000/(duration)); // rpm = (1/ time millis)*1000*1000*60;
      }
   }
  prevstate = currentstate; // store this scan (prev scan) data for next scan

  if (((micros() - prevmillis) > 60000000) && (fanoff==true))
    {
      fanoff = false;
      rpm = 0;
    }
  
  return rpm;
}
