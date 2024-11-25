/*******************************************************************************************************
 ****************************************** PIN MAPPING ************************************************
 ******************************************************************************************************/
// LED
#define const_out_L_led 5 // Digital OUT led const_out_L
#define const_out_R_led 6 // Digital OUT led const_out_R
#define stereo_link_led 7 // Digital OUT led stereo_link

// switch
#define const_out_sw_L 2 // Digital IN switch const_out_L
#define const_out_sw_R 3 // Digital IN switch const_out_R
#define stereo_link_sw 4 // Digital IN switch stereo_link

// PIN curseur potentiomètre
#define gain_0_pot A1 // entrée pot gain gauche
#define vol_0_pot A0  // entrée pot volume gauche
#define gain_1_pot A3 // entrée pot gain droite
#define vol_1_pot A2  // entrée pot volume droite

/*******************************************************************************************************
****************************************** potentiomètre ***********************************************
*******************************************************************************************************/
// variables potentiomètre
int position_lue[4] = {0, 0, 0, 0};                                  // tableau des valeur lus sur les potentiomètre
int valut_pot_old[4] = {0, 0, 0, 0};                                 // tableau de comparaison de valeur lus sur les potentiomètre
int position_save[4] = {0, 0, 0, 0};                                 // tableau de sauvegarde de valeurs des potentiomètre pour comparaison
int position_set[4] = {0, 0, 0, 0};                                  // tableau de consigne de position des potentiomètre
int position_memory[4] = {0, 0, 0, 0};                               // tableau de memorisation de valeur des potentiomètre pour mode constant output
bool position_change[4] = {false, false, false, false};              // tableau de flag de changement de position des potentiomètre
bool motor_change[4] = {false, false, false, false};                 // tableau de flag d'enclenchement des moteurs de potentiomètre
byte analog_pot[4] = {gain_0_pot, vol_0_pot, gain_1_pot, vol_1_pot}; // tableau de PIN des potentiomètre

// réglagle de précision de lecture du potentiomètre
/*
Max-val_pot  /  divider relais /  divider_analog
    255      /      1          /      4
    512      /      2          /      2
    1023     /      4          /      1
*/
#define bit_read_pot 1 // 4 = 8 bits, 2 = 9 bits, 1 = 10 bits

#if bit_read_pot == 4
#define divider_relais 1
#define divider_analog 4
#define max_val_pot 255 // valeur max du potentiomètre
#elif bit_read_pot == 2
#define divider_relais 2
#define divider_analog 2
#define max_val_pot 512 // valeur max du potentiomètre
#elif bit_read_pot == 1
#define divider_relais 4
#define divider_analog 1
#define max_val_pot 1023 // valeur max du potentiomètre
#endif

#define min_val_pot 0    // valeur min du potentiomètre
#define offset_max_pot 0 // valeur de suppression de la valeur max du potentiomètre (lecture à 0% de la course)
#define offset_min_pot 5 // valeur d'ajout de la valeur min du potentiomètre (lecture à 100% de la course)


unsigned long max_pot_time[4] = {0, 0, 0, 0};    // compteur de temps de maintien moteur pour la valeur max du potentiomètre
unsigned long last_pot_change[4] = {0, 0, 0, 0}; // compteur de temps de changement de valeur du potentiomètre
int count_pot_change[4] = {1, 1, 1, 1};          // compteur de changement de valeur du potentiomètre
// int max_pot[4] = {max_val_pot - offset_max_pot, max_val_pot - offset_max_pot, max_val_pot - offset_max_pot, max_val_pot - offset_max_pot}; // valeur max potentiomètre pour calibrage ADC
// int min_pot[4] = {min_val_pot + offset_min_pot, min_val_pot + offset_min_pot, min_val_pot + offset_min_pot, min_val_pot + offset_min_pot}; // valeur min potentiomètre pour calibrage ADC

int max_pot[4] = {1023, 1022, 1014, 1014}; // valeur max potentiomètre pour calibrage ADC
int min_pot[4] = {30, 29, 20, 21};         // valeur min potentiomètre pour calibrage ADC

// #define CALIBRATED

#ifdef CALIBRATED
#define value_middle_pot_0 109
#define value_middle_pot_1 117
#define value_middle_pot_2 130
#define value_middle_pot_3 134

int calibrating_midle_pot[4] = {value_middle_pot_0, value_middle_pot_1, value_middle_pot_2, value_middle_pot_3};
#else
const int middle_value = max_val_pot / 2; // valeur du milieu du potentiomètre
int calibrating_midle_pot[4] = {middle_value, middle_value, middle_value, middle_value};
#endif

#define bounce_time_pot 200 // interval de temps entre 2 lecture de l'état du potentiomètre en ms

/*******************************************************************************************************
****************************************** moteur ******************************************************
*******************************************************************************************************/
// adressage I2C carte moteur
/*
0xC8 : A0 = open, A1 = open
0xCE : A0 = open, A1 = 1
0xC2 : A0 = open, A1 = 0
0xC6 : A0 = 0, A1 = open
*/
#define gain_0_motor 0xC6
#define vol_0_motor 0xC8
#define gain_1_motor 0xC2
#define vol_1_motor 0xCE

int motor_speed[4];               // tableau de valeur de vitesse des moteur
unsigned long last_motor_end = 0; // compteur de temps d'arrêt moteur

/*******************************************************************************************************
 ****************************************** relais *****************************************************
********************************************************************************************************/
// adressage I2C carte relais
/*
0x39 : A0 = 0, A1 = 1, A2 = 1
0x38 : A0 = 1, A1 = 1, A2 = 1
0x3B : A0 = 0, A1 = 0, A2 = 1
0x3A : A0 = 1, A1 = 0, A2 = 1
*/
#define gain_0_relais 0x39
#define vol_0_relais 0x38
#define gain_1_relais 0x3B
#define vol_1_relais 0x3A

// variables relais
byte relais_gain_val[2] = {0, 0};  // tableau de valeur des relais gain
byte relais_vol_val[2] = {0, 0};   // tableau de valeur des relais volumes
byte relais_old[4] = {0, 0, 0, 0}; // tableau de comparaison de valeur des relais
byte relais_map[4] = {0, 0, 0, 0}; // tableau de mappage des valeur des relais
#define delay_relais 1             // temps de commutation entre relais pour éviter les clic audio

/*******************************************************************************************************
 ****************************************** boutton ****************************************************
*******************************************************************************************************/
bool const_out_L_state_old = false; // variable de comparaison d'état du bouton const_out_L
bool const_out_R_state_old = false; // variable de comparaison d'état du bouton const_out_R
bool stereo_link_state_old = false; // variable de comparaison d'état du bouton stereo_link
bool const_out_L_state = false;     // variable d'état du bouton const_out_L
bool const_out_R_state = false;     // variable d'état du bouton const_out_R
bool stereo_link_state = false;     // variable d'état du bouton stereo_link
bool state_button_change = false;   // variable de changement d'état des boutons
#define interval_button 10          // interval de temps entre 2 lecture de l'état du bouton

/*******************************************************************************************************
 ****************************************** variable diverse *******************************************
 ******************************************************************************************************/
int smoothgain[2] = {0, 0}; // tableau de valeur de lissage du gain
int smoothvol[2] = {0, 0};  // tableau de valeur de lissage du volume
// bool const_state = false;                                // variable d'état constant ouptut/stereo_link
// bool error_motor = false;                                // variable d'erreur de communication avec le moteur

/*******************************************************************************************************
 ****************************************** fonction erreur LED  ***************************************
*******************************************************************************************************/
bool error_state = false;
bool error_led_state = LOW;
int blink_speed_led = 250;
unsigned long old_led_time = millis(); // variable de temps courant

void error_led() // fonction clignotement LED en cas d'erreur
{
    unsigned long current_led_time = millis(); // variable de temps courant
    if (current_led_time - old_led_time >= blink_speed_led)
    {
        old_led_time = millis();
        error_led_state = !error_led_state;
        digitalWrite(stereo_link_led, error_led_state);
        digitalWrite(const_out_L_led, !error_led_state);
        digitalWrite(const_out_R_led, !error_led_state);
    }
}