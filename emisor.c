//Motor Shield
#include <AFMotor.h>
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

//RECEPTOR
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//Uso de RFID
#include <MFRC522.h>      // Libreria  MFRC522
#include <Servo.h>     // Libreria  SERVO

#define RST_PIN  44     // Pin de reset
#define SS_PIN  53      // Pin de slave select SDA

MFRC522 mfrc522(SS_PIN, RST_PIN); // Objeto mfrc522 enviando pines de slave select y reset

byte LecturaUID[4];         // Array para almacenar el UID leido
byte Usuario1[4]= {0x79, 0xA4, 0x5D, 0xB3} ;    // NUMERO DEL USUARIO 1 (ponga el de su tarjeta)
byte Usuario2[4]= {0xF7, 0xEC, 0x6C, 0xB4} ;    // NUMERO DEL USUARIO 2 (ponga el de su tarjeta)
byte Usuario3[4]= {0x3A, 0x92, 0X7E, 0x9C} ;
Servo servoPT;                                 // Asocia la libreria servo a  servoPT

//Declaremos los pines CE y el CSN
#define CE_PIN 48
#define CSN_PIN 49

//Variable con la dirección del canal que se va a leer
byte direccion[5] ={'c','a','n','a','l'}; 

//creamos el objeto radio (NRF24L01)
RF24 radio(CE_PIN, CSN_PIN);

//vector para los datos recibidos
long datos[3];
int velocidad;

void setup() {
   //inicializamos el NRF24L01 
  radio.begin();
  
  //inicializamos el puerto serie
  Serial.begin(115200); 
    
  //Abrimos el canal de Lectura
  radio.openReadingPipe(1, direccion);
  
    //empezamos a escuchar por el canal
  radio.startListening();

  //inicializamos RFID
  SPI.begin();        //Iniciamos el Bus SPI
  servoPT.attach(10);      // Sevo asociado al pin 4 y lleva a 170 grados
  servoPT.write(170);
  mfrc522.PCD_Init();     // inicializa modulo lector
  Serial.println("Control Inicializado ...");    // Muestra texto Listo

}


void loop() {

RFID();

//Comunicacion para RF (radio frecuencia)
   uint8_t numero_canal;
 //if ( radio.available(&numero_canal) )
 //Serial.print(radio.available());
 //Serial.println(radio.available());
 //Serial.println(datos[2]);
 if ( radio.available() )
 {    
     //Leemos los datos y los guardamos en la variable datos[]
     radio.read(&datos,sizeof(datos));
     //radio.read(&datos[2],sizeof(datos[2]));
     //Serial.print(datos[2]);

    //Función para la activación de la puerta por medio del RFID
    RFID();
    velocidad= datos[0];

    if(datos[3]>90){
      forward();
      }
      
    else if(datos[3]<90){
      back();
      }

    else if(datos[3]=90){
      Stop();
      }

    if(datos[4]>89){
      right();
      }
    else if(datos[4]<89){
      left();
      }
    
    //Funcionamiento del vehiculo con dispositivo movil con conexión bluetooth
     switch(datos[2]){
      case 'P':
        Stop();
        break;
      case 'W':
        forward();
        break;
      case 'S':
        back();
        break;
      case 'D':
        right();
        break;
      case 'A':
        left();
        break;
      }

     //reportamos por el puerto serial los datos recibidos
     Serial.print("Dato0= " );
     Serial.print(datos[0]);
     Serial.print(",");
     Serial.print("Dato2= ");
     Serial.print(datos[2]);
     Serial.println();
 }
 //else
 //{
 //    Serial.println("No hay datos de radio disponibles");
 //}
 delay(50);
}

//Funciones para el control de las motores del vehiculo
void forward(){
  motor1.run(FORWARD);
  motor1.setSpeed(255);
  motor2.run(FORWARD);
  motor2.setSpeed(255);
  motor3.run(FORWARD);
  motor3.setSpeed(255);
  motor4.run(FORWARD);
  motor4.setSpeed(255);
  }

void back(){
  motor1.run(BACKWARD);
  motor1.setSpeed(255);
  motor2.run(BACKWARD);
  motor2.setSpeed(255);
  motor3.run(BACKWARD);
  motor3.setSpeed(255);
  motor4.run(BACKWARD);
  motor4.setSpeed(255);  
  }

void left(){
  motor1.run(BACKWARD);
  motor1.setSpeed(255);
  motor2.run(BACKWARD);
  motor2.setSpeed(255);
  motor3.run(FORWARD);
  motor3.setSpeed(255);
  motor4.run(FORWARD);
  motor4.setSpeed(255);
  }

void right(){
  motor1.run(FORWARD);
  motor1.setSpeed(255);
  motor2.run(FORWARD);
  motor2.setSpeed(255);
  motor3.run(BACKWARD);
  motor3.setSpeed(255);
  motor4.run(BACKWARD);
  motor4.setSpeed(255);
  }
  
void Stop(){
  motor1.setSpeed(0);
  motor1.run(RELEASE);
  motor2.setSpeed(0);
  motor2.run(RELEASE);
  motor3.setSpeed(0);
  motor3.run(RELEASE);
  motor4.setSpeed(0);
  motor4.run(RELEASE);
  }

//Conexión RFID
void RFID(){
    if ( ! mfrc522.PICC_IsNewCardPresent())   // si no hay una tarjeta presente
    return;           // retorna al loop esperando por una tarjeta
  
  if ( ! mfrc522.PICC_ReadCardSerial())     // si no puede obtener datos de la tarjeta
    return;           // retorna al loop esperando por otra tarjeta
    
    Serial.print("UID:");       // muestra texto UID:
    for (byte i = 0; i < 4; i++) { // bucle recorre de a un byte por vez el UID
      if (mfrc522.uid.uidByte[i] < 0x10){   // si el byte leido es menor a 0x10
        Serial.print(" 0");       // imprime espacio en blanco y numero cero
        }
        else{           // sino
          Serial.print(" ");        // imprime un espacio en blanco
          }
          Serial.print(mfrc522.uid.uidByte[i], HEX);    // imprime el byte del UID leido en hexadecimal
          LecturaUID[i]=mfrc522.uid.uidByte[i];     // almacena en array el byte del UID leido      
          }
          
          Serial.print("\t");         // imprime un espacio de tabulacion             
                    
          if(comparaUID(LecturaUID, Usuario1)){    // llama a funcion comparaUID con Usuario1
            Serial.println("Retire sus productos"); // si retorna verdadero muestra texto bienvenida
            abrirPuerta();                      // funcion para abrir la puerta
          }
          else if(comparaUID(LecturaUID, Usuario2)){ // llama a funcion comparaUID con Usuario2
            Serial.println("Retire sus productos"); // si retorna verdadero muestra texto bienvenida
             abrirPuerta();                      // funcion para abrir la puerta
          }

          else if(comparaUID(LecturaUID, Usuario3)){ // llama a funcion comparaUID con Usuario2
            Serial.println("Bienvenido Jhonny Vargas"); // si retorna verdadero muestra texto bienvenida
             abrirPuerta();                      // funcion para abrir la puerta
          }
          
           else {          // si retorna falso
            Serial.println("No Registrado");    // muestra texto equivalente a acceso denegado          
             Mal_Registro();                    // funcion sonido de targeta no identificada
           }  
                  mfrc522.PICC_HaltA();     // detiene comunicacion con tarjeta    

 
}
  

//Función para abrir y cerrar puerta del compartimiento del vehiculo
void abrirPuerta() {
  servoPT.write(90);       // Abrir la puerta 
  delay(3000);             // Tiempo de la puerta abierta
  servoPT.write(170);       // Cierra puerta 
  delay(500);
}

void Mal_Registro() {      // Activa el Buzzer 2 veces por tarjeta no autorizada
  digitalWrite(7, HIGH);
  delay(200);
  digitalWrite(7, LOW);
  delay(100);
  digitalWrite(7, HIGH);
  delay(200);
  digitalWrite(7, LOW);
}

boolean comparaUID(byte lectura[],byte usuario[]) // funcion comparaUID
{
  for (byte i=0; i < 4; i++){    // bucle recorre de a un byte por vez el UID
  if(lectura[i] != usuario[i])        // si byte de UID leido es distinto a usuario
    return(false);          // retorna falso
  }
  return(true);           // si los 4 bytes coinciden retorna verdadero
}

