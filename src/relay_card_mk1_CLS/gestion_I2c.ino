void I2c() {


  // transmition I2C vers relais
  Wire.beginTransmission(0x20); //32 dec // transmit to device #8 A2L  A1L A0L volume

  Wire.write(I2Csend);
  Wire.endTransmission();  // stop transmitting

  Wire.beginTransmission(0x38);  //56 dec // transmit to device #8 A2L  A1L A0L volume

  Wire.write(I2Csend);
  Wire.endTransmission();  // stop transmitting
}