// *******************************************************************************************************
// ****************************************** gestion valeurs ********************************************
// *******************************************************************************************************
void valeurs_set(int i);       // gestion des valeurs
void valeurs_const_set(int i); // gestion des valeurs constant ouptut
void smoothgain_set(int i);    // calcul des valeurs lissées de gain et mise à jour des relais
void smoothvolume_set(int i);  // calcul des valeurs lissées de volume et mise à jour des relais

void valeurs_set(int i)
{
    if (position_change[i] == true && position_lue[i] != position_save[i]) // si changement d'état du potentiomètre
    {
        switch (i)
        {
        case 0:
            smoothgain[0] = byte(position_lue[0] / 4); // adaptation de valeur
            smoothgain_set(0);                         // mise à jour de valeur du relais gain gauche
            break;
        case 1:
            smoothvol[0] = byte(position_lue[1] / 4) - 1; // adaptation de valeur
            smoothvolume_set(0);
            break;
        case 2:
            smoothgain[1] = byte(position_lue[2] / 4); // adaptation de valeur
            smoothgain_set(1);                         // mise à jour de valeur du relais gain droit
            break;
        case 3:
            smoothvol[1] = byte(position_lue[3] / 4) - 1; // adaptation de valeur
            smoothvolume_set(1);                          // mise à jour de valeur du relais volume droit
            break;
        default:
            break;
        }
    }
}

void valeurs_const_set(int i)
{
    if (position_change[i] == true && position_set[i] != position_save[i]) // si changement d'état du potentiomètre
    {
        switch (i)
        {
        case 0:
            smoothgain[0] = byte(position_set[0] / 4); // adaptation de valeur
            smoothgain_set(0);                         // mise à jour de valeur du relais gain gauche
            break;
        case 1:
            smoothvol[0] = byte(position_set[1] / 4) - 1; // adaptation de valeur
            smoothvolume_set(0);                          // mise à jour de valeur du relais volume gauche
            break;
        case 2:
            smoothgain[1] = byte(position_set[2] / 4); // adaptation de valeur
            smoothgain_set(1);                         // mise à jour de valeur du relais gain droit
            break;
        case 3:
            smoothvol[1] = byte(position_set[3] / 4) - 1; // adaptation de valeur
            smoothvolume_set(1);                          // mise à jour de valeur du relais volume droit
            break;
        default:
            break;
        }
    }
}
 
void smoothgain_set(int i)
{
    // lisages manuel des valeurs de gain
    smoothgain[i] = map(smoothgain[i], 0, 255, 0, 156); // 156sm
    {
        relais_gain_val[i] = smoothgain[i];
    }
    if (smoothgain[i] <= 7 && smoothgain[i] >= 0)
    {
        relais_gain_val[i] = smoothgain[i];
    }
    if (smoothgain[i] <= 14 && smoothgain[i] >= 8)
    {
        relais_gain_val[i] = smoothgain[i] + 1;
    }
    if (smoothgain[i] <= 18 && smoothgain[i] >= 15)
    {
        relais_gain_val[i] = smoothgain[i] + 3; //+4
    }
    if (smoothgain[i] == 19)
    {
        relais_gain_val[i] = smoothgain[i] + 4;
    }
    if (smoothgain[i] <= 23 && smoothgain[i] >= 20)
    {
        relais_gain_val[i] = smoothgain[i] + 5;
    }
    if (smoothgain[i] <= 25 && smoothgain[i] >= 24)
    {
        relais_gain_val[i] = smoothgain[i] + 6;
    }
    if (smoothgain[i] <= 28 && smoothgain[i] >= 26)
    {
        relais_gain_val[i] = smoothgain[i] + 7;
    }
    if (smoothgain[i] <= 31 && smoothgain[i] >= 29)
    {
        relais_gain_val[i] = smoothgain[i] + 8;
    }
    if (smoothgain[i] <= 35 && smoothgain[i] >= 32)
    {
        relais_gain_val[i] = smoothgain[i] + 9;
    }
    if (smoothgain[i] <= 37 && smoothgain[i] >= 36)
    {
        relais_gain_val[i] = smoothgain[i] + 10;
    }
    if (smoothgain[i] == 38)
    {
        relais_gain_val[i] = smoothgain[i] + 11;
    }
    if (smoothgain[i] <= 40 && smoothgain[i] >= 39)
    {
        relais_gain_val[i] = smoothgain[i] + 12;
    }
    if (smoothgain[i] <= 42 && smoothgain[i] >= 41)
    {
        relais_gain_val[i] = smoothgain[i] + 13;
    }
    if (smoothgain[i] <= 44 && smoothgain[i] >= 43)
    {
        relais_gain_val[i] = smoothgain[i] + 14;
    }
    if (smoothgain[i] <= 46 && smoothgain[i] >= 45)
    {
        relais_gain_val[i] = smoothgain[i] + 15;
    }
    if (smoothgain[i] == 47)
    {
        relais_gain_val[i] = smoothgain[i] + 16;
    }
    if (smoothgain[i] <= 58 && smoothgain[i] >= 48)
    {
        relais_gain_val[i] = smoothgain[i] + 18;
    }
    if (smoothgain[i] <= 60 && smoothgain[i] >= 59)
    {
        relais_gain_val[i] = smoothgain[i] + 18;
    }
    if (smoothgain[i] <= 63 && smoothgain[i] >= 61)
    {
        relais_gain_val[i] = smoothgain[i] + 19;
    }
    if (smoothgain[i] <= 66 && smoothgain[i] >= 64)
    {
        relais_gain_val[i] = smoothgain[i] + 20;
    }
    if (smoothgain[i] <= 68 && smoothgain[i] >= 67)
    {
        relais_gain_val[i] = smoothgain[i] + 21;
    }
    if (smoothgain[i] <= 70 && smoothgain[i] >= 69)
    {
        relais_gain_val[i] = smoothgain[i] + 22;
    }
    if (smoothgain[i] <= 72 && smoothgain[i] >= 71)
    {
        relais_gain_val[i] = smoothgain[i] + 23;
    }
    if (smoothgain[i] <= 74 && smoothgain[i] >= 73)
    {
        relais_gain_val[i] = smoothgain[i] + 24;
    }
    if (smoothgain[i] <= 76 && smoothgain[i] >= 75)
    {
        relais_gain_val[i] = smoothgain[i] + 25;
    }
    if (smoothgain[i] <= 78 && smoothgain[i] >= 77)
    {
        relais_gain_val[i] = smoothgain[i] + 26;
    }
    if (smoothgain[i] <= 80 && smoothgain[i] >= 79)
    {
        relais_gain_val[i] = smoothgain[i] + 27;
    }
    if (smoothgain[i] == 81)
    {
        relais_gain_val[i] = smoothgain[i] + 28;
    }
    if (smoothgain[i] == 82)
    {
        relais_gain_val[i] = smoothgain[i] + 29;
    }
    if (smoothgain[i] <= 84 && smoothgain[i] >= 83)
    {
        relais_gain_val[i] = smoothgain[i] + 30;
    }
    if (smoothgain[i] <= 86 && smoothgain[i] >= 85)
    {
        relais_gain_val[i] = smoothgain[i] + 31;
    }
    if (smoothgain[i] == 87)
    {
        relais_gain_val[i] = smoothgain[i] + 32;
    }
    if (smoothgain[i] == 88)
    {
        relais_gain_val[i] = smoothgain[i] + 33;
    }
    if (smoothgain[i] <= 90 && smoothgain[i] >= 89)
    {
        relais_gain_val[i] = smoothgain[i] + 34;
    }
    if (smoothgain[i] <= 104 && smoothgain[i] >= 91)
    {
        relais_gain_val[i] = smoothgain[i] + 35;
    }
    if (smoothgain[i] <= 107 && smoothgain[i] >= 105)
    {
        relais_gain_val[i] = smoothgain[i] + 36;
    }
    if (smoothgain[i] <= 110 && smoothgain[i] >= 108)
    {
        relais_gain_val[i] = smoothgain[i] + 37;
    }
    if (smoothgain[i] <= 113 && smoothgain[i] >= 111)
    {
        relais_gain_val[i] = smoothgain[i] + 38;
    }
    if (smoothgain[i] <= 115 && smoothgain[i] >= 114)
    {
        relais_gain_val[i] = smoothgain[i] + 39;
    }
    if (smoothgain[i] <= 118 && smoothgain[i] >= 116)
    {
        relais_gain_val[i] = smoothgain[i] + 40;
    }
    if (smoothgain[i] == 119)
    {
        relais_gain_val[i] = smoothgain[i] + 42;
    }
    if (smoothgain[i] <= 121 && smoothgain[i] >= 120)
    {
        relais_gain_val[i] = smoothgain[i] + 43;
    }
    if (smoothgain[i] == 122)
    {
        relais_gain_val[i] = smoothgain[i] + 44;
    }
    if (smoothgain[i] <= 124 && smoothgain[i] >= 123)
    {
        relais_gain_val[i] = smoothgain[i] + 45;
    }
    if (smoothgain[i] <= 126 && smoothgain[i] >= 125)
    {
        relais_gain_val[i] = smoothgain[i] + 46;
    }
    if (smoothgain[i] <= 128 && smoothgain[i] >= 127)
    {
        relais_gain_val[i] = smoothgain[i] + 47;
    }
    if (smoothgain[i] == 129)
    {
        relais_gain_val[i] = smoothgain[i] + 48;
    }
    if (smoothgain[i] == 130)
    {
        relais_gain_val[i] = smoothgain[i] + 49;
    }
    if (smoothgain[i] == 131)
    {
        relais_gain_val[i] = smoothgain[i] + 50;
    }
    if (smoothgain[i] == 132)
    {
        relais_gain_val[i] = smoothgain[i] + 51;
    }
    if (smoothgain[i] <= 134 && smoothgain[i] >= 133)
    {
        relais_gain_val[i] = smoothgain[i] + 52;
    }
    if (smoothgain[i] == 135)
    {
        relais_gain_val[i] = smoothgain[i] + 53;
    }
    if (smoothgain[i] == 136)
    {
        relais_gain_val[i] = smoothgain[i] + 54;
    }
    if (smoothgain[i] == 137)
    {
        relais_gain_val[i] = smoothgain[i] + 60;
    }
    if (smoothgain[i] == 138)
    {
        relais_gain_val[i] = smoothgain[i] + 61;
    }
    if (smoothgain[i] == 139)
    {
        relais_gain_val[i] = smoothgain[i] + 63;
    }
    if (smoothgain[i] == 140)
    {
        relais_gain_val[i] = smoothgain[i] + 64;
    }
    if (smoothgain[i] == 141)
    {
        relais_gain_val[i] = smoothgain[i] + 64;
    }
    if (smoothgain[i] == 142)
    {
        relais_gain_val[i] = smoothgain[i] + 64;
    }
    if (smoothgain[i] == 143)
    {
        relais_gain_val[i] = smoothgain[i] + 65;
    }
    if (smoothgain[i] == 144)
    {
        relais_gain_val[i] = smoothgain[i] + 68;
    }
    if (smoothgain[i] == 145)
    {
        relais_gain_val[i] = smoothgain[i] + 70;
    }
    if (smoothgain[i] == 146)
    {
        relais_gain_val[i] = smoothgain[i] + 71;
    }
    if (smoothgain[i] == 147)
    {
        relais_gain_val[i] = smoothgain[i] + 73;
    }
    if (smoothgain[i] == 148)
    {
        relais_gain_val[i] = smoothgain[i] + 75;
    }
    if (smoothgain[i] == 149)
    {
        relais_gain_val[i] = smoothgain[i] + 78;
    }
    if (smoothgain[i] == 150)
    {
        relais_gain_val[i] = smoothgain[i] + 80;
    }
    if (smoothgain[i] == 151)
    {
        relais_gain_val[i] = smoothgain[i] + 83;
    }
    if (smoothgain[i] == 152)
    {
        relais_gain_val[i] = smoothgain[i] + 84;
    }
    if (smoothgain[i] == 153)
    {
        relais_gain_val[i] = smoothgain[i] + 87;
    }
    if (smoothgain[i] == 154)
    {
        relais_gain_val[i] = smoothgain[i] + 92;
    }
    if (smoothgain[i] == 155)
    {
        relais_gain_val[i] = smoothgain[i] + 95;
    }
    if (smoothgain[i] == 156)
    {
        relais_gain_val[i] = smoothgain[i] + 99;
    }
    if (i == 0)
    {
        relais_set(0); // mise à jour des relais
    }
    if (i == 1)
    {
        relais_set(2); // mise à jour des relais
    }
}

void smoothvolume_set(int i)
{
    smoothvol[i] = map(smoothvol[i], 0, 255, 0, 255); // map de 0 à 255
    smoothvol[i] = constrain(smoothvol[i], 0, 255);   // map de 0 à 255
    relais_vol_val[i] = smoothvol[i];                 // valeur du relais = valeur du volume
    if (i == 0)
    {
        relais_set(1); // mise à jour des relais
    }
    if (i == 1)
    {
        relais_set(3); // mise à jour des relais
    }
}