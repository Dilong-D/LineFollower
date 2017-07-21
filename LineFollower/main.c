/*
 * LineFollower.c
 *
 * Created: 21.07.2017 17:27:31
 * Author : dmark
 */ 
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	
	PORTE.DIR = 0;                  // caly port E jako wej�cie
	PORTF.DIR = 0b11111111;         // caly port F jako wyj�cie
	
	uint8_t licznik = 0;            // zmienna steruj�ca mrugaj�cymi diodami
	
	while(1) {
		licznik++;
		PORTF.OUT = licznik;
		if(PORTE.IN & (1<<5)) {     // je�li przycisk FLIP zwolniony
			_delay_ms(100);
			} else {                    // je�li przycisk FLIP wci�ni�ty
			_delay_ms(50);
		}
	}
}
