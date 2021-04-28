/**
 * Code permettant d'envoyer les données 
 * de l'acceléromètre au robot
 */

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

const int x_out = A0;
const int y_out = A1;

RF24 radio(8,10);
const byte address[6] = "00001";

struct dataToSend
{
  int xAxis;
  int yAxis;
};
dataToSend send_data;


// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.setDataRate(RF24_250KBPS);
    radio.stopListening();
}


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
    send_data.xAxis = analogRead(x_out);
    send_data.yAxis = analogRead(y_out);
    radio.write(&send_data, sizeof(dataToSend));
}
