// Final Code for GSM based Home security system


#include <APR33A3.h>
#include <SoftwareSerial.h>

// As interrupt pins are 2 and 3 
SoftwareSerial ss(4, 5);  //// RX, TX


//Playing audio using APR33A3
#define APRChannel0 6
#define APRChannel1 7
#define APRChannel2 8
#define APRChannel3 9
#define BUSY_PIN 10



#define STATUS_LED 13

int buttonState = 0;
char buffer[50];
String number1 = "+918197429387";
String number2 = "+919738460098";
String message1 = "Theif has entered your house.\n Please take immediate action";




static int intStatus;

void setup()
{

  intStatus = 0;

  // Initialize the Status LED
  pinMode(STATUS_LED, OUTPUT);
  digitalWrite(STATUS_LED, LOW);
  
  // Initialize the H/W serial port for debugging purposes
  Serial.begin(9600);
  Serial.flush();

  // Initialize the APR33A3 Pins
  initAPR33a3();
    
  // Start each soft serial port
  ss.begin(9600);
  ss.flush();

  // Set the interrupts for Rising edge
  attachInterrupt(0, door1, RISING);
  attachInterrupt(1, door2, RISING);  

  // Print in serial port for debugging 
  Serial.println("Init");
  Serial.print("Value is " );
  Serial.println(intStatus);
}
   
   void door1()
   {
      intStatus = 1;
   }
 
 void door2()
{
    intStatus = 1;
 }

void loop()
{
    Serial.println("IML");
    Serial.print("Value is " );
     Serial.println(intStatus);
    if(intStatus != 1)
    {
      Serial.println("No Interrupt");
      digitalWrite(STATUS_LED, HIGH);
      delay(500);
      digitalWrite(STATUS_LED, LOW);
      delay(500);
    }
    else {
      Serial.println("Interrupt");
      AlertUser();
      Serial.println("Alerted users");
      exit();
    }

    
}


// Exit loop to exit from the code and loop here forever
  void exit()
  {
    for(;;);
  }


  
void initAPR33a3()
{
  pinMode(APRChannel0,OUTPUT);
  pinMode(APRChannel1,OUTPUT);
  pinMode(APRChannel2,OUTPUT);
  pinMode(APRChannel3,OUTPUT);
  
  
  //Set all the outputs as high
   setHigh();
  pinMode(BUSY_PIN,INPUT);      //To sense the busy signal
  delay(1000);
}

void playAudio(int i)
{
  
  
  switch(i)
  {
    case 0: 
    delay(100);
  digitalWrite(APRChannel0,LOW);
  delay(100);
  setHigh();
  break;
  case 1: 
    delay(100);
  digitalWrite(APRChannel1,LOW);
  delay(100);
  setHigh();
  break;
  case 2: 
    delay(100);
  digitalWrite(APRChannel2,LOW);
  delay(100);

  setHigh();
  break;
  case 3: 
    delay(100);
  digitalWrite(APRChannel3,LOW);
  delay(100);
  setHigh();
  break;
    
  }
  //check whether APR33A3 is busy or not
  delay(100);
  WaitTillBusy();
  
}

int checkBusyAPR33a3()
{
   delay(100);
  buttonState = digitalRead(BUSY_PIN);
  if(buttonState == HIGH)
  {
    return 1;
  }
  else
 {
   return 0;
 }
}

void WaitTillBusy()
{
  Serial.println("PA");
   while ( !checkBusyAPR33a3());
    Serial.println("EA");
}


void setHigh()
{
   //Set all the outputs as high
  digitalWrite(APRChannel0,HIGH);
  digitalWrite(APRChannel1,HIGH);
  digitalWrite(APRChannel2,HIGH);
  digitalWrite(APRChannel3,HIGH);
}

unsigned char  GSMReady()
{
  ss.print("AT");
  ss.print((char)13);
  ss.listen();
  delay(1000);
  int bytes;
  if(ss.available()>0){
    int index =0;
     bytes = ss.available();
    int numChar=ss.available();
    if(numChar>20)
  {
  numChar =20;
}
    while(numChar--){
      buffer[index++] = ss.read();
    }
    
  }
  
  if(bytes == 9 && buffer[5]=='O' && buffer[6]=='K')
  return 0;                  //Return 0 if the GSM Replies with OK command
  else return 1;            //Return 1 if the GSM Replies with invalid data
  
}



unsigned char GSMMakeCall ()
{
  while (GSMReady());                    //Wait till GSM is ready
  Serial.println("MAC");
  ss.print("ATD" + number1 + ";\r");
  ss.listen();
  delay(1000);    //Wait for the modem to respond
  
  if(ss.available() > 0)
  {
    delay(1000);
    int index =0;
    int numChar = ss.available();
    Serial.println(numChar,DEC);
    if(numChar>45)
    numChar = 45;
    while(numChar--){
      buffer[index++] = ss.read();
    }
    Serial.println(buffer); 
  }
  
}


unsigned char GSMSendMessage()
{
  ss.flush();
  Serial.flush();
  while(GSMReady());    //Stay here till GSM becomes ready
   Serial.println("ML");
    //Enters only if GSM is ready
    ss.println("AT+CMGS= \""+ number1+"\"");
    ss.listen();
    //Printing response
    delay(1000);
    if(ss.available()>0){
    int index =0;
    delay(100);  //let the buffer fill up
    int numChar=ss.available();
    Serial.println( "RS");
    Serial.println(numChar,DEC);
    if(numChar>45)
  {
  numChar =45;
}
    while(numChar--){
     buffer[index++] = ss.read();
    }
    Serial.println(buffer);

  }
  
    
    delay(1000); // wait till the modem responds with '>'
    ss.println(message1);
    ss.print((char)26);
    
    ss.listen();
    //Printing response
    delay(1000);  //let the buffer fill up
    if(ss.available()>0){
    int index =0;
    delay(100);  //let the buffer fill up
    int numChar=ss.available();
    Serial.println(numChar,DEC);
    if(numChar>45)
  {
  numChar =45;
}
    while(numChar--){
    buffer[index++] = ss.read();
    }
        Serial.println(buffer);

  }
 
  ss.flush();
  Serial.flush();
  
}


void AlertUser()
{
    if( !  GSMReady()) Serial.println("GSMR");
      else Serial.println("GSMNR");
delay(5000);

//Make a call
GSMMakeCall();
//Wait for 5 seconds
delay(5000);
//Play Audio
playAudio(1);
playAudio(2);
playAudio(3);

//Hangup
delay(2000);
ss.println("ATH ");
delay(5000);
GSMSendMessage();
}

