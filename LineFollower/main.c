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
A2		-	POZIOM BATERII			-		ZROBIONE
A3-A6	-	ODCZYT FOTOTRANZYSTOROW	-		NIE ZROBIONE
B4-B7	-	ODCZYT FOTOTRANZYSTOROW -		NIEZROBIONE
C0-C1	-	ENKODER MOTOR L			-		ZROBIONE
C2-C3	-	BLUETOOTH				-		ZROBIONE
D0		-	PWM	MOTOR R				-		ZROBIONE
D1		-	INPUT MOTOR 2 R			-		ZROBIONE
D2		-	INPUT MOTOR 1 R			-		ZROBIONE
D3		-	PWM	MOTOR L				-		ZROBIONE
D4		-	INPUT 2 MOTOR L			-		ZROBIONE
D5		-	INPUT 1	MOTOR L			-		ZROBIONE
F0-F1	-	ENKODER MOTOR R			-		ZROBIONE
F2-F3	-	PRZYCISKI				-		ZROBIONE
F5-F7	-	DIODY LED				-		ZROBIONE
*/


//Przerwanie przycisk
ISR(PORTF_INT0_vect){
}
//Przerwanie przycisk
ISR(PORTF_INT1_vect){	
}
//przerwanie pomiar baterii
ISR(ACA_AC0_vect){
	PORTF_OUTCLR=PIN7_bm;//zmiena stanu diody gdy bateria jest poni¿ej poziomu dopuszczalnego
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
