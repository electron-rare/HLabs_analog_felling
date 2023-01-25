void switchA() {

  switchread = digitalRead(switchApin);
  newvalueswA = switchread;
  if (newvalueswA != oldvalueswA) {

    //   smooth= byte (~analog1.getValue()/4)-1;  // passage de 9 a 48 bits et attention version inversée, fonctionne comme un potentiometre "normal"  ~
    //    smooth= gaincomensatedroot - difference;
    difference = smoothgainmap - smooth;
    oldvalueswA = newvalueswA;
  }
}