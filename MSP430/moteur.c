#include "moteur.h"

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(){
	if(distance_to_do){
		if(distance_to_do <0){
			PORT_MOTEUR_DIR &= ~PIN_DIRRECTION_MOTEUR_DROITE;
			PORT_MOTEUR_DIR &= ~PIN_DIRRECTION_MOTEUR_GAUCHE;
		}
		else{
			PORT_MOTEUR_DIR |= PIN_DIRRECTION_MOTEUR_DROITE;
			PORT_MOTEUR_DIR |= PIN_DIRRECTION_MOTEUR_GAUCHE;
		}
		PORT_MOTEUR_OUT ^= PIN_MOTEUR_DROITE;
		PORT_MOTEUR_OUT ^= PIN_MOTEUR_GAUCHE;
		__delay_cycles(TIME_TO_WAIT);
		PORT_MOTEUR_OUT ^= PIN_MOTEUR_DROITE;
		PORT_MOTEUR_OUT ^= PIN_MOTEUR_GAUCHE;
		cran_avance++;
		if(cran_avance >= distance_to_do){
			//on appelle la fonction uart pour dire que on a fini !
			cran_avance = 0;
			distance_to_do = 0;
		}
	}
	if(angle_to_do){
		
	}
}


void avancer(char distance){
        distance_to_do = distance;
        cran_avance = 0;//we normaly don’t need to do it, it should already have this value !
}

void tourner(char angle_sur_3_bits){
        angle_to_do = tab_angle[angle_sur_3_bits];
        cran_tourne = 0;//we normaly don’t need to do it, it should already have this value !
}
