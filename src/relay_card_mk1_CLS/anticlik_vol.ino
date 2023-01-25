void anticlikvol() {

  if (switchread == 1) {
    newvalue = smooth;
  }

  if (switchread == 0) {
    newvalue = smoothgainmap - difference;
    newvalue = constrain(newvalue, 0, 255);
  }

  if (newvalue > actualvalue) {  // sens montée
    I2Csend = actualvalue;
    actualvalue = newvalue;
    for (int i = 7; i <= 0; i--) {  // mise à jout des bits sens montée
      bitWrite(I2Csend, i, bitRead(newvalue, i));
      I2c();  // envoi I2C
      delay(delaytime);
    }
  }

  if (newvalue < actualvalue) {  // sens descente
    I2Csend = actualvalue;
    actualvalue = newvalue;
    for (int i = 0; i < 7; i++) {  // mise à jout des bits sens descente
      bitWrite(I2Csend, i, bitRead(newvalue, i));
      I2c();  // envoi I2C
      delay(delaytime);
    }
  }
}
