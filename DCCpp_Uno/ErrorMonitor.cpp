/**********************************************************************

ErrorMonitor.cpp
COPYRIGHT (c) Costin Grigoras

Part of DCC++ BASE STATION for the Arduino / extended for Pololu shield

**********************************************************************/

#include "DCCpp_Uno.h"
#include "ErrorMonitor.h"
#include "Comm.h"

///////////////////////////////////////////////////////////////////////////////

void errorCheck(){
  if (digitalRead(ERROR_MONITOR_PIN) == 0 && digitalRead(SIGNAL_ENABLE_PIN_PROG)==HIGH){
    digitalWrite(SIGNAL_ENABLE_PIN_PROG,LOW);                                                     // disable both Motor Shield Channels
    digitalWrite(SIGNAL_ENABLE_PIN_MAIN,LOW);                                                     // regardless of which caused current overload

    for (int i=0; i<3; i++){
      digitalWrite(SIGNAL_ENABLE_PIN_MAIN, HIGH);
      digitalWrite(SIGNAL_ENABLE_PIN_PROG, HIGH);

      delay(1);

      if (digitalRead(ERROR_MONITOR_PIN) == 0){
        digitalWrite(SIGNAL_ENABLE_PIN_MAIN, LOW);
        digitalWrite(SIGNAL_ENABLE_PIN_PROG, LOW);
      }
      else
        break;
    }

    if (digitalRead(ERROR_MONITOR_PIN) == 0){
      INTERFACE.print("<p2>");                                                                            // print corresponding error message
    }
  }
}
