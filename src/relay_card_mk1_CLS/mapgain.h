void mapgain() {


  //  gaincomensatedroot=smoothgainmap;
  smoothgainmap = map(smoothgain, 0, 255, 0, 156);  // 156

  if (smoothgainmap <= 7 && smoothgainmap >= 0) {
    gaincomensatedroot = smoothgainmap;
  }
  if (smoothgainmap <= 14 && smoothgainmap >= 8) {
    gaincomensatedroot = smoothgainmap + 1;
  }
  if (smoothgainmap <= 18 && smoothgainmap >= 15) {
    gaincomensatedroot = smoothgainmap + 3;  //+4
  }
  if (smoothgainmap == 19) {
    gaincomensatedroot = smoothgainmap + 4;
  }
  if (smoothgainmap <= 23 && smoothgainmap >= 20) {
    gaincomensatedroot = smoothgainmap + 5;
  }
  if (smoothgainmap <= 25 && smoothgainmap >= 24) {
    gaincomensatedroot = smoothgainmap + 6;
  }
  if (smoothgainmap <= 28 && smoothgainmap >= 26) {
    gaincomensatedroot = smoothgainmap + 7;
  }
  if (smoothgainmap <= 31 && smoothgainmap >= 29) {
    gaincomensatedroot = smoothgainmap + 8;
  }
  if (smoothgainmap <= 35 && smoothgainmap >= 32) {
    gaincomensatedroot = smoothgainmap + 9;
  }
  if (smoothgainmap <= 37 && smoothgainmap >= 36) {
    gaincomensatedroot = smoothgainmap + 10;
  }
  if (smoothgainmap == 38) {
    gaincomensatedroot = smoothgainmap + 11;
  }
  if (smoothgainmap <= 40 && smoothgainmap >= 39) {
    gaincomensatedroot = smoothgainmap + 12;
  }
  if (smoothgainmap <= 42 && smoothgainmap >= 41) {
    gaincomensatedroot = smoothgainmap + 13;
  }
  if (smoothgainmap <= 44 && smoothgainmap >= 43) {
    gaincomensatedroot = smoothgainmap + 14;
  }
  if (smoothgainmap <= 46 && smoothgainmap >= 45) {
    gaincomensatedroot = smoothgainmap + 15;
  }
  if (smoothgainmap == 47) {
    gaincomensatedroot = smoothgainmap + 16;
  }
  if (smoothgainmap <= 58 && smoothgainmap >= 48) {
    gaincomensatedroot = smoothgainmap + 18;
  }
  if (smoothgainmap <= 60 && smoothgainmap >= 59) {
    gaincomensatedroot = smoothgainmap + 18;
  }
  if (smoothgainmap <= 63 && smoothgainmap >= 61) {
    gaincomensatedroot = smoothgainmap + 19;
  }
  if (smoothgainmap <= 66 && smoothgainmap >= 64) {
    gaincomensatedroot = smoothgainmap + 20;
  }
  if (smoothgainmap <= 68 && smoothgainmap >= 67) {
    gaincomensatedroot = smoothgainmap + 21;
  }
  if (smoothgainmap <= 70 && smoothgainmap >= 69) {
    gaincomensatedroot = smoothgainmap + 22;
  }
  if (smoothgainmap <= 72 && smoothgainmap >= 71) {
    gaincomensatedroot = smoothgainmap + 23;
  }
  if (smoothgainmap <= 74 && smoothgainmap >= 73) {
    gaincomensatedroot = smoothgainmap + 24;
  }
  if (smoothgainmap <= 76 && smoothgainmap >= 75) {
    gaincomensatedroot = smoothgainmap + 25;
  }
  if (smoothgainmap <= 78 && smoothgainmap >= 77) {
    gaincomensatedroot = smoothgainmap + 26;
  }
  if (smoothgainmap <= 80 && smoothgainmap >= 79) {
    gaincomensatedroot = smoothgainmap + 27;
  }
  if (smoothgainmap == 81) {
    gaincomensatedroot = smoothgainmap + 28;
  }
  if (smoothgainmap == 82) {
    gaincomensatedroot = smoothgainmap + 29;
  }
  if (smoothgainmap <= 84 && smoothgainmap >= 83) {
    gaincomensatedroot = smoothgainmap + 30;
  }
  if (smoothgainmap <= 86 && smoothgainmap >= 85) {
    gaincomensatedroot = smoothgainmap + 31;
  }
  if (smoothgainmap == 87) {
    gaincomensatedroot = smoothgainmap + 32;
  }
  if (smoothgainmap == 88) {
    gaincomensatedroot = smoothgainmap + 33;
  }
  if (smoothgainmap <= 90 && smoothgainmap >= 89) {
    gaincomensatedroot = smoothgainmap + 34;
  }
  if (smoothgainmap <= 104 && smoothgainmap >= 91) {
    gaincomensatedroot = smoothgainmap + 35;
  }
  if (smoothgainmap <= 107 && smoothgainmap >= 105) {
    gaincomensatedroot = smoothgainmap + 36;
  }
  if (smoothgainmap <= 110 && smoothgainmap >= 108) {
    gaincomensatedroot = smoothgainmap + 37;
  }
  if (smoothgainmap <= 113 && smoothgainmap >= 111) {
    gaincomensatedroot = smoothgainmap + 38;
  }
  if (smoothgainmap <= 115 && smoothgainmap >= 114) {
    gaincomensatedroot = smoothgainmap + 39;
  }
  if (smoothgainmap <= 118 && smoothgainmap >= 116) {
    gaincomensatedroot = smoothgainmap + 40;
  }
  if (smoothgainmap == 119) {
    gaincomensatedroot = smoothgainmap + 42;
  }
  if (smoothgainmap <= 121 && smoothgainmap >= 120) {
    gaincomensatedroot = smoothgainmap + 43;
  }
  if (smoothgainmap == 122) {
    gaincomensatedroot = smoothgainmap + 44;
  }
  if (smoothgainmap <= 124 && smoothgainmap >= 123) {
    gaincomensatedroot = smoothgainmap + 45;
  }
  if (smoothgainmap <= 126 && smoothgainmap >= 125) {
    gaincomensatedroot = smoothgainmap + 46;
  }
  if (smoothgainmap <= 128 && smoothgainmap >= 127) {
    gaincomensatedroot = smoothgainmap + 47;
  }
  if (smoothgainmap == 129) {
    gaincomensatedroot = smoothgainmap + 48;
  }
  if (smoothgainmap == 130) {
    gaincomensatedroot = smoothgainmap + 49;
  }
  if (smoothgainmap == 131) {
    gaincomensatedroot = smoothgainmap + 50;
  }
  if (smoothgainmap == 132) {
    gaincomensatedroot = smoothgainmap + 51;
  }
  if (smoothgainmap <= 134 && smoothgainmap >= 133) {
    gaincomensatedroot = smoothgainmap + 52;
  }
  if (smoothgainmap == 135) {
    gaincomensatedroot = smoothgainmap + 53;
  }
  if (smoothgainmap == 136) {
    gaincomensatedroot = smoothgainmap + 54;
  }
  if (smoothgainmap == 137) {
    gaincomensatedroot = smoothgainmap + 60;
  }
  if (smoothgainmap == 138) {
    gaincomensatedroot = smoothgainmap + 61;
  }
  if (smoothgainmap == 139) {
    gaincomensatedroot = smoothgainmap + 63;
  }
  if (smoothgainmap == 140) {
    gaincomensatedroot = smoothgainmap + 64;
  }
  if (smoothgainmap == 141) {
    gaincomensatedroot = smoothgainmap + 64;
  }
  if (smoothgainmap == 142) {
    gaincomensatedroot = smoothgainmap + 64;
  }
  if (smoothgainmap == 143) {
    gaincomensatedroot = smoothgainmap + 65;
  }
  if (smoothgainmap == 144) {
    gaincomensatedroot = smoothgainmap + 68;
  }
  if (smoothgainmap == 145) {
    gaincomensatedroot = smoothgainmap + 70;
  }
  if (smoothgainmap == 146) {
    gaincomensatedroot = smoothgainmap + 71;
  }
  if (smoothgainmap == 147) {
    gaincomensatedroot = smoothgainmap + 73;
  }
  if (smoothgainmap == 148) {
    gaincomensatedroot = smoothgainmap + 75;
  }
  if (smoothgainmap == 149) {
    gaincomensatedroot = smoothgainmap + 78;
  }
  if (smoothgainmap == 150) {
    gaincomensatedroot = smoothgainmap + 80;
  }
  if (smoothgainmap == 151) {
    gaincomensatedroot = smoothgainmap + 83;
  }
  if (smoothgainmap == 152) {
    gaincomensatedroot = smoothgainmap + 84;
  }
  if (smoothgainmap == 153) {
    gaincomensatedroot = smoothgainmap + 87;
  }
  if (smoothgainmap == 154) {
    gaincomensatedroot = smoothgainmap + 92;
  }
  if (smoothgainmap == 155) {
    gaincomensatedroot = smoothgainmap + 95;
  }
  if (smoothgainmap == 156) {
    gaincomensatedroot = smoothgainmap + 99;
  }
}