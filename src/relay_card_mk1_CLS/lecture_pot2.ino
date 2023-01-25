
void gain_pot() {

  smoothgain = byte(gain_1.getValue() / 4);  // passage de 9 a 48 bits et attention version inversée, fonctionne comme un potentiometre "normal"  ~
}
