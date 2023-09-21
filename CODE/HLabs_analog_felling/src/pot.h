/*******************************************************************************************************
Gestion de la lecture des potentiomètres et des boutons
********************************************************************************************************/

// make a ResponsiveAnalogRead object, pass in the pin, and either true or false depending on if you want sleep enabled
// enabling sleep will cause values to take less time to stop changing and potentially stop changing more abruptly,
// where as disabling sleep will cause values to ease into their correct position smoothly and with slightly greater accuracy

// the next optional argument is snapMultiplier, which is set to 0.01 by default
// you can pass it a value from 0 to 1 that controls the amount of easing
// increase this to lessen the amount of easing (such as 0.1) and make the responsive values more responsive
// but doing so may cause more noise to seep through if sleep is not enabled
#include <ResponsiveAnalogRead.h>

#define amount_easing 0.1 // valeur d'atténuation du potentiomètre
#define sleep_mode true   // true = mode sleep activé, false = mode sleep désactivé
ResponsiveAnalogRead gain_0(gain_0_pot, sleep_mode, amount_easing);
ResponsiveAnalogRead vol_0(vol_0_pot, sleep_mode, amount_easing);
ResponsiveAnalogRead gain_1(gain_1_pot, sleep_mode, amount_easing);
ResponsiveAnalogRead vol_1(vol_1_pot, sleep_mode, amount_easing);

void save_pot(int i);             // sauvegarde position potentiomètre
void lecture_pot(int i);          // lecture analogique potentiomètre
void adapt_pot(int value, int i); // adaptation de la valeur du potentiomètre

void save_pot(int i) // sauvegarde position potentiomètre
{
    position_save[i] = position_lue[i]; // sauvegarde position potentiomètre
    position_change[i] = false;         // RAZ flag de changement de position du potentiomètre
}

#define offset_pot 1   // valeur de décalage de mise à jour du potentiomètre
#define nb_count_pot 1 // nombre de lecture du potentiomètre avant mise à jour

void adapt_pot(int value, int i)
{
    last_pot_change[i] = millis();
    valut_pot_old[i] = value;
    position_change[i] = true; // il y a changement de position du potentiomètre
    int map_value = map(value, max_pot[i], min_pot[i], min_val_pot, max_val_pot);
    //map_value = map_value / divider_analog;
#ifdef CALIBRATED
    if (map_value <= calibrating_midle_pot[i])
        map_value = map(map_value, 0, calibrating_midle_pot[i], min_val_pot, middle_value);
    else if (map_value > calibrating_midle_pot[i])
        map_value = map(map_value, calibrating_midle_pot[i], max_val_pot, middle_value + 1, max_val_pot);
#endif
    position_lue[i] = constrain(map_value, min_val_pot, max_val_pot);

    Serial.print("position_lue[");
    Serial.print(i);
    Serial.print("] = ");
    Serial.println(position_lue[i]);
    Serial.print("map_value = ");
    Serial.println(map_value);
    Serial.print("value = ");
    Serial.println(value);
}

void lecture_pot(int i) // lecture analogique potentiomètre
{
    switch (i)
    {
    case 0:
        gain_0.update();
        if (gain_0.hasChanged())
        {
            Serial.print("gain_0.getValue() = ");
            Serial.println(gain_0.getValue());
            int value = gain_0.getValue();
            if (value > valut_pot_old[i] + offset_pot or value < valut_pot_old[i] - offset_pot)
                count_pot_change[i]++;
            if (count_pot_change[i] > nb_count_pot)
                adapt_pot(value, i);
        }
        break;

    case 1:
        vol_0.update();
        if (vol_0.hasChanged())
        {
            Serial.print("vol_0.getValue() = ");
            Serial.println(vol_0.getValue());
            int value = vol_0.getValue();
            if (value > valut_pot_old[i] + offset_pot or value < valut_pot_old[i] - offset_pot)
                count_pot_change[i]++;
            if (count_pot_change[i] > nb_count_pot)
                adapt_pot(value, i);
        }
        break;

    case 2:
        gain_1.update();
        if (gain_1.hasChanged())
        {
            Serial.print("gain_1.getValue() = ");
            Serial.println(gain_1.getValue());
            int value = gain_1.getValue();
            if (value > valut_pot_old[i] + offset_pot or value < valut_pot_old[i] - offset_pot)
                count_pot_change[i]++;
            if (count_pot_change[i] > nb_count_pot)
                adapt_pot(value, i);
        }
        break;

    case 3:
        vol_1.update();
        if (vol_1.hasChanged())
        {
            Serial.print("vol_1.getValue() = ");
            Serial.println(vol_1.getValue());
            int value = vol_1.getValue();
            if (value > valut_pot_old[i] + offset_pot or value < valut_pot_old[i] - offset_pot)
                count_pot_change[i]++;
            if (count_pot_change[i] > nb_count_pot)
                adapt_pot(value, i);
        }
        break;
    }
}