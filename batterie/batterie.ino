/*************
 * Ce code permet d'allumer les led en fonction de 
 * la charge de la batterie
 */

enum led {ROUGE = 3, 
          JAUNE1 = 5, JAUNE2 = 6, 
          VERT1 = 9, VERT2 = 10, VERT3 = 11};

const byte pinLed[6] = {3, 5, 6, 9, 10, 11};

const byte tension = A0;


// ---------------------------------------- //
// -                SETUP                 - //
// ---------------------------------------- //
void setup() 
{
  Serial.begin(9600);
  Serial.print(__FILE__); Serial.println(" initialising ...");
  
  for(int i = 0; i < 6; i++)
  {
    pinMode(pinLed[i], OUTPUT);
  }
}


// ---------------------------------------- //
// -                LOOP                  - //
// ---------------------------------------- //
void loop() 
{
   short lecture = analogRead(tension);

}


int batt;

void setup() {
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT); 
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT); 
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  batt = analogRead(A0);
  Serial.println(batt);
  if(batt < 827)
  {
    analogWrite(3, LOW);
    analogWrite(5, LOW);
    analogWrite(6, LOW);
    analogWrite(9, LOW);
    analogWrite(10, LOW);
    analogWrite(11, LOW);
    digitalWrite(13, LOW);
  }
  if((batt >= 827 )&& (batt < 850))
  {
    analogWrite(3, map(batt, 827, 850, 0, 255));
    analogWrite(5, LOW);
    analogWrite(6, LOW);
    analogWrite(9, LOW);
    analogWrite(10, LOW);
    analogWrite(11, LOW);
    digitalWrite(13, LOW);
  }
  if((batt >= 850 )&& (batt < 873))
  {
    analogWrite(3, 255);
    analogWrite(5, map(batt, 850, 873, 0, 255));
    analogWrite(6, LOW);
    analogWrite(9, LOW);
    analogWrite(10, LOW);
    analogWrite(11, LOW);
    digitalWrite(13, LOW);
  }
  if((batt >= 873 )&& (batt < 896))
  {
    analogWrite(3, 255);
    analogWrite(5, 255);
    analogWrite(6, map(batt, 873, 896, 0, 255));
    analogWrite(9, LOW);
    analogWrite(10, LOW);
    analogWrite(11, LOW);
    digitalWrite(13, LOW);
  }
  if((batt >= 896 )&& (batt < 920))
  {
    analogWrite(3, 255);
    analogWrite(5, 255);
    analogWrite(6, 255);
    analogWrite(9, map(batt, 896, 920, 0, 255));
    analogWrite(10, LOW);
    analogWrite(11, LOW);
    digitalWrite(13, LOW);
  }
  if((batt >= 920 )&& (batt < 943))
  {
    analogWrite(3, 255);
    analogWrite(5, 255);
    analogWrite(6, 255);
    analogWrite(9, 255);
    analogWrite(10, map(batt, 920, 943, 0, 255));
    analogWrite(11, LOW);
    digitalWrite(13, LOW);
  }
  if((batt >= 943 )&& (batt < 966))
  {
    analogWrite(3, 255);
    analogWrite(5, 255);
    analogWrite(6, 255);
    analogWrite(9, 255);
    analogWrite(10, 255);
    analogWrite(11, map(batt, 943, 966, 0, 255));
    digitalWrite(13, LOW);
  }
  if(batt >= 966 )
  {
    analogWrite(3, 255);
    analogWrite(5, 255);
    analogWrite(6, 255);
    analogWrite(9, 255);
    analogWrite(10, 255);
    analogWrite(11, 255);
    digitalWrite(13, HIGH);
  }
  

}
