/**********************************************************************

  ErrorMonitor.cpp
  COPYRIGHT (c) Costin Grigoras

  Part of DCC++ BASE STATION for the Arduino / extended for Pololu shield

**********************************************************************/

#include "DCCpp_Uno.h"
#include "ErrorMonitor.h"
#include "Comm.h"

///////////////////////////////////////////////////////////////////////////////

long int lastPowerOffTime = 0;

// #define ERROR_MONITOR_DEBUG

void errorCheck() {
  if (digitalRead(SIGNAL_ENABLE_PIN_PROG) == LOW)
    lastPowerOffTime = millis();

  if (digitalRead(ERROR_MONITOR_PIN) == 0 && digitalRead(SIGNAL_ENABLE_PIN_PROG) == HIGH) {
    digitalWrite(SIGNAL_ENABLE_PIN_PROG, LOW);                                                    // disable both Motor Shield Channels
    digitalWrite(SIGNAL_ENABLE_PIN_MAIN, LOW);                                                    // regardless of which caused current overload

    if (millis() - lastPowerOffTime < 1000) {
      // at startup time large capacitors charging simultaneously can trigger the error state
      // re-enable the power for a few milliseconds to allow them to charge
      for (int i = 0; i < 10; i++) {
        digitalWrite(SIGNAL_ENABLE_PIN_MAIN, HIGH);
        digitalWrite(SIGNAL_ENABLE_PIN_PROG, HIGH);

        delay(1);

        if (digitalRead(ERROR_MONITOR_PIN) == 0) {
          digitalWrite(SIGNAL_ENABLE_PIN_MAIN, LOW);
          digitalWrite(SIGNAL_ENABLE_PIN_PROG, LOW);
        }
        else {
          #ifdef ERROR_MONITOR_DEBUG
          INTERFACE.print("\nWorked@");
          INTERFACE.println(i);
          #endif
          
          break;
        }
      }

      #ifdef ERROR_MONITOR_DEBUG
      if (digitalRead(ERROR_MONITOR_PIN) == 0)
        INTERFACE.println("\nRecovery didn't work");
      #endif
    }
    #ifdef ERROR_MONITOR_DEBUG
    else
      INTERFACE.println("\nNo recovery");
    #endif

    if (digitalRead(ERROR_MONITOR_PIN) == 0) {
      #ifdef ERROR_MONITOR_DEBUG
      INTERFACE.print("\nShort circuit\n");
      #endif
      
      INTERFACE.print("<p2>");                                                                            // print corresponding error message
    }
  }
}
