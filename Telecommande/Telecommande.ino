/**
 * Ce code joue le role de telecommande du Uno-Braccio.
 * Chaque appuie sur le boutton déclenche une action
 */

#include <U8g2lib.h>
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

//Premier bouton
const byte bouton = 2;

void setup() 
{
  // Initialisation
  u8g2.begin();

  pinMode(bouton, INPUT);

  // Affichage de données à l'écran
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB14_tr) ;  // sélection d'une police de caractère
  u8g2.drawStr(0, 10, "Ligne 1");   // écriture à la position (x=0 et y=10)
  u8g2.drawStr(0, 28, "Ligne 2");     // écriture à la position (x=0 et y=28)
  u8g2.drawStr(0, 46, "Ligne 3");     // écriture à la position (x=0 et y=46)
  u8g2.drawStr(0, 64, "Ligne 4");     // écriture à la position (x=0 et y=64)
  u8g2.sendBuffer();                // affichage de ces infos à l'écran
  delay(500);
}

void loop() 
{
  //vide le buffer
  u8g2.clearBuffer();

  //affiche "bouton"
  u8g2.drawStr(0, 20, "Bouton");

  //affiche l'état du bouton (n'accepte pas les caractères accentués)
  if (digitalRead(bouton) == LOW)
    u8g2.drawStr(0, 40, "relache");
  
  else 
    u8g2.drawStr(0, 40, "appuye");
  
  //envoie les infos dans le buffer
  u8g2.sendBuffer();
  delay(100);
}
