/* Demo will blink the LED connected to digital pin 13 if the interrrupt on INT0 is triggered
 *  and will stop the LED blink if the interrupt on INT1 is triggered.  
 */
#include <SIM900.h>

#define STATUS_LED 13
volatile int intVal = 0;

SIM900 cell(4,5);

void setVal()
{
  intVal = 1;
}
void resetVal()
{
  intVal= 0;
}

void setup() {
    Serial.begin(9600);
  Serial.println("System ready");
  // put your setup code here, to run once:
  cell.begin(9600);
  cell.Verbose(true);
  cell.Boot();
  // Make the interrupt pins as input and enable the pullup resistors
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  digitalWrite(2,HIGH);
  digitalWrite(3,HIGH);

  attachInterrupt(0, setVal, FALLING);
  attachInterrupt(1, resetVal, FALLING);
  pinMode(STATUS_LED,OUTPUT);
  digitalWrite(STATUS_LED,HIGH);  // Output HIGH to indicate system is ready

  // For one LOW output
  pinMode(6,OUTPUT);
  digitalWrite(6,LOW);

}

void loop() {
  Serial.print("value is " );
  Serial.println(intVal);
  // put your main code here, to run repeatedly:
  if(intVal == 1)
  {
    // Blink the status LED
    digitalWrite(STATUS_LED,HIGH);
    delay(1000);
    digitalWrite(STATUS_LED,LOW);
    delay(1000);  
    Serial.println("Making the call");
    cell.Call("+919738460098");
    delay(15000);
    Serial.println("Hanging up the call");
    cell.HangUp();
    delay(2000);
  }
  else
  {
    // No action to perform
    delay(1000);
  }
}
