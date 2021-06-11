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
const uint64_t Address[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL, 0xB3B4B5B6A3LL, 0xB3B4B5B60FLL, 0xB3B4B5B605LL };
byte emetteur; //buffer correspondant à l'adresse d'un éméteur

char str[20]; // buffer temporaire pour la récupération de données
const char taille = 4; //taille max des valeurs converties en chaine

// valeurs reçu par le module RF
struct data 
{
    //short id;
    short xAxis;
    short yAxis;
    //short zAxis; // inutilisé
} receive_data;

// valeurs à envoyer à la Uno-Braccio
struct dataToSend
{
  char id[taille];
  char xAxis[taille];
  char yAxis[taille];
  //char zAxis[taille];
} send_data;


// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    Serial.begin(115200);
    Serial.print(__FILE__); Serial.println(" initialising ...");
    
    radio.begin();
    radio.setPALevel(RF24_PA_MAX);
    radio.setChannel(108);
    radio.setDataRate(RF24_2MBPS);
    radio.openReadingPipe(0,Address[0]);
    radio.openReadingPipe(1,Address[1]);
    radio.openReadingPipe(2,Address[2]);
    //radio.openReadingPipe(3,Address[3]);
    //radio.openReadingPipe(4,Address[4]);
    //radio.openReadingPipe(5,Address[5]);
    radio.startListening();
    
    Serial.print(__FILE__); Serial.println(" ready !");
}


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
    while(radio.available(&emetteur)) 
    {
        radio.read(&receive_data, sizeof(data));
        
        str[0] = 0;  // réinitialisation du buffer

        // convertion valeurs (short) vers chaines (char*)
        itoa(emetteur + 1, send_data.id, 10);
        itoa(receive_data.xAxis, send_data.xAxis, 10);
        itoa(receive_data.yAxis, send_data.yAxis, 10);
        //itoa(receive_data.yAxis, send_data.zAxis, 10);

        // création d'une chaine au format '#1;xxx;yyy;zzz#'
        strcat(str, "#");
        strcat(str, send_data.id);
        strcat(str, ";");
        strcat(str, send_data.xAxis);
        strcat(str, ";");
        strcat(str, send_data.yAxis);
        //strcat(str, ";");
        //strcat(str, send_data.zAxis);
        strcat(str, "#");
        //Serial.println(str);
        //emetteurSerial.write(str); 
        Serial.write(str);
        
        delay(10);
    }
}
