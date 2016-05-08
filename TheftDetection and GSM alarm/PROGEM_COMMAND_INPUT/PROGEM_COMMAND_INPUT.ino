#include <APR33A3.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

//Initialize the APR33A3 connected to Arduino
//        APR33A3(busy,rec,m0,m1,m2,m3)
APR33A3 audio1(4,5,6,7,16,17);
//Number of Rows and columns in KEYPAD
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};


byte rowPins[ROWS]= {8,9,10,11};      //Connect to the row pinouts
byte colPins[COLS] = {12,13,14,15};    //Connect to col pinouts

//Keypad Initialization Routine
Keypad keypad = Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);

//Initialize LCD to pins
LiquidCrystal lcd(19,18,20,21,22,23);

// Number of Rows and Columns in LCD
const int numRows = 2;
const int numCols = 16;

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
#define LCD_Initizlizing 19
#define LCD_Ready 20
#define LCD_INVALID_COMMAND 21
#define LCD_CANCELLED 22
#define LCD_CONFIRMED 23
#define LCD_CONFIRMATION 24


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


//Set up a table to refer your string
//Note : Add in the same order as the #define is used to reference the same
const char* const lcd_DisplayTable[] PROGMEM = {lcdEnterCmd,lcdModifyAudio0,lcdModifyAudio1,lcdModifyAudio2,lcdModifyAudio3,
lcdModifyNumber0,lcdModifyNumber1,lcdModifyNumber2,lcdModifyNumber3,lcdModifyNumber4,lcdModifyNumber5,lcdModifyNumber6,
lcdModifyNumber7,lcdModifyNumber8,lcdModifyNumber9,lcdModifyRTCDay,lcdModifyRTCTime,lcdPressSTARToStart,lcdPressHASHToEnd,
lcdInitializing,lcdReady,lcdInvalidCommand,lcdCancelled,lcdConfirmed,lcdConfirmation};

//Buffer large enough for the largest string to be displayed
char lcdBuffer[16];

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


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                 EEPROM Configuration variables
#define SYSTEM_STATE 0
#define NUMBER_STORE_START_LOCATION 10
#define NUMBER_LENGTH 15


//XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
int k;  //Local Integer counter variable

void setup(){
  lcd.begin(numCols,numRows); 
  
  //Testing Purpose only
//  Serial.begin(9600);


  lcdDisplayPROGMEM(LCD_Initizlizing,0,0);
  delay(500);
  lcdDisplayPROGMEM(LCD_Ready,0,0);
  delay(500);
}

void loop(){
  lcd.clear();
  lcdDisplayPROGMEM(LCD_ENTER_CMD,0,0);
  acceptCommand();
  executeCommand();
  //Delay between Each command entry
  delay(2000);
}


//This function loads the data from the flash into buffer and then displays in the LCD  
// lcdDisplay(byte stringOffset, byte row, byte col)
// String offset is the offset in the arrary, row,col is the position in the LCD
void lcdDisplayPROGMEM(byte cmdOffset, byte row, byte col)
{
 strcpy_P(lcdBuffer,(char*)pgm_read_word(&(lcd_DisplayTable[cmdOffset])));
   lcdClearROW(row);
  //Position the cursor to First ROW
  lcd.setCursor(col,row);
  lcd.print(lcdBuffer);
  
}

//This function clears the ROW based on the ROW variable passed to it
void lcdClearROW(byte ROW)
{
  lcd.setCursor(0,ROW);
  for(byte i=0;i<numCols;i++)
  lcd.print(' ');
}


void acceptCommand()
{
  //Clear the Input command Buffer
  clearInputCommandBuffer();
  //Clear Row1
  lcdClearROW(1);
  //Set cursor to second Row
  lcd.setCursor(0,1);
  char key ;
  for(k=0;k<maxCmdLength;k++)
  {
    key=  keypad.getKey();
     while (key == NO_KEY)
     {
       key = keypad.getKey();
     }
     lcd.print(key);
     //Break from loop when '#' Key is pressed
     if(key=='#')break;
     inputCommandBuffer[k] = key;
  }
}


void executeCommand()
{
//  lcdClearROW(0);
//  lcd.setCursor(0,0);
//  lcd.print(inputCommandBuffer);
//  
//   //Testing Purpose only
//   Serial.println(inputCommandBuffer);


//Create a logic to compare data from EEPROM and place the result
  for ( k = 0; k <MAX_CMD ; k++)
  {
    strcpy_P(cmdRPROGMEMBuffer, (char*)pgm_read_word(&(cmdTable[k])));   // Necessary casts and dereferencing, just copy.
//    Serial.println(cmdRPROGMEMBuffer);
    //Type caste both the Buffers and compare
     if( (String)inputCommandBuffer == (String)cmdRPROGMEMBuffer) break;    
  }
  //Now K will hold the state
  
  switch(k)
  {
    case cmdModifyAudio0:
    //Execute code to modify audio on Channel 0
    lcdDisplayPROGMEM(LCD_MODIFY_AUDIO_0,0,0);
    modifyAudio(0);
    break;
    
    case cmdModifyAudio1:
    //Call/Execute code to modify audio on Channel 1
    lcdDisplayPROGMEM(LCD_MODIFY_AUDIO_1,0,0);
    modifyAudio(1);
    break;
    case cmdModifyAudio2:
    //Execute code to modify audio on Channel 2
    lcdDisplayPROGMEM(LCD_MODIFY_AUDIO_2,0,0);
    modifyAudio(2);
    break;
    
    case cmdModifyAudio3:
    //Call/Execute code to modify audio on Channel 3
    lcdDisplayPROGMEM(LCD_MODIFY_AUDIO_3,0,0);
    modifyAudio(3);
    break;
    
    case cmdModifyNumber0:
    //Call/Execute code to modify Number 0
    lcdDisplayPROGMEM(LCD_MODIFY_NUMBER_0,0,0);
    modifyPhoneNumber(0);
    
    break;
    case cmdModifyNumber1:
    //Call/Execute code to modify Number 1
    lcdDisplayPROGMEM(LCD_MODIFY_NUMBER_1,0,0);
    modifyPhoneNumber(1);
    break;
    
    case cmdModifyNumber2:
    //Call/Execute code to modify Number 2
    lcdDisplayPROGMEM(LCD_MODIFY_NUMBER_2,0,0);
    modifyPhoneNumber(2);
    break;
    
    case cmdModifyNumber3:
    //Call/Execute code to modify Number 3
    lcdDisplayPROGMEM(LCD_MODIFY_NUMBER_3,0,0);
    modifyPhoneNumber(3);
    break;
    
    case cmdModifyNumber4:
    //Call/Execute code to modify Number 4
    lcdDisplayPROGMEM(LCD_MODIFY_NUMBER_4,0,0);
    modifyPhoneNumber(4);
    break;
    
    case cmdModifyNumber5:
    //Call/Execute code to modify Number 5
    lcdDisplayPROGMEM(LCD_MODIFY_NUMBER_5,0,0);
    modifyPhoneNumber(5);
    break;
    
    case cmdModifyNumber6:
    //Call/Execute code to modify Number 6
    lcdDisplayPROGMEM(LCD_MODIFY_NUMBER_6,0,0);
    modifyPhoneNumber(6);
    break;
    
    case cmdModifyNumber7:
    //Call/Execute code to modify Number 7
    lcdDisplayPROGMEM(LCD_MODIFY_NUMBER_7,0,0);
    modifyPhoneNumber(7);
    break;
    
    case cmdModifyNumber8:
    //Call/Execute code to modify Number 8
    lcdDisplayPROGMEM(LCD_MODIFY_NUMBER_8,0,0);
    modifyPhoneNumber(8);
    break;
    
    case cmdModifyNumber9:
    //Call/Execute code to modify Number 9
    lcdDisplayPROGMEM(LCD_MODIFY_NUMBER_9,0,0);
    modifyPhoneNumber(9);
    break;
    
    case cmdModifyRTCDay:
    //Call/Execute code to modify RTC Day
    lcdDisplayPROGMEM(LCD_MODIFY_RTC_DATE,0,0);
    break;
    
    case cmdModifyRTCTime:
    //Call/Execute code to modify RTC Time
    lcdDisplayPROGMEM(LCD_MODIFY_RTC_TIME,0,0);
    break;
    
    
    default:
    lcdDisplayPROGMEM(LCD_INVALID_COMMAND,0,0);
  }

}

//Clears the Input Command Buffer
void clearInputCommandBuffer()
{
  for (k = 0; k<maxCmdLength;k++)
  {
    inputCommandBuffer[k] =0;
  }
}

//Write the logic to accept input from user
void modifyPhoneNumber(byte numOffset)
{
 
  //Clear Row1
  lcdClearROW(1);
  //Set cursor to second Row
  lcd.setCursor(0,1);
  loadPhoneNumberFromEEPROM(numOffset);
  lcd.print(inputCommandBuffer);
  lcd.setCursor(0,1);
  
  
  //Clear the Input command Buffer
  clearInputCommandBuffer();
  char key ;
  for(k=0;k<maxCmdLength;k++)
  {
    key=  keypad.getKey();
     while (key == NO_KEY)
     {
       key = keypad.getKey();
     }
     lcd.print(key);
     //Break from loop when '#' Key is pressed
     if(key=='#')break;
     inputCommandBuffer[k] = key;
  }
  lcdDisplayPROGMEM(LCD_CONFIRMATION,0,0);
  if(getControlKey() =='*') 
  {
    storePhoneNumberInEEPROM(numOffset);
   lcdDisplayPROGMEM(LCD_CONFIRMED,1,0);
  }
  else 
  {
    lcdDisplayPROGMEM(LCD_CANCELLED,1,0);
  }
  }

//Gets the confirmation keys from Keypad
char getControlKey()
{
  char key ;
  key = keypad.getKey();
  while (key == NO_KEY)
     {
       key = keypad.getKey();
     }
//     Serial.println(key);
   return key;
}



void modifyAudio(byte numChannel)
{
  //Write the logic to accept key and audio input from user and 
    audio1.playAudioTillBusy(numChannel);
    lcdDisplayPROGMEM(LCD_STAR_TO_START,0,0);
    lcdDisplayPROGMEM(LCD_PRESS_HASH_TO_END,1,0);
    lcd.setCursor(0,1);
    char ch= getControlKey();
    if(ch == '*')
    {
      lcdClearROW(1);
      lcd.setCursor(0,1);
      lcd.print(ch);
      audio1.recordAudio(numChannel);
      lcdDisplayPROGMEM(LCD_PRESS_HASH_TO_END,0,0);
      ch =getControlKey();
      lcdClearROW(1);
      lcd.setCursor(0,1);
      lcd.print(ch);
      audio1.stopRecording();
      lcdDisplayPROGMEM(LCD_CONFIRMED,1,0);
    }
    else
    {
      lcdClearROW(1);
      lcd.setCursor(0,1);
      lcd.print(ch);
      lcdDisplayPROGMEM(LCD_CANCELLED,1,0);
    }
}
void modifyRTCDate()
{
}

void modifyRTCTime()
{
  
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

