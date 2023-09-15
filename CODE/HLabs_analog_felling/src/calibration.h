/*******************************************************************************************************
Gestion de la calibration des min et max des potentiomètres et stockage dans l'EEPROM
********************************************************************************************************/
#define speed 20
#define interval_cal 3000

void calibration_pot(int i); // definition fonction de calibration des potentiomètres

void calibration_pot(int i) // calibration des potentiomètres
{
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
  // calibration du min des potentiomètres
  digitalWrite(stereo_link_led, HIGH);    // allume LED stereo_link
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
    max_pot[i] = gain_0.getValue();
    break;
  case 1:
    vol_0.update();
    max_pot[i] = vol_0.getValue();
    break;
  case 2:
    gain_1.update();
    max_pot[i] = gain_1.getValue();
    break;
  case 3:
    vol_1.update();
    max_pot[i] = vol_1.getValue();
    break;
  }
  EEPROM.write(i + 10, max_pot[i] / 4);
  delay(500);

  // *******************************************************************************************************
  // calibration du max des potentiomètres
  digitalWrite(stereo_link_led, HIGH);   // éteint LED stereo_link
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
    min_pot[i] = gain_0.getValue();
    break;
  case 1:
    vol_0.update();
    min_pot[i] = vol_0.getValue();
    break;
  case 2:
    gain_1.update();
    min_pot[i] = gain_1.getValue();
    break;
  case 3:
    vol_1.update();
    min_pot[i] = vol_1.getValue();
    break;
  }
  EEPROM.write(i, min_pot[i] / 4);
  delay(500);
  digitalWrite(stereo_link_led, HIGH); // éteint LED stereo_link

  // remise à zéro du potentiomètres
  digitalWrite(stereo_link_led, HIGH);           // allume LED stereo_link
  while (analogRead(analog_pot[i]) < max_pot[i]) // en analogue read les valeurs sont inversées
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