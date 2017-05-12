/*

Megatec Protocol information
http://networkupstools.org/protocols/megatec.html

Command summary
  D Status Inquiry *disable
  Q1 Status Inquiry
  T 10 Seconds Test
  TL Test until Battery Low
  T<n> Test for Specified Time Period
  Q Turn On/Off beep
  S<n> Shut Down Command - Accepted values for n: .2 -> .9 , 01 -> 10 minutes
  S<n>R<m> Shut Down and Restore Command - Accepted values for m: 0001 -> 9999 minutes 
  C Cancel Shut Down Command
  CT Cancel Test Command
  I UPS Information Command
  F UPS Rating Information


Computer: Q1<cr>
UPS: (MMM.M NNN.N PPP.P QQQ RR.R SS.S TT.T b7b6b5b4b3b2b1b0<cr>

  MMM.M Input voltage
  NNN.N Input fault voltage
  PPP.P Output voltage
  QQQ   Output current %
  RR.R  Input frequency
  SS.S  Battery voltage
  TT.T  Temperature centigrade
  UPS Status Byte
    Bit# Description
      7 Utility Fail (Immediate)
      6 Battery Low
      5 Bypass/Boost or Buck Active
      4 UPS Failed
      3 UPS Type is Standby (0 is On_line)
      2 Test in Progress
      1 Shutdown Active
      0 Beeper On

Computer: T<cr>
UPS: Test for 10 seconds and return to utility.

Computer: TL<cr>
UPS: Test until battery low and return to utility.

Computer: T<n><cr>
UPS: Test for <n> minutes.

Computer: Q<cr>
UPS:Turn On/Off beep - Toggle the UPS beeper.

Computer: S<n><cr>
UPS: Shut UPS output off in <n> minutes.

Computer: S<n>R<m><cr>
UPS: Shut UPS output off in <n> minutes, and waiting for <m> minutes then turn on UPS output again.

Computer: C<cr>
UPS: Cancel the SN<n><cr> and SN<n>R<m><cr> command.

Computer: CT<cr>
UPS: Cancel all test activity and connect the utility to output immediately.

Computer: I<cr>
UPS: #Company_Name UPS_Model Version<cr>
  Company_Name 15 characters, leave space if less than 15 characters
  UPS_Model 10 characters, leave space if less than 10 characters
  Version 10 characters, leave space if less than 10 characters

Computer: F<cr>
UPS: #MMM.M QQQ SS.SS RR.R<cr>
  UPS Rating Information
    MMM.M   Rating Voltage
    QQQ     Rating Current
    SS.SS   Battery Voltage
    RR.R    Frequency

Invalid Command/Information Handling
  If the UPS receives any command that it could not handle, the UPS should echo the received command back to the computer. 
  The host should check if the command send to UPS been echo or not.

  If there is any information field in the UPS’s response which is unavailable or not supported, the UPS should fill the field with @.

*/  

void UPS_Response(){

//  Q1 Status Inquiry (MMM.M NNN.N PPP.P QQQ RR.R SS.S TT.T b7b6b5b4b3b2b1b0<cr>
  if (inputString == "Q1\r") { 

//    Serial.print("(215.0 195.0 230.0 014 49.0 2.27 30.0 00101000\r");
      

      Serial.print("(00");
      Serial.print(Vin, 1);                   // MMM.M Input voltage
      
      Serial.print(" 00");
      Serial.print(Vin, 1);                  // NNN.N Input fault voltage
      
      Serial.print(" 00");
      Serial.print(Vout, 1);                  // PPP.P Output voltage
      
      Serial.print(" ");
      Serial.print("050");                    // QQQ   Output current % 
      
      Serial.print(" ");
      Serial.print("00.0");                   // RR.R  Input frequency
      
      Serial.print(" ");
      Serial.print(Vbat, 2);                  // S.SS  Battery voltage
      
      Serial.print(" ");
      Serial.print("21.1");                   // TT.T  Temperature centigrade
      
      Serial.print(" ");
        
//   UPS Status Byte
     Serial.print(Vin_Fail);          // 7 Utility Fail (Immediate)
     Serial.print(LowBat);            // 6 Battery Low
     Serial.print(Vin_Fail);          // 5 Bypass/Boost or Buck Active
     Serial.print("0");               // 4 UPS Failed
     Serial.print("1");               // 3 UPS Type is Standby (0 is On_line)
     Serial.print("0");               // 2 Test in Progress
     
     if(State == Shutting_Down || State == ShutdownCommand){Serial.print("1");}  // 1 Shutdown Active
     else{Serial.print("0");}

     Serial.print(Beeper_Enable);     // 0 Beeper On
     Serial.print("\r");    

  }

//  S<n> Shut Down Command
  else if (inputString.startsWith("S") && inputString.indexOf("R") < 0) {
    String SD_delay = inputString.substring(1);
    
    Shutdown_Delay = SD_delay.toInt() * 60;
    Shutdown_Delay = constrain(Shutdown_Delay, 60, 600); //ensuring value is at least 1 minute   

    State = ShutdownCommand;
  }

//  S<n>R<m> Shut Down and Restore Command 
  else if (inputString.startsWith("S") && inputString.indexOf("R") >= 0) {
    int R_index = inputString.indexOf("R");

    String SD_delay = inputString.substring(1, R_index);
    Shutdown_Delay = SD_delay.toInt() * 60;
    Shutdown_Delay = constrain(Shutdown_Delay, 60,600); //ensuring value is at least 1 minute   

    String RS_delay = inputString.substring(R_index + 1);
    Restart_Delay = RS_delay.toInt() * 60;

    if(Restart_Delay != 0){
      Restart_Delay = constrain(Restart_Delay, 60, 36000); // 1 minute to 10 hours (600 minutes)
    }    
    State = ShutdownCommand;
  }

//  C Cancel Shut Down Command
  else if (inputString.startsWith("C")) {
    State = Running;
  }
  
//  I UPS Information Command  #Company_Name UPS_Model Version<cr>
  else if (inputString == "I\r") {
      Serial.print("#" + Company_Name +" "+ UPS_Model +" "+ Version);
      Serial.print("\r"); 
  }

//  F UPS Rating Information #MMM.M QQQ SS.SS RR.R<cr>
  else if (inputString == "F\r") {
      Serial.print("#");
      Serial.print(UPS_RV);
      Serial.print(" ");
      Serial.print(UPS_RC);
      Serial.print(" ");
      Serial.print(UPS_BV);
      Serial.print(" ");
      Serial.print(UPS_HZ);
      Serial.print("\r"); 
  }


//  Q Turn On/Off beep
  else if (inputString == "Q\r") {
      Beeper_Enable = !Beeper_Enable;
  }

//  ***** Unsupported Commands ***** 
//  D Status Inquiry *disable
  else if (inputString == "D\r") {
      Serial.print("@\r");
  }
//  T 10 Seconds Test
//  TL Test until Battery Low
//  T<n> Test for Specified Time Period
  else if (inputString.startsWith("T")) {
      Serial.println("@\r");
  }

//  CT Cancel Test Command
  else if (inputString == "CT\r") {
      Serial.println("@\r");
  }

//  ***** Custom Commands ***** 
  else if (inputString == "MS\r") {
    Serial.print("State:    "); Serial.println(State);
    Serial.print("Startup:  "); Serial.println(Startup_Delay);
    Serial.print("Shutdown: "); Serial.println(Shutdown_Delay);
    Serial.print("Restart:  "); Serial.println(Restart_Delay);
  }
  
// Invalid command 
  else{    
      Serial.print(inputString);
      Serial.print("\r"); 
  }
  
}


