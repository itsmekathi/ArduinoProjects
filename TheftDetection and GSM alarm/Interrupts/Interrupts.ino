#define LED 13
void setup()
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  attachInterrupt(0, door1, RISING);
  attachInterrupt(1, door2, RISING);
  Serial.println("Initialized");
}
   
   void door1()
   {
       noInterrupts();
       Serial.println("Door 1 is hacked"); 
       exit();
   }
 
 void door2()
{
    noInterrupts();    
    Serial.println("Door 2 is hacked"); 
    exit();
 }

void loop()
{
  Serial.println("In Main loop");
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
    
}
  
  void exit()
  {
    Serial.println("In Exit loop");
    for(;;){      }
      Serial.println("Coming out of Exit loop");
  }

