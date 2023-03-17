// *******************************************************************************************************
// ****************************************** gestion bouttons ********************************************
// *******************************************************************************************************

// Include the Bounce2 library found here :
// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

// création instance bouton
Bounce2::Button const_out_L = Bounce2::Button(); // instance du bouton const_out_L
Bounce2::Button const_out_R = Bounce2::Button(); // instance du bouton const_out_R
Bounce2::Button stereo_link = Bounce2::Button(); // instance du bouton stereo_link

void lecture_switch();  // lecture bouton
void bouton_set();      // gestion des boutons
void stereo_link_set(); // gestion du bouton stereo_link
void const_out_L_set(); // gestion du bouton const_out_L
void const_out_R_set(); // gestion du bouton const_out_R

void lecture_switch()
{
    // lecture des boutons
    const_out_L.update();
    const_out_R.update();
    stereo_link.update();
    // mise à jour des états des boutons
    if (const_out_L.pressed())
    {
        const_out_L_state = !const_out_L_state;
    }
    if (const_out_R.pressed())
    {
        const_out_R_state = !const_out_R_state;
    }
    if (stereo_link.pressed())
    {
        stereo_link_state = !stereo_link_state;
    }
}

void bouton_set()
{
    lecture_switch();
    if (stereo_link_state_old != stereo_link_state) // si changement d'état du bouton stereo_link
    {
        stereo_link_set();                                          // gestion du bouton stereo_link
        if (const_out_L_state == true || const_out_R_state == true) // si bouton const_out_L ou R appuyé
        {
            const_out_L_state = true;
            const_out_R_state = true;
        }
        state_button_change = true;                // il y a changement d'état d'un bouton
        stereo_link_state_old = stereo_link_state; // sauvegarde état bouton stereo_link
    }
    if (const_out_L_state_old != const_out_L_state) // si changement d'état du bouton const_out_L
    {
        const_out_L_set();                         // gestion du bouton const_out_L
        state_button_change = true;                // il y a changement d'état d'un bouton
        const_out_L_state_old = const_out_L_state; // sauvegarde état bouton const_out_L
    }
    if (const_out_R_state_old != const_out_R_state) // si changement d'état du bouton const_out_R
    {
        const_out_R_set();                         // gestion du bouton const_out_R
        state_button_change = true;                // il y a changement d'état d'un bouton
        const_out_R_state_old = const_out_R_state; // sauvegarde état bouton const_out_R
    }
}

void stereo_link_set() // gestion du bouton stereo_link
{
    if (stereo_link_state == true) // si bouton stereo_link appuyé
    {
        digitalWrite(stereo_link_led, HIGH); // allume LED stereo_link
    }
    else // si bouton stereo_link relaché
    {
        digitalWrite(stereo_link_led, LOW); // éteint LED stereo_link
        for (int i = 0; i <= 3; i++)        // pour chaque moteur
        {
            motor[i].brake();                     // frein moteur
            motor[i].stop();                      // arrêt moteur
            position_memory[i] = position_lue[i]; // sauvegarde position moteur
        }
    }
}

void const_out_L_set() // gestion du bouton const_out_L
{
    if (const_out_L_state == true) // si bouton const_out_L appuyé
    {
        digitalWrite(const_out_L_led, HIGH); // allume LED const_out_L
    }
    else if (stereo_link_state != true) // si bouton const_out_L relaché et pas de stereo_link
    {
        digitalWrite(const_out_L_led, LOW); // éteint LED const_out_L
        for (int i = 0; i <= 1; i++)        // pour chaque moteur
        {
            motor[i].brake();                     // frein moteur
            motor[i].stop();                      // arrêt moteur
            position_memory[i] = position_lue[i]; // sauvegarde position moteur
        }
    }
}

void const_out_R_set() // gestion du bouton const_out_R
{
    if (const_out_R_state == true) // si bouton const_out_R appuyé
    {
        digitalWrite(const_out_R_led, HIGH); // allume LED const_out_R
    }
    else if (stereo_link_state != true) // si bouton const_out_L relaché et pas de stereo_link
    {
        digitalWrite(const_out_R_led, LOW); // éteint LED const_out_R
        for (int i = 2; i <= 3; i++)        // pour chaque moteur
        {
            motor[i].brake();                     // frein moteur
            motor[i].stop();                      // arrêt moteur
            position_memory[i] = position_lue[i]; // sauvegarde position moteur
        }
    }
}