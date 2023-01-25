void vol_pot() {

  if (switchread == 1) {

    smooth = byte(vol_1.getValue() / 4) - 1;  // passage de 9 a 48 bits et attention version inversée, fonctionne comme un potentiometre "normal"  ~
  }
}
