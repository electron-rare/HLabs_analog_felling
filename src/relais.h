// *******************************************************************************************************
// ****************************************** gestion relais *********************************************
// *******************************************************************************************************
#include "PCF8574.h" // https://github.com/RobTillaart/PCF8574/blob/master/examples/PCF8574_test1/PCF8574_test1.ino

// instance carte relais
PCF8574 PCF_vol_0(vol_0_relais);
PCF8574 PCF_gain_0(gain_0_relais);
PCF8574 PCF_vol_1(vol_1_relais);
PCF8574 PCF_gain_1(gain_1_relais);

#define delay_relais 1 // délai entre chaque relais
void relais_set(int relais);
void relais_send(int relais, int value, int byte_value);

void relais_send(int relais, int value, int byte_value) // fonction d'envoie de valeur sur relais
{
#ifdef DEBUG_RELAIS // si DEBUG activé
    Log.notice("relais_send(%d, %d, %d)" CR, relais, value, byte_value);
#endif

    // PCF.write(const uint8_t pin, const uint8_t value) writes a single pin; pin = 0..7; value is HIGH(1) or LOW (0)
    // uint8_t write(const uint8_t value) writes all pins; value = 0..255
    switch (relais)
    {
    case 0:
#ifdef DEBUG_RELAIS // si DEBUG activé
        Log.notice("PCF_gain_0.write(%d, %d)" CR, byte_value, value);
#endif
        PCF_gain_0.write(byte_value, value); // envoie valeur sur relais volume gauche
        break;

    case 1:
#ifdef DEBUG_RELAIS // si DEBUG activé
        Log.notice("PCF_vol_0.write(%d, %d)" CR, byte_value, value);
#endif
        PCF_vol_0.write(byte_value, value); // envoie valeur sur relais gain gauche
        break;

    case 2:
#ifdef DEBUG_RELAIS // si DEBUG activé
        Log.notice("PCF_gain_1.write(%d, %d)" CR, byte_value, value);
#endif
        PCF_gain_1.write(byte_value, value); // envoie valeur sur relais volume droite
        break;

    case 3:
#ifdef DEBUG_RELAIS // si DEBUG activé
        Log.notice("PCF_vol_1.write(%d, %d)" CR, byte_value, value);
#endif
        PCF_vol_1.write(byte_value, value); // envoie valeur sur relais gain droite
        break;
    }
    delay(delay_relais);
}


void relais_set(int relais) // fonction de gestion des relais
{
    // tableau de valeur des relais
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

#ifdef DEBUG_RELAIS // si DEBUG activé
    Log.notice("relais_map[%d] = %d" CR, relais, relais_map[relais]);
    Log.notice("relais_old[%d] = %d" CR, relais, relais_old[relais]);
#endif

    if (relais_map[relais] != relais_old[relais]) // si changement de valeur
    {
        // si changement en sens montant
        if (relais_map[relais] > relais_old[relais])
        {
#ifdef DEBUG_RELAIS // si DEBUG activé
            Log.notice("relais UP" CR);
            Log.notice("relais_map[%d] = %d" CR, relais, relais_map[relais]);
            Log.notice("relais_old[%d] = %d" CR, relais, relais_old[relais]);
#endif
            for (int j = 0; j <= 7; j++)
            {
                if (bitRead(relais_map[relais], j) == 1)
                {
                    relais_send(relais, 1, j);
                }
                else if (bitRead(relais_map[relais], j) == 0)
                {
                    relais_send(relais, 0, j);
                }
            }
        }
        // si changement en sens descendant
        if (relais_map[relais] < relais_old[relais])
        {
#ifdef DEBUG_RELAIS // si DEBUG activé
            Log.notice("relais DOWN" CR);
            Log.notice("relais_map[%d] = %d" CR, relais, relais_map[relais]);
            Log.notice("relais_old[%d] = %d" CR, relais, relais_old[relais]);
#endif
            for (int j = 7; j >= 0; j--)
            {
                if (bitRead(relais_map[relais], j) == 1)
                {
                    relais_send(relais, 1, j);
                }
                else if (bitRead(relais_map[relais], j) == 0)
                {
                    relais_send(relais, 0, j);
                }
            }
        }
        relais_old[relais] = relais_map[relais]; // sauvegarde valeur
    }
}
