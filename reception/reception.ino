  /**
 * Code permettant de recevoir les données 
 * de l'acceléromètre avant de les transmettre au robot
 */

//#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7,8); //emission avec Arduino Nano + NRF24l01
//RF24 radio(9,10); //emission avec Arduino Nano-rf
const byte address[6] = "00001";

char str[20]; // buffer temporaire pour la récupération de données
const char taille = 4; //taille max des valeurs convertit en chaine

// valeurs reçu par le module RF
struct data 
{
    short id;
    short xAxis;
    short yAxis;
    short zAxis;
} receive_data;

// valeurs à envoyer à la Uno-Braccio
struct dataToSend
{
  char id[taille];
  char xAxis[taille];
  char yAxis[taille];
  char zAxis[taille];
} send_data;


// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    Serial.begin(115200);
    Serial.print(__FILE__); Serial.println(" initialising ...");
    
    radio.begin();
    radio.openReadingPipe(0,address);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);
    radio.startListening();
    //Serial.print(__FILE__); Serial.println(" ready !");
}


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
    while(radio.available()) 
    {
        radio.read(&receive_data, sizeof(data));
        
        str[0] = 0;  // réinitialisation du buffer

        // convertion valeurs (short) vers chaines (char*)
        itoa(receive_data.id, send_data.id, 10);
        itoa(receive_data.xAxis, send_data.xAxis, 10);
        itoa(receive_data.yAxis, send_data.yAxis, 10);
        itoa(receive_data.yAxis, send_data.zAxis, 10);

        // création d'une chaine au format '#1;xxx;yyy;zzz#'
        strcat(str, "#");
        strcat(str, send_data.id);
        strcat(str, ";");
        strcat(str, send_data.xAxis);
        strcat(str, ";");
        strcat(str, send_data.yAxis);
        strcat(str, ";");
        strcat(str, send_data.zAxis);
        strcat(str, "#");
        //Serial.println(str);
        //emetteurSerial.write(str); 
        Serial.write(str);
        
        delay(15);
    }
}
