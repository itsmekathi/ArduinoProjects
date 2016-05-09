#include <APR33A3.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>

//Initialize the APR33A3 connected to Arduino
//        APR33A3(busy,rec,m0,m1,m2,m3)
APR33A3 audio1(4,5,6,7,8,9);

// As interrupt pins are 2 and 3 
SoftwareSerial ss(4, 5);  // RX, TX


//************************************************************************************************************************
// LCD Display Strings stored in PROGRAM Memory
#define LCD_ENTER_CMD 0
#define LCD_MODIFY_AUDIO_0 1
#define LCD_MODIFY_AUDIO_1 2
#define LCD_MODIFY_AUDIO_2 3
#define LCD_MODIFY_AUDIO_3 4
#define LCD_MODIFY_NUMBER_0 5
#define LCD_MODIFY_NUMBER_1 6
#define LCD_MODIFY_NUMBER_2 7
#define LCD_MODIFY_NUMBER_3 8
#define LCD_MODIFY_NUMBER_4 9
#define LCD_MODIFY_NUMBER_5 10
#define LCD_MODIFY_NUMBER_6 11
#define LCD_MODIFY_NUMBER_7 12
#define LCD_MODIFY_NUMBER_8 13
#define LCD_MODIFY_NUMBER_9 14
#define LCD_MODIFY_RTC_DATE 15
#define LCD_MODIFY_RTC_TIME 16
#define LCD_STAR_TO_START 17
#define LCD_PRESS_HASH_TO_END 18
#define LCD_INITIALIZING 19
#define LCD_Ready 20
#define LCD_INVALID_COMMAND 21
#define LCD_CANCELLED 22
#define LCD_CONFIRMED 23
#define LCD_CONFIRMATION 24
#define LCD_ENTERNUMBER 25


//Store LCD Strings that are supposed to be displayed in PROGRAM Memory so as to save RAM Space
const char lcdEnterCmd[] PROGMEM = {"Enter Command"};
const char lcdModifyAudio0[] PROGMEM = {"Modify Audio 0"};
const char lcdModifyAudio1[] PROGMEM = {"Modify Audio 1"};
const char lcdModifyAudio2[] PROGMEM = {"Modify Audio 2"};
const char lcdModifyAudio3[] PROGMEM = {"Modify Audio 3"};
const char lcdModifyNumber0[] PROGMEM = {"Modify Number 0"};
const char lcdModifyNumber1[] PROGMEM = {"Modify Number 1"};
const char lcdModifyNumber2[] PROGMEM = {"Modify Number 2"};
const char lcdModifyNumber3[] PROGMEM = {"Modify Number 3"};
const char lcdModifyNumber4[] PROGMEM = {"Modify Number 4"};
const char lcdModifyNumber5[] PROGMEM = {"Modify Number 5"};
const char lcdModifyNumber6[] PROGMEM = {"Modify Number 6"};
const char lcdModifyNumber7[] PROGMEM = {"Modify Number 7"};
const char lcdModifyNumber8[] PROGMEM = {"Modify Number 8"};
const char lcdModifyNumber9[] PROGMEM = {"Modify Number 9"};
const char lcdModifyRTCDay[] PROGMEM = {"Modify RTC Date"};
const char lcdModifyRTCTime[] PROGMEM = {"Modify RTC Time"};
const char lcdPressSTARToStart[] PROGMEM = {"Press * to START"};
const char lcdPressHASHToEnd[] PROGMEM = {"Press # to END"};
const char lcdInitializing[] PROGMEM = {"Initializing"};
const char lcdReady[] PROGMEM ={"Ready"};
const char lcdInvalidCommand[] PROGMEM ={"Invalid Command"};
const char lcdCancelled[] PROGMEM = {"Cancelled.."};
const char lcdConfirmed[] PROGMEM = {"Confirmed.."};
const char lcdConfirmation[] PROGMEM = {"* for Y & #for N"};
const char lcdEnterNumber[] PROGMEM = {"Enter phone NO:"};


//Set up a table to refer your string
//Note : Add in the same order as the #define is used to reference the same
const char* const lcd_DisplayTable[] PROGMEM = {lcdEnterCmd,lcdModifyAudio0,lcdModifyAudio1,lcdModifyAudio2,lcdModifyAudio3,
lcdModifyNumber0,lcdModifyNumber1,lcdModifyNumber2,lcdModifyNumber3,lcdModifyNumber4,lcdModifyNumber5,lcdModifyNumber6,
lcdModifyNumber7,lcdModifyNumber8,lcdModifyNumber9,lcdModifyRTCDay,lcdModifyRTCTime,lcdPressSTARToStart,lcdPressHASHToEnd,
lcdInitializing,lcdReady,lcdInvalidCommand,lcdCancelled,lcdConfirmed,lcdConfirmation, lcdEnterNumber};

//Buffer large enough for the largest string to be displayed
char serialDisplayBuffer[16];

//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Store Commands in PROGMEM

#define MAX_CMD 16  // considering commands from 0 to 5

#define cmdModifyAudio0 0
#define cmdModifyAudio1 1
#define cmdModifyAudio2 2
#define cmdModifyAudio3 3
#define cmdModifyNumber0 4
#define cmdModifyNumber1 5
#define cmdModifyNumber2 6
#define cmdModifyNumber3 7
#define cmdModifyNumber4 8
#define cmdModifyNumber5 9
#define cmdModifyNumber6 10
#define cmdModifyNumber7 11
#define cmdModifyNumber8 12
#define cmdModifyNumber9 13
#define cmdModifyRTCDay 14
#define cmdModifyRTCTime 15

const char commanddModifyAudio0[] PROGMEM = {"A00"};
const char commanddModifyAudio1[] PROGMEM = {"A01"};
const char commanddModifyAudio2[] PROGMEM = {"A02"};
const char commanddModifyAudio3[] PROGMEM = {"A03"};
const char commanddModifyNumber0[] PROGMEM = {"B00"};
const char commanddModifyNumber1[] PROGMEM = {"B01"};
const char commanddModifyNumber2[] PROGMEM = {"B02"};
const char commanddModifyNumber3[] PROGMEM = {"B03"};
const char commanddModifyNumber4[] PROGMEM = {"B04"};
const char commanddModifyNumber5[] PROGMEM = {"B05"};
const char commanddModifyNumber6[] PROGMEM = {"B06"};
const char commanddModifyNumber7[] PROGMEM = {"B07"};
const char commanddModifyNumber8[] PROGMEM = {"B08"};
const char commanddModifyNumber9[] PROGMEM = {"B09"};
const char commanddModifyRTCDay[] PROGMEM = {"C00"};
const char commanddModifyRTCTime[] PROGMEM = {"C01"};

const char* const cmdTable[] PROGMEM = {commanddModifyAudio0,commanddModifyAudio1,commanddModifyAudio2,commanddModifyAudio3,commanddModifyNumber0,commanddModifyNumber1,
commanddModifyNumber2,commanddModifyNumber3,commanddModifyNumber4,commanddModifyNumber5,commanddModifyNumber6,commanddModifyNumber7,commanddModifyNumber8,commanddModifyNumber9,
commanddModifyRTCDay,commanddModifyRTCTime};


//Make the input command lenght to 15 so as to accept command plus phone numbers
const byte maxCmdLength = 15;
char cmdRPROGMEMBuffer[maxCmdLength+1] ={0,0,0,0,0,0,0,0,0,0,0};
char inputCommandBuffer[maxCmdLength+1] ={0,0,0,0,0,0,0,0,0,0,0};
int k=0;


#define SYSTEM_STATE 0
#define NUMBER_STORE_START_LOCATION 10
#define NUMBER_LENGTH 15



#define SYSTEM_STATE 0
#define NUMBER_STORE_START_LOCATION 10
#define NUMBER_LENGTH 15

#define OUT0 11
#define OUT1 12

#define GMSREADYRETRY 3

#define STATUS_LED 13

int buttonState = 0;
char buffer[50];
String number1 = "+918197429387";
String number2 = "+919738460098";
String message1 = "Theif has entered your house.\n Please take immediate action";

static int intInterruptStatus;


void setup() {
  
  intStatus = 0;

  // Initialize the Status LED
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);

  // Initialize the out pins
  pinMode(OUT0, OUTPUT);
  digitalWrite(OUT0, LOW);
  pinMode(OUT1, OUTPUT);
  digitalWrite(OUT1, LOW);

  // Set the interrupts for Rising edge
  attachInterrupt(0, door1, RISING);
  attachInterrupt(1, door2, RISING);  

  // Initialize the hardware serial port
  Serial.begin(9600);
  Serial.flush();

  // Start each soft serial port
  ss.begin(9600);
  ss.flush();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.flush();
  SerialPrintFromPROGMEM(LCD_ENTER_CMD);
  acceptAndExecuteCommand();
  //Delay between Each command entry
  delay(1000);
}


/* Custom function declarations
 *  
 *  */

void acceptAndExecuteCommand()
{
    Serial.flush();
  clearCommandBuffer();
  // Stay here untill data is availabel in serial port
  while(!Serial.available());
  delay(100); // let the serialbuffer fill up
  clearInputCommandBuffer();
  char ch;
   for(k=0;k<maxCmdLength;k++)
  {
    if(Serial.available())
     {
       ch = Serial.read();
       if(ch == '*')break;
       inputCommandBuffer[k] = ch;  
     }
     else break;
     
  }
//  String cmd = Serial.readString();
  

//Create a logic to compare data from EEPROM and place the result
  for ( k = 0; k <MAX_CMD ; k++)
  {
    strcpy_P(cmdRPROGMEMBuffer, (char*)pgm_read_word(&(cmdTable[k])));   // Necessary casts and dereferencing, just copy.
    // if( cmd == (String)cmdRPROGMEMBuffer) break;    
//    if(strcmp(inputCommandBuffer,cmdRPROGMEMBuffer)==0) break;
    if( (inputCommandBuffer[0] == cmdRPROGMEMBuffer[0]) & (inputCommandBuffer[1] == cmdRPROGMEMBuffer[1]) & (inputCommandBuffer[2] == cmdRPROGMEMBuffer[2])) break;
  }

  
  //Now K will hold the state
  
  switch(k)
  {
    case cmdModifyAudio0:
    //Execute code to modify audio on Channel 0
    SerialPrintFromPROGMEM(LCD_MODIFY_AUDIO_0);
    modifyAudio(0);
    break;
    
    case cmdModifyAudio1:
    SerialPrintFromPROGMEM(LCD_MODIFY_AUDIO_1);
    modifyAudio(1);
    break;
    
    case cmdModifyAudio2:
    SerialPrintFromPROGMEM(LCD_MODIFY_AUDIO_2);
    modifyAudio(2);
    break;
    
    case cmdModifyAudio3:
    SerialPrintFromPROGMEM(LCD_MODIFY_AUDIO_3);
    modifyAudio(3);
    break;
    
    case cmdModifyNumber0:
    //Call/Execute code to modify Number 0
    SerialPrintFromPROGMEM(LCD_MODIFY_NUMBER_0);
    modifyPhoneNumber(0);
    
    break;
    case cmdModifyNumber1:
    //Call/Execute code to modify Number 1
    SerialPrintFromPROGMEM(LCD_MODIFY_NUMBER_1);
    modifyPhoneNumber(1);
    break;
    
    case cmdModifyNumber2:
    SerialPrintFromPROGMEM(LCD_MODIFY_NUMBER_2);
    modifyPhoneNumber(2);
    break;
    
    case cmdModifyNumber3:
    SerialPrintFromPROGMEM(LCD_MODIFY_NUMBER_3);
    modifyPhoneNumber(3);
    break;
    
    case cmdModifyNumber4:
    SerialPrintFromPROGMEM(LCD_MODIFY_NUMBER_4);
    modifyPhoneNumber(4);
    break;
    
    case cmdModifyNumber5:
    SerialPrintFromPROGMEM(LCD_MODIFY_NUMBER_5);
    modifyPhoneNumber(5);
    break;
    
    case cmdModifyNumber6:
    SerialPrintFromPROGMEM(LCD_MODIFY_NUMBER_6);
    modifyPhoneNumber(6);
    break;
    
    case cmdModifyNumber7:
    SerialPrintFromPROGMEM(LCD_MODIFY_NUMBER_7);
    modifyPhoneNumber(7);
    break;
    
    case cmdModifyNumber8:
    SerialPrintFromPROGMEM(LCD_MODIFY_NUMBER_8);
    modifyPhoneNumber(8);
    break;
    
    case cmdModifyNumber9:
    SerialPrintFromPROGMEM(LCD_MODIFY_NUMBER_9);
    modifyPhoneNumber(9);
    break;    
    
    default:
    SerialPrintFromPROGMEM(LCD_INVALID_COMMAND);
  }
}
 void SerialPrintFromPROGMEM(byte cmdOffset)
{
  strcpy_P(serialDisplayBuffer,(char*)pgm_read_word(&(lcd_DisplayTable[cmdOffset])));
  Serial.println(serialDisplayBuffer);  
}



void modifyAudio(byte numChannel)
{
  //Write the logic to accept key and audio input from user and 
  audio1.playAudioTillBusy(numChannel);
  SerialPrintFromPROGMEM(LCD_STAR_TO_START);
  SerialPrintFromPROGMEM(LCD_PRESS_HASH_TO_END);
  char ch= getControlKey();
  if(ch == '*')
  {
    audio1.recordAudio(numChannel);
    SerialPrintFromPROGMEM(LCD_PRESS_HASH_TO_END);
    
    // Stay here till the user sends # on serial line
    Serial.flush();
    while(getControlKey() != '#');
    audio1.stopRecording();
    SerialPrintFromPROGMEM(LCD_CONFIRMED);
  }    
  else
  {
    SerialPrintFromPROGMEM(LCD_CANCELLED);
  }
}



//Write the logic to accept input from user
void modifyPhoneNumber(byte numOffset)
{
  loadPhoneNumberFromEEPROM(numOffset);
  Serial.println(inputCommandBuffer);
  SerialPrintFromPROGMEM(LCD_ENTERNUMBER);
  //Clear the Input command Buffer
  clearInputCommandBuffer();
  char key ;

// Read the input from Serial port
  Serial.flush();
  // Wait till data is present in serial port
  while(!Serial.available());
  delay(100);
  for(k=0;k<maxCmdLength;k++)
  {
    if(Serial.available()){
      key = Serial.read();
      if(key=='#')break;
      inputCommandBuffer[k] = key;
    }
    else break;
  }

  Serial.println(inputCommandBuffer);
    
  SerialPrintFromPROGMEM(LCD_CONFIRMATION);
  if(getControlKey() =='*') 
  {
    storePhoneNumberInEEPROM(numOffset);
   SerialPrintFromPROGMEM(LCD_CONFIRMED);
  }
  else 
  {
    SerialPrintFromPROGMEM(LCD_CANCELLED);
  }
  loadPhoneNumberFromEEPROM(numOffset);
  Serial.println(inputCommandBuffer);
 }
  

//Gets the confirmation keys from Keypad
char getControlKey()
{
  char key ;
  //Stay here untill data is available on serial port
  while(!Serial.available());
  delay(10);  // let the serial buffer fill up
  key = Serial.read();
  Serial.println(key);
   return key;
}

void storePhoneNumberInEEPROM(byte offset)
{
  for (k=0;k<NUMBER_LENGTH;k++)
  {
    EEPROM.write(NUMBER_STORE_START_LOCATION+(offset*NUMBER_LENGTH)+k,inputCommandBuffer[k]);
  }
}

void loadPhoneNumberFromEEPROM(byte offset)
{
  clearInputCommandBuffer();
  for(k=0;k<NUMBER_LENGTH;k++)
  {
    inputCommandBuffer[k] = (char)EEPROM.read(NUMBER_STORE_START_LOCATION+(offset*NUMBER_LENGTH)+k);
  }
}

//Clears the Input Command Buffer
void clearInputCommandBuffer()
{
  for (k = 0; k<maxCmdLength;k++)
  {
    inputCommandBuffer[k] ='\0';
  }
}

//Clears the Input Command Buffer
void clearCommandBuffer()
{
  for (k = 0; k<maxCmdLength;k++)
  {
    cmdRPROGMEMBuffer[k] ='\0';
  }
}






