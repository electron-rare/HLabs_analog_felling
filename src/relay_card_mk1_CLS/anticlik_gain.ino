void anticlikgain() {

  newvaluegain = gaincomensatedroot;

  if (newvaluegain < actualvaluegain) {
    for (int i = 7; i <= 0; i--) {  // mise à jout des bits sens montée
      bitWrite(I2Csendgain, i, bitRead(newvaluegain, i));
      I2cgain();  // envoi I2C
      delay(delaytime);
    }
    actualvaluegain = newvaluegain;
    I2Csendgain = actualvaluegain;
  }

  // sens inverse

  if (newvaluegain > actualvaluegain) {
    I2Csendgain = actualvaluegain;
    for (int i = 0; i < 7; i--) {  // mise à jout des bits sens montée
      bitWrite(I2Csendgain, i, bitRead(newvaluegain, i));
      I2cgain();  // envoi I2C
      delay(delaytime);
      actualvaluegain = newvaluegain;
    }
  }
}
