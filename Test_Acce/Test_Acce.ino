//Arduino Gesture Control Robot
//Reciver Circuit 
//Created BY DIY Builder
//You need to install the required libraries before uploading the code.
//To install the libraries first download the library's zip file then go to sketch > include Library > ADD .ZIP File > Select the downloaded
// libary's Zip file and you're done now. You can upload the the sketch now.
// For more details you can contact me here www.instagram.com/diy.builder


#include<SPI.h>
#include<nRF24L01.h>
#include<RF24.h>

const int x_out = A0;
const int y_out = A1;

RF24 radio(8,10);

const byte address[6] = "00001";
struct data
{
  int xAxis;
  int yAxis;
};
data send_data;

int X_MIN = 1000;
int X_MAX = -1000;
int Y_MIN = 1000;
int Y_MAX = -1000;

int V0_MIN = 1000;
int V0_MAX = -1000;
int V1_MIN = 1000;
int V1_MAX = -1000;
int V2_MIN = 1000;
int V2_MAX = -1000;


// ---------------------------------------- //
// ---------------- SETUP ----------------- //
// ---------------------------------------- //
void setup() 
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();

  for (int i = 0; i < 100; i++)
  {
    send_data.xAxis = analogRead(x_out);
    send_data.yAxis = analogRead(y_out);
    
    //Serial.print("X = "); Serial.println(send_data.xAxis);
    //Serial.print("Y = "); Serial.println(send_data.yAxis);
  
    X_MIN = send_data.xAxis < X_MIN ? send_data.xAxis : X_MIN;
    X_MAX = send_data.xAxis > X_MAX ? send_data.xAxis : X_MAX;
    Y_MIN = send_data.xAxis < Y_MIN ? send_data.xAxis : Y_MIN;
    Y_MAX = send_data.xAxis > Y_MAX ? send_data.xAxis : Y_MAX;

    //V0_MIN = 
    Serial.println(i);
    
    delay(50);
  }

  Serial.print("X_MIN = "); Serial.println(X_MIN);
  Serial.print("X_MAX = "); Serial.println(X_MAX);
  Serial.print("Y_MIN = "); Serial.println(Y_MIN);
  Serial.print("Y_MAX = "); Serial.println(Y_MAX);
}


// ---------------------------------------- //
// ----------------- LOOP ----------------- //
// ---------------------------------------- //
void loop() 
{
  Serial.print("X = "); Serial.println(analogRead(x_out));
  Serial.print("Y = "); Serial.println(analogRead(y_out));
  delay(1000); 
}
