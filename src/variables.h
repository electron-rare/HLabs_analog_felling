// *******************************************************************************************************
// ****************************************** PIN MAPPING ************************************************
// *******************************************************************************************************
// définition LED
#define const_out_L_led 5 // Digital OUT led const_out_L
#define const_out_R_led 6 // Digital OUT led const_out_R
#define stereo_link_led 7 // Digital OUT led stereo_link
// définition switch
#define const_out_sw_L 2 // Digital IN switch const_out_L
#define const_out_sw_R 3 // Digital IN switch const_out_R
#define stereo_link_sw 4 // Digital IN switch stereo_link
// définition PIN curseur potentiomètre
#define gain_0_pot A1 // entrée pot gain gauche
#define vol_0_pot A0  // entrée pot volume gauche
#define gain_1_pot A2 // entrée pot gain droite
#define vol_1_pot A3  // entrée pot volume droite

// *******************************************************************************************************
// ****************************************** potentiomètre **********************************************
// *******************************************************************************************************
// adresse moteur
#define gain_0_motor 0xC8 // A0 = open, A1 = 1
#define vol_0_motor 0xCE  // A0 = open, A1 = open
#define gain_1_motor 0xD0 // A0 = 1, A1 = open
#define vol_1_motor 0xCA  // A0 = 1, A1 = 1

// variables potentiomètre
int position_lue[4] = {0, 0, 0, 0};                                  // tableau des valeur lus sur les potentiomètre
int position_save[4] = {0, 0, 0, 0};                                 // tableau de sauvegarde de valeurs des potentiomètre pour comparaison
int position_set[4] = {0, 0, 0, 0};                                  // tableau de consigne de position des potentiomètre
int position_memory[4] = {0, 0, 0, 0};                               // tableau de memorisation de valeur des potentiomètre pour mode constant output
bool position_change[4] = {false, false, false, false};              // tableau de flag de changement de position des potentiomètre
bool motor_change[4] = {false, false, false, false};                 // tableau de flag pour moteurs des potentiomètre
byte analog_pot[4] = {gain_0_pot, vol_0_pot, gain_1_pot, vol_1_pot}; // tableau de PIN des potentiomètre
#define max_pot 1000                                                 // valeur max potentiomètre pour calibrage ADC
#define min_pot 10                                                    // valeur min potentiomètre pour calibrage ADC
// variable pour gestion du temps
unsigned long last_change_time = millis(); // remise à zéro du compteur de temps
unsigned long current_time = millis();     // variable de temps courant

#define bounce_time_pot 150 // interval de temps entre 2 lecture de l'état du potentiomètre

// *******************************************************************************************************
// ****************************************** relais *****************************************************
// *******************************************************************************************************
// adresse carte relais
#define gain_0_relais 0x39 // A0 = 0, A1 = 1, A2 = 1
#define vol_0_relais 0x38  // A0 = 1, A1 = 1, A2 = 1
#define gain_1_relais 0x00
#define vol_1_relais 0x00
// variables relais
byte relais_gain_val[2] = {0, 0};  // tableau de valeur des relais gain
byte relais_vol_val[2] = {0, 0};   // tableau de valeur des relais volumes
byte relais_old[4] = {0, 0, 0, 0}; // tableau de comparaison de valeur des relais
byte relais_map[4] = {0, 0, 0, 0}; // tableau de mappage des valeur des relais
#define delay_relais 1             // temps de commutation entre relais

// *******************************************************************************************************
// ****************************************** boutton *****************************************************
// *******************************************************************************************************
bool const_out_L_state_old = false; // variable de comparaison d'état du bouton const_out_L
bool const_out_R_state_old = false; // variable de comparaison d'état du bouton const_out_R
bool stereo_link_state_old = false; // variable de comparaison d'état du bouton stereo_link
bool const_out_L_state = false;     // variable d'état du bouton const_out_L
bool const_out_R_state = false;     // variable d'état du bouton const_out_R
bool stereo_link_state = false;     // variable d'état du bouton stereo_link
#define interval_button 10          // interval de temps entre 2 lecture de l'état du bouton

// *******************************************************************************************************
// ****************************************** variable diverse *******************************************
// *******************************************************************************************************
bool state_pot_change[4] = {false, false, false, false}; // variable de changement d'état
bool state_button_change = true;                         // variable de changement d'état
bool const_state = false;                                // variable d'état d'activtion d'un constant ouptut
bool error_motor = false;                                // variable d'erreur de communication avec le moteur
int smoothgain[2] = {0, 0};                              // tableau de valeur de lissage du gain
int smoothvol[2] = {0, 0};                               // tableau de valeur de lissage du volume
int motor_speed[4] = {0, 0, 0, 0};                       // variable de vitesse de rotation du moteur

// *******************************************************************************************************
// ****************************************** fonction error LED  ****************************************
// *******************************************************************************************************
bool error_state = false;
bool error_led_state = LOW;

unsigned long old_led_time = millis(); // variable de temps courant

void error_led()
{
    unsigned long current_led_time = millis(); // variable de temps courant
    if (current_led_time - old_led_time >= 500)
    {
        old_led_time = millis();
        if (error_led_state == LOW)
        {
            error_led_state = HIGH;
        }
        else
        {
            error_led_state = LOW;
        }
        digitalWrite(stereo_link_led, error_led_state);
    }
}