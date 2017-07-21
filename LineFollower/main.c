/*
 * LineFollower.c
 *
 * Created: 21.07.2017 17:27:31
 * Author : dmark
 */ 
#include <avr/io.h>
#include <util/delay.h>

int main(void) {
	
	PORTE.DIR = 0;                  // caly port E jako wejœcie
	PORTF.DIR = 0b11111111;         // caly port F jako wyjœcie
	
	uint8_t licznik = 0;            // zmienna steruj¹ca mrugaj¹cymi diodami
	
	while(1) {
		licznik++;
		PORTF.OUT = licznik;
		if(PORTE.IN & (1<<5)) {     // jeœli przycisk FLIP zwolniony
			_delay_ms(100);
			} else {                    // jeœli przycisk FLIP wciœniêty
			_delay_ms(50);
		}
	}
}

