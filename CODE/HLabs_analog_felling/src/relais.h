/*******************************************************************************************************
gestion des valeurs des relais avec incrément et décrément et de l'envoie des valeurs sur les cartes relais
*******************************************************************************************************/

#include "PCF8574.h" // https://github.com/RobTillaart/PCF8574/blob/master/examples/PCF8574_test1/PCF8574_test1.ino

// instance carte relais
PCF8574 PCF_vol_0(vol_0_relais);
PCF8574 PCF_gain_0(gain_0_relais);
PCF8574 PCF_vol_1(vol_1_relais);
PCF8574 PCF_gain_1(gain_1_relais);

void relais_set(int relais);                             // fonction de mappage des valeur pour envoie sur les cartes relais
void relais_send(int relais, int value, int byte_value); // fonction d'envoie de valeur sur les cartes relais

void relais_set(int relais)
{
    // mise à jour du tableau de valeur des relais
    switch (relais)
    {
    case 0:
        relais_map[0] = relais_gain_val[0];
        break;
    case 1:
        relais_map[1] = relais_vol_val[0];
        break;
    case 2:
        relais_map[2] = relais_gain_val[1];
        break;
    case 3:
        relais_map[3] = relais_vol_val[1];
        break;
    }

    if (relais_map[relais] != relais_old[relais]) // si changement de valeur
    {
        // si changement en sens montant
        if (relais_map[relais] > relais_old[relais])
        {
            for (int j = 0; j <= 7; j++)
            {
                if (bitRead(relais_map[relais], j) == 1)
                {
                    if (relais == 0 or relais == 2)
                        relais_send(relais, 1, j);
                    else if (relais == 1 or relais == 3)
                        relais_send(relais, 0, j);
                }
                else if (bitRead(relais_map[relais], j) == 0)
                {
                    if (relais == 0 or relais == 2)
                        relais_send(relais, 0, j);
                    else if (relais == 1 or relais == 3)
                        relais_send(relais, 1, j);
                }
            }
        }
        // si changement en sens descendant
        if (relais_map[relais] < relais_old[relais])
        {
            for (int j = 7; j >= 0; j--)
            {
                if (bitRead(relais_map[relais], j) == 1)
                {
                    if (relais == 0 or relais == 2)
                        relais_send(relais, 1, j);
                    else if (relais == 1 or relais == 3)
                        relais_send(relais, 0, j);
                }
                else if (bitRead(relais_map[relais], j) == 0)
                {
                    if (relais == 0 or relais == 2)
                        relais_send(relais, 0, j);
                    else if (relais == 1 or relais == 3)
                        relais_send(relais, 1, j);
                }
            }
        }
        relais_old[relais] = relais_map[relais]; // sauvegarde valeur de comparaison
    }
}

void relais_send(int relais, int value, int byte_value)
{
    // PCF.write(const uint8_t pin, const uint8_t value) writes a single pin; pin = 0..7; value is HIGH(1) or LOW (0)
    // uint8_t write(const uint8_t value) writes all pins; value = 0..255
    // envoie I2C des valeur sur les cartes relais
    switch (relais)
    {
    case 0:
        PCF_gain_0.write(byte_value, value); // carte relais gain gauche
        break;
    case 1:
        PCF_vol_0.write(byte_value, value); // carte relais volume gauche
        break;
    case 2:
        PCF_gain_1.write(byte_value, value); // carte relais gain droit
        break;
    case 3:
        PCF_vol_1.write(byte_value, value); // carte relais volume droit
        break;
    }
    delay(delay_relais);
}