// Install pin change interrupt for a pin, can be called multiple times

volatile int intStatus = 0;
void pciSetup(byte pin)
{
  *digitalPinToPCMSK(pin) |= bit(digitalPinToPCMSKbit(pin));  // enable pin
  PCIFR |= bit(digitalPinToPCICRbit(pin));                    // Clear any outstanding interrupt
  PCICR |= bit(digitalPinToPCICRbit(pin));                    // Enable interrupt of the group
}

ISR(PCINT2_vect)  // handle pin change interrupt for D0 to D7 here
{
  intStatus = 1;
}
void setup() {
  // put your setup code here, to run once:
  digitalWrite(2,HIGH);    // set pullups
  digitalWrite(3,HIGH);
  
  // Debug LED
  pinMode(13,OUTPUT);


  pciSetup(2);
  pciSetup(3);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(intStatus == 1)
  {
    digitalWrite(13,HIGH);
    delay(1000);
    digitalWrite(13,LOW);
    delay(1000);
  }
  else
  {
    
  }

}
