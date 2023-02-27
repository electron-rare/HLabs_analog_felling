#include <ResponsiveAnalogRead.h>

// make a ResponsiveAnalogRead object, pass in the pin, and either true or false depending on if you want sleep enabled
// enabling sleep will cause values to take less time to stop changing and potentially stop changing more abruptly,
// where as disabling sleep will cause values to ease into their correct position smoothly and with slightly greater accuracy

// the next optional argument is snapMultiplier, which is set to 0.01 by default
// you can pass it a value from 0 to 1 that controls the amount of easing
// increase this to lessen the amount of easing (such as 0.1) and make the responsive values more responsive
// but doing so may cause more noise to seep through if sleep is not enabled

#define amount_easing 0.05 // valeur d'atténuation du potentiomètre

ResponsiveAnalogRead gain_0(gain_0_pot, true, amount_easing);
ResponsiveAnalogRead vol_0(vol_0_pot, true, amount_easing);
ResponsiveAnalogRead gain_1(gain_1_pot, true, amount_easing);
ResponsiveAnalogRead vol_1(vol_1_pot, true, amount_easing);

void button_position_save(); // calcul des différences de valeurs
void save_pot(int i);        // sauvegarde position potentiomètre
void lecture_pot(int i);     // lecture analogique potentiomètre

void button_position_save() // calcul de la différence entre gain et volume
{
    if (const_out_L_state == 1) // si bouton const_out_L appuyé
    {
        for (int i = 0; i <= 1; i++)
        {
            lecture_pot(i);                    // lecture des potentiomètres
            position_set[i] = position_lue[i]; // set position potentiomètre gain et volume gauche et droit
            position_memory[i] = position_lue[i];
        }
    }

    if (const_out_R_state == 1) // si bouton const_out_R appuyé
    {
        for (int i = 2; i <= 3; i++)
        {
            lecture_pot(i);                    // lecture des potentiomètres
            position_set[i] = position_lue[i]; // set position potentiomètre gain et volume gauche et droit
            position_memory[i] = position_lue[i];
        }
    }

    if (stereo_link_state == 1) // si bouton stereo_link appuyé
    {
        for (int i = 0; i <= 3; i++)
        {
            lecture_pot(i);                    // lecture des potentiomètres
            position_set[i] = position_lue[i]; // set position potentiomètre gain et volume gauche et droit
            position_memory[i] = position_lue[i];
        }
    }
}

void save_pot(int i)
{
    position_save[i] = position_lue[i]; // sauvegarde position potentiomètre
    position_change[i] = false;         // RAZ flag de changement de position du potentiomètre
    last_change_time = millis();        // sauvegarde du temps du dernier changement de position
}

// *******************************************************************************************************
// ****************************************** lecture bouton *********************************************
// *******************************************************************************************************
void lecture_pot(int i)
{
    switch (i)
    {
    case 0:
#ifdef ENABLE_LEFT // si DEBUG LEFT activé bouton droit ignoré
        gain_0.update();
        if (gain_0.hasChanged())
        {
            last_change_time = millis(); // sauvegarde du temps du dernier changement de position
            position_change[i] = true;   // il y a changement de position du potentiomètre
            position_lue[i] = gain_0.getValue();
            position_lue[i] = map(position_lue[i], max_pot, min_pot, 0, 1023);
            position_lue[i] = constrain(position_lue[i], 0, 1023);
        }
#else
        position_lue[0] = 512;
#endif
        break;
    case 1:
#ifdef ENABLE_LEFT // si DEBUG LEFT activé bouton droit ignoré
        vol_0.update();
        if (vol_0.hasChanged())
        {
            last_change_time = millis(); // sauvegarde du temps du dernier changement de position
            position_change[i] = true;   // il y a changement de position du potentiomètre
            position_lue[i] = vol_0.getValue();
            position_lue[i] = map(position_lue[i], max_pot, min_pot, 0, 1023);
            position_lue[i] = constrain(position_lue[i], 0, 1023);
        }
#else
        position_lue[i] = 512;
#endif
        break;
    case 2:
#ifdef ENABLE_RIGHT // si DEBUG RIGHT activé bouton gauche ignoré
        gain_1.update();
        if (gain_1.hasChanged())
        {
            last_change_time = millis(); // sauvegarde du temps du dernier changement de position
            position_change[i] = true;   // il y a changement de position du potentiomètre
            position_lue[i] = gain_1.getValue();
            position_lue[i] = map(position_lue[i], max_pot, min_pot, 0, 1023);
            position_lue[i] = constrain(position_lue[i], 0, 1023);
        }
#else
        position_lue[i] = 512;
#endif
        break;
    case 3:
#ifdef ENABLE_RIGHT // si DEBUG RIGHT activé bouton gauche ignoré
        vol_1.update();
        if (vol_1.hasChanged())
        {
            last_change_time = millis(); // sauvegarde du temps du dernier changement de position
            position_change[i] = true;   // il y a changement de position du potentiomètre
            position_lue[i] = vol_1.getValue();
            position_lue[i] = map(position_lue[i], max_pot, min_pot, 0, 1023);
            position_lue[i] = constrain(position_lue[i], 0, 1023);
        }
#else
        position_lue[i] = 512;
#endif
        break;
    }
}
