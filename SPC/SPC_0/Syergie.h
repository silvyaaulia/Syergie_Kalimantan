#ifndef Syergie_h
#define Syergie_h

#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Proximity
{
  public:
    void pinRPM(unsigned int _inPinPro);

    int Proximity::calcRPM();

    //Useful value variables
    int rpm; // RPM value

  private:

    //Set Voltage and current input pins
    unsigned int inPinPro;

    //--------------------------------------------------------------------------------------
    // Variable declaration for calculation procedure
    //--------------------------------------------------------------------------------------
    unsigned long prevmillis; // To store time
  unsigned long duration; // To store time difference
  unsigned long refresh; // To store time for refresh of reading

  boolean currentstate; // Current state of Proximity input scan
  boolean prevstate; // State of Proximity sensor in previous scan
  boolean fanoff;

};
