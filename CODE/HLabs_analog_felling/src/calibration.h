/*******************************************************************************************************
Gestion de la calibration des min et max des potentiomètres et stockage dans l'EEPROM
********************************************************************************************************/
#include <EEPROM.h>

#define speed 20
#define interval_cal 3000
#define largeur_addr 5
#define nb_addr 4

void calibration_pot(int i); // definition fonction de calibration des potentiomètres

void calibration_pot(int i) // calibration des potentiomètres
{
  int value;
  int map_value;
  int eeprom_value;

  if (analogRead(analog_pot[i]) > 800)
  {
    motor[i].drive(63);
    delay(1000);
  }
  if (analogRead(analog_pot[i]) < 200)
  {
    motor[i].drive(-63);
    delay(1000);
  }

  // *******************************************************************************************************
  // calibration du max des potentiomètres
  digitalWrite(stereo_link_led, HIGH);   // allume LED stereo_link
  while (analogRead(analog_pot[i]) > 50) // en analogue read les valeurs sont inversées
  {
    delay(200);
    digitalWrite(const_out_L_led, LOW);
    digitalWrite(const_out_R_led, HIGH);
    motor[i].drive(63);
    delay(200);
    digitalWrite(const_out_L_led, HIGH);
    digitalWrite(const_out_R_led, LOW);
  }
  motor[i].drive(speed);
  delay(interval_cal);
  motor[i].stop();

  delay(500);
  digitalWrite(stereo_link_led, LOW); // éteint LED stereo_link
  switch (i)
  {
  case 0:
    gain_0.update();
    value = gain_0.getValue();
    break;
  case 1:
    vol_0.update();
    value = vol_0.getValue();
    break;
  case 2:
    gain_1.update();
    value = gain_1.getValue();
    break;
  case 3:
    vol_1.update();
    value = vol_1.getValue();
    break;
  }
  map_value = map(value, max_pot[i], value, 0, 1023);
  eeprom_value = constrain(map_value, 0, 1023);
  max_pot[i] = eeprom_value;
  EEPROM.put((i * largeur_addr), max_pot[i]);
  delay(500);
#ifdef DEBUG // si DEBUG activé
  breakpoint();
#endif
  // *******************************************************************************************************
  // calibration du min des potentiomètres
  digitalWrite(stereo_link_led, HIGH); // éteint LED stereo_link

  while (analogRead(analog_pot[i]) < 950) // en analogue read les valeurs sont inversées
  {
    delay(200);
    digitalWrite(const_out_L_led, HIGH);
    digitalWrite(const_out_R_led, LOW);
    motor[i].drive(-63);
    delay(200);
    digitalWrite(const_out_L_led, LOW);
    digitalWrite(const_out_R_led, HIGH);
  }
  motor[i].drive(-speed);
  delay(interval_cal);
  motor[i].stop();
  delay(500);
  digitalWrite(stereo_link_led, LOW); // éteint LED stereo_link
  switch (i)
  {
  case 0:
    gain_0.update();
    value = gain_0.getValue();
    break;
  case 1:
    vol_0.update();
    value = vol_0.getValue();
    break;
  case 2:
    gain_1.update();
    value = gain_1.getValue();
    break;
  case 3:
    vol_1.update();
    value = vol_1.getValue();
    break;
  }
  map_value = map(value, value, min_pot[i], 0, 1023);
  eeprom_value = constrain(map_value, 0, 1023);
  max_pot[i] = eeprom_value;
  EEPROM.put(((largeur_addr * nb_addr) + (i * largeur_addr)), max_pot[i]);
  delay(500);
#ifdef DEBUG // si DEBUG activé
  breakpoint();
#endif
  // *******************************************************************************************************
  // remise à zéro du potentiomètres
  digitalWrite(stereo_link_led, HIGH);                // allume LED stereo_link
  while (analogRead(analog_pot[i]) < max_pot[i] - 50) // en analogue read les valeurs sont inversées
  {
    delay(200);
    digitalWrite(stereo_link_led, HIGH); // allume LED stereo_link
    motor[i].drive(-63);
    delay(200);
    digitalWrite(stereo_link_led, LOW); // allume LED stereo_link
  }
  motor[i].stop();
  delay(500);
}