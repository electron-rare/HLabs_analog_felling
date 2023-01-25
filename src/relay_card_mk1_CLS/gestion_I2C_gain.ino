void I2cgain() {


  // transmition I2C vers relais
  Wire.beginTransmission(0x21); //33 dec // transmit to device #9 A2L  A1L A0H  gain

  Wire.write(I2Csendgain);
  Wire.endTransmission();   // stop transmitting

  Wire.beginTransmission(0x39);  // 57 dec // transmit to device #9 A2L  A1L A0H  gain

  Wire.write(I2Csendgain);
  Wire.endTransmission();  // stop transmitting
}