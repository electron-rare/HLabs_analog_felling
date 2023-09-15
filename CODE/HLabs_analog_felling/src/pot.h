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

#define amount_easing 0.8 // valeur d'atténuation du potentiomètre

ResponsiveAnalogRead gain_0(gain_0_pot, true, amount_easing);
ResponsiveAnalogRead vol_0(vol_0_pot, true, amount_easing);
ResponsiveAnalogRead gain_1(gain_1_pot, true, amount_easing);
ResponsiveAnalogRead vol_1(vol_1_pot, true, amount_easing);

void save_pot(int i);        // sauvegarde position potentiomètre
void lecture_pot(int i);     // lecture analogique potentiomètre

void save_pot(int i)
{
    position_save[i] = position_lue[i]; // sauvegarde position potentiomètre
    position_change[i] = false;         // RAZ flag de changement de position du potentiomètre
}

// *******************************************************************************************************
// ****************************************** lecture bouton *********************************************
// *******************************************************************************************************
void lecture_pot(int i)
{
    switch (i)
    {
    case 0:
        gain_0.update();
        if (gain_0.hasChanged())
        {
            position_change[i] = true;   // il y a changement de position du potentiomètre
            int value = gain_0.getValue();
            int map_value = map(value, max_pot[i], min_pot[i], 0, 1023);
            position_lue[i] = constrain(map_value, 0, 1023);
        }
        break;

    case 1:
        vol_0.update();
        if (vol_0.hasChanged())
        {
            position_change[i] = true;   // il y a changement de position du potentiomètre
            int value = vol_0.getValue();
            int map_value = map(value, max_pot[i], min_pot[i], 0, 1023);
            position_lue[i] = constrain(map_value, 0, 1023);
        }
        break;

    case 2:
        gain_1.update();
        if (gain_1.hasChanged())
        {
            position_change[i] = true;   // il y a changement de position du potentiomètre
            int value = gain_1.getValue();
            int map_value = map(value, max_pot[i], min_pot[i], 0, 1023);
            position_lue[i] = constrain(map_value, 0, 1023);
        }
        break;

    case 3:
        vol_1.update();
        if (vol_1.hasChanged())
        {
            position_change[i] = true;   // il y a changement de position du potentiomètre
            int value = vol_1.getValue();
            int map_value = map(value, max_pot[i], min_pot[i], 0, 1023);
            position_lue[i] = constrain(map_value, 0, 1023);
        }
        break;
    }
}