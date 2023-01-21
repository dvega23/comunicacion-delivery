#include <SPI.h>  
#include <RF24.h>


 
 RF24 radio(9, 10); // CE, CSN
 
 const byte direccion[5] = "00001";

 const int angulo= 1;
void setup() {

  radio.begin();
  Serial.begin(9600);
 
   radio.openWritingPipe(direccion);
   radio.setPALevel(RF24_PA_MIN);
   radio.stopListening();
}
 
void loop() {
  
  radio.write(&"1", sizeof("1"));
  delay(50); 
  
}
