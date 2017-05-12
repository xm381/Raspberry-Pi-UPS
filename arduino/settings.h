/****************************************************************
settings.h for use with Raspi_UPS_2_Base.ino

Megatec UPS emulation for use with NUT UPS software on Raspberry Pi

Mike Waldron
May 12, 2017

https://github.com/xm381/Raspberry-Pi-UPS

This code is beerware; if you see me at the local, and you've found our code helpful, please
buy me a round!

Distributed as-is; no warranty is given.
****************************************************************/

/********************* These Values are Static *************************/

const byte Vin_pin = A7;
const byte Vout_pin = A0;
const byte Vbat_pin = A6;
const byte LBO_pin = A2;
const byte Vout_Enable_pin = A3;

float Vin;
float Vout;
float Vbat;
float Bsoc;

bool LBO; //hardware low battery (has no hysteresis): low = low battery.

bool LowBat = false; //software low battery
bool DeadBat = false;
bool Vin_Fail = false;

const int Starting_Up = 0;
const int Running = 1;
const int Low_Battery = 2;
const int Shutting_Down = 3;
const int Shutdown = 4;
const int ShutdownCommand = 5;
const int RestartCommand = 6;

static int State = Starting_Up;

bool Vout_Enable = false;
bool Beeper_Enable = true;
bool LED = false;

bool Command_Shutdown = false;
bool Command_Restart = false;

int timer = 0;

unsigned long tick1;

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

//UPS Rating Information
const float UPS_RV = 5.2; //Rated Voltage
const float UPS_RC = 1.3; //Rated Current
const float UPS_BV = 3.7; //Rated Voltage
const float UPS_HZ = 0.0; //Rated Frequency

// Hysteresis for software lowbat
const float LowBat_Max = 3.60;
const float LowBat_Min = 3.55;
const float Dead_Bat = 3.50;

int Startup_Delay = 10;
int Shutdown_Delay = 60;
int Restart_Delay = 0;
/********************* Change these values as needed ***********************/

const String Company_Name ="Raspberry Pi   "; //15 characters, leave space if less than 15 characters
const String UPS_Model ="Pi UPS    "; //10 characters, leave space if less than 10 characters
const String Version ="v1,0      "; //10 characters, leave space if less than 10 characters
   




