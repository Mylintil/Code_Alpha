#include <msp430.h>
#include "uart.h"

#define WEEL_SIZE 800 //in milimeter
#define DISTANCE_BETWEEN_WEEL 100//in milimeter

#define PIN_MOTEUR_DROITE BIT3
#define PIN_MOTEUR_GAUCHE BIT0
#define TIME_TO_WAIT 5//nombre de cycle à attendre pour que le moteur détecte bien le front montant
#define DIAMETRE 3.14159*2*DISTANCE_BETWEEN_WEEL
#define TAILLE_PAS 0.05 //distance parcourut par la roue en un pas (en mm)
#define BIT_DIRRECTION_MOTEUR_DROITE BIT4
#define BIT_DIRRECTION_MOTEUR_GAUCHE BIT1
#define PORT_MOTEUR_OUT P2OUT

signed short tab_angle[] = {0, (DIAMETRE/(TAILLE_PAS*8)), (DIAMETRE/(TAILLE_PAS*4)), (3*DIAMETRE/(TAILLE_PAS*8)), (DIAMETRE/(TAILLE_PAS*2)), (-DIAMETRE/(TAILLE_PAS*8)), (-DIAMETRE/(TAILLE_PAS*4)), -(3*DIAMETRE/(TAILLE_PAS*8))};
/*****TRANSCODAGE TABLE*******
 *  UART		degre*
 *			     *
 *  000	 ----------- >	  0° *
 *  001	 ----------- >	 45° *
 *  010	 ----------- >	 90° *
 *  011	 ----------- >	135° *
 *  100	 ----------- >	180° *
 *  101	 ----------- >	-45° *
 *  110	 ----------- >	-90° *
 *  111	 ----------- >	-135°*
 *****************************/

unsigned short cran_avance = 0;//compteur; représente le nombre de front montant effectuer depuis le dernier ordre UART avancer
unsigned short cran_tourne = 0;//compteur; représente le nombre de front montant effectuer depuis le dernier ordre UART tourner 
unsigned short distance = 0;//distance à parcourir
unsigned short angle = 0; //angle à parcourir


//#pragma vector = TIMER0_A0_VECTOR
__interrupt void interupt_moteur(){

void avancer(char distance);
void tourner(char angle_sur_3_bits);
