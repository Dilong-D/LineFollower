/*
 * linefollower
 *
 * Created: 2017.07.21
 * Author : Dominik Markowski, Jan Zyczkowski
 */ 

#define  F_CPU    32000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include "config.h"
#include "bluetooth.h"

/*
Opis portów
port	funkcja		
D0	-	PWM	MOTOR R
D1	-	INPUT MOTOR 2 R
D2	-	INPUT MOTOR 1 R
D3	-	PWM	MOTOR L
D4	-	INPUT 2 MOTOR L
D5	-	INPUT 1	MOTOR L
C0	-	enkoder MOTOR L
C1	-	enkoder	MOTOR L
F0	-	enkoder MOTOR R
F1	-	enkoder	MOTOR R
E4,E5,E6,E7	-	dioda IR
B0-B4	-	fototranzystory
C3-C7-	lcd
*/


//Przerwanie przycisk dol
ISR(PORTF_INT0_vect){
}
//Przerwanie przycisk gora
ISR(PORTF_INT1_vect){	

}
//przerwanie pomiar baterii
ISR(ACA_AC0_vect){
	PORTF_OUTTGL=PIN7_bm;//zmiena stanu diody gdy bateria jest poni¿ej poziomu dopuszczalnego
}

ISR(TCD1_OVF_vect){
}

ISR(TCC1_OVF_vect){	
}

ISR(OSC_OSCF_vect) {									// przerwanie w razie awarii oscylatora
	OSC.XOSCFAIL	|=	OSC_XOSCFDIF_bm;				// kasowanie flagi przerwania
	
}







int main(void) {
	setall();
	
}
