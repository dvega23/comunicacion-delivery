#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
 
//pines CE y el CSN
RF24 radio(9,10);
 
//Variable con la dirección del canal que se va a leer
const byte direccion[5] = "00001";

int led= 3;

void setup()
{
  //inicializamos el puerto serie
  Serial.begin(9600); 
  pinMode(led,OUTPUT);
  digitalWrite(led,LOW);
  //inicializamos el NRF24L01 
  radio.begin();

  
  //Abrimos el canal de Lectura
  radio.openReadingPipe(0, direccion);
  radio.setPALevel(RF24_PA_MIN);
  
  //empezamos a escuchar por el canal
  radio.startListening();

 
}
 
void loop() {
  if (radio.available()){
      //radio.read(&"1", sizeof("1"));
      Serial.println(radio.available());
      digitalWrite(led,HIGH);
      
  }
 
}
