#include "settings.h"
#include "protocol.h"

void setup() {
  Serial.begin(2400);

  analogReference(EXTERNAL);

  pinMode(Vin_pin, INPUT);              // Analog pin to measure input voltage (utility)
  pinMode(Vout_pin, INPUT);             // Analog pin to measure output voltage
  pinMode(Vbat_pin, INPUT);             // Analog pin to measure battery voltage 
  pinMode(LBO_pin, INPUT_PULLUP);       // Digital pin to read low battery signal

  pinMode(Vout_Enable_pin, OUTPUT);     // Digital pin to turn on output
  digitalWrite(Vout_Enable_pin, LOW);

  pinMode(LED_BUILTIN, OUTPUT);         // Digital pin to iluminate Pin 13 LED durring UPS restart timeout 

}

void loop() {

  if (millis() > tick1 + 1000){ //every 1 second
    tick1 =millis();     
    UPS_Status();
    UPS_State();   

    timer = timer + 1;

    if(State == Shutting_Down || State == ShutdownCommand){LED = !LED;} else{LED = false;}
    if(State == Starting_Up){LED = true;}
    if(State == RestartCommand && Restart_Delay != 0){LED = true;}
    
    digitalWrite(LED_BUILTIN, LED);
    
  }
  else if(tick1 > millis()){ //handle millis() unsignedlong rollover, about 50 days
    tick1 = 0;
  }
  
  if (stringComplete) {
    UPS_Response();
      // clear the string:
      inputString = "";
      stringComplete = false;
  }

}

void UPS_Status(){

  Vin = analogRead(Vin_pin) * 0.008;   // resistor divider halves voltage so double value for correct reading, 0.008 for 4.096v reference. 
  Vout = analogRead(Vout_pin) * 0.008; // resistor divider halves voltage so double value for correct reading, 0.008 for 4.096v reference. 
  Vbat = analogRead(Vbat_pin) * 0.004; // 0.008 for 4.096v reference.

  LBO = digitalRead(LBO_pin);

  if (Vin < Vbat){
    Vin_Fail = true;
  }
  else{
    Vin_Fail = false;
  }
}

void UPS_State(){

  switch (State){
    case Starting_Up:
      if(Vin_Fail == true){
        State = Shutdown;
      }
      else if (timer > Startup_Delay){ 
        timer = 0;
        Vout_Enable = true;
        State = Running;
      }
      break;
    
    case Running:
      if(Vin_Fail == true && LowBat == true){
        State = Low_Battery;
      }
      timer = 0;
      break;

    case Low_Battery:
      if(DeadBat == true){
        State = Shutdown;
      }
      else if(LowBat == false){
        State = Running;
      }
      timer = 0;            
      break;
      
    case Shutdown:
      if(Vin_Fail == false){
        State = Starting_Up;
      }
      timer = 0;    
      break;
      
    case ShutdownCommand:
      if (timer > Shutdown_Delay){
        timer = 0;
        Vout_Enable = false;
        State = RestartCommand;
      }
      break;
      
    case RestartCommand:
        if (Restart_Delay == 0){
          timer = 0;
        }
        else if (timer > Restart_Delay){
          timer = 0;
          Restart_Delay = 0;
          Vout_Enable = true;
          State = Running;
        }
      break;
  }
  
  digitalWrite(Vout_Enable_pin, Vout_Enable);
}


/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\r') {
      stringComplete = true;
    }
  }
}


