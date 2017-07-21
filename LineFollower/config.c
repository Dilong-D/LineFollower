#define  F_CPU    32000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stddef.h>
#include "config.h"
#include "bluetooth.h"

void OscXtal(void) {
	// konfiguracja generatora kwarcowego
	OSC.XOSCCTRL	=	OSC_FRQRANGE_12TO16_gc |		// wyb�r kwarcu od 12 do 16 MHZ
	OSC_XOSCSEL_XTAL_16KCLK_gc;		// czas na uruchomienie generatora
	OSC.CTRL		=	OSC_XOSCEN_bm;					// uruchomienie generatora kwarcowego
	// czekanie na ustabilizowanie si� generatora
	for(uint8_t i=0; i<255; i++) {
		if(OSC.STATUS & OSC_XOSCRDY_bm) {
			_delay_ms(20);
			CPU_CCP			=	CCP_IOREG_gc;			// odblokowanie zmiany �r�d�a sygna�u zegarowego
			CLK.CTRL		=	CLK_SCLKSEL_XOSC_gc;	// wyb�r �r�d�a sygna�u zegarowego na XTAL 16MHz
			// uk�ad nadzoruj�cy kwarc
			CPU_CCP			=	CCP_IOREG_gc;			// odblokowanie modyfikacji wa�nych rejestr�w
			OSC.XOSCFAIL	=	OSC_XOSCFDEN_bm;		// w��czenie uk�adu detekcji b��du sygna�u zegarowego
			
			return;										// wyj�cie z funkcji je�li generator si� uruchomi�
		}
		_delay_us(10);
	}
}
void Osc32MHz(void) {
	OSC.CTRL     =    OSC_RC32MEN_bm;       // w��czenie oscylatora 32MHz
	while(!(OSC.STATUS & OSC_RC32MRDY_bm)); // czekanie na ustabilizowanie si� generatora
	CPU_CCP      =    CCP_IOREG_gc;         // odblokowanie zmiany �r�d�a sygna�u
	CLK.CTRL     =    CLK_SCLKSEL_RC32M_gc; // zmiana �r�d�a sygna�u na RC 32MHz
	_delay_ms(1000);
}

void setMotorL(void){
	//------------ustawienia silnika 1
	PORTD.DIRSET	=	PIN5_bm|//inpu2
	PIN4_bm|//input1
	PIN3_bm;//pwm
	PORTD.OUTCLR	=	PIN5_bm;//input 1->0
	PORTD.OUTCLR	=	PIN4_bm;//input 2->0
	TCD0.CTRLB		=	TC_WGMODE_SINGLESLOPE_gc|		// pwm singleslope
	TC0_CCDEN_bm|
	TC0_CCAEN_bm;
	TCD0.PER		=	500;
	TCD0.CCD		=	0;
	TCD0.CTRLA		=	TC_CLKSEL_DIV1_gc;
}
void setMotorR(void){
	//------------ustawienia silnika 2
	PORTD.DIRSET	=	PIN2_bm|//inpu2
	PIN1_bm|//input1
	PIN0_bm;//pwm
	PORTD.OUTCLR	=	PIN1_bm;//input 1->0
	PORTD.OUTCLR	=	PIN2_bm;//input 2->0
	TCD0.CCA		=	0;
}
void motorR(int8_t o, int8_t k){ //kierowanie silnikiem prawym
	TCD0.CCD		=	5*o;
	if(k==LUZ){
		PORTD.OUTCLR	=	PIN5_bm;//input 1->0
		PORTD.OUTCLR	=	PIN4_bm;//input 2->0
	}
	
	else if(k==STOP){
		PORTD.OUTSET	=	PIN5_bm;//input 1->1
		PORTD.OUTSET	=	PIN4_bm;//input 2->1
	}
	else if(k==PRAWO){
		PORTD.OUTCLR	=	PIN5_bm;//input 1->0
		PORTD.OUTSET	=	PIN4_bm;//input 2->1
	}
	else{
		PORTD.OUTSET	=	PIN5_bm;//input 1->1
		PORTD.OUTCLR	=	PIN4_bm;//input 2->0
	}
}
void motorL(int8_t o, int8_t k){ //kierowanie silnikiem lewym
	TCD0.CCA		=	5*o;
	if(k==LUZ){
		PORTD.OUTCLR	=	PIN1_bm;//input 1->0
		PORTD.OUTCLR	=	PIN2_bm;//input 2->0
	}
	else if(k==STOP){
		PORTD.OUTSET	=	PIN1_bm;//input 1->1
		PORTD.OUTSET	=	PIN2_bm;//input 2->1
	}
	else if(k==PRAWO){
		PORTD.OUTCLR	=	PIN1_bm;//input 1->0
		PORTD.OUTSET	=	PIN2_bm;//input 2->1
	}
	else{
		PORTD.OUTSET	=	PIN1_bm;//input 1->1
		PORTD.OUTCLR	=	PIN2_bm;//input 2->0
	}
}
void setall(void){
	// ============================		kwarc		===========================================================================================
	Osc32MHz();//ustawienie kwarcu 32MHz
	sei();//ustawienie przerwan
	// ============================		wejscia		===========================================================================================
	
	//-----------------------------		przyciski	-----------------------------------------------------------------------
	PORTF.DIRCLR		=	PIN2_bm|PIN3_bm;					// pin F2 F3 jako wej�cie
	PORTF.INT0MASK		=   PIN2_bm;               // pin F2 ma generowa� przerwania INT0
	PORTF.INT1MASK		=   PIN3_bm;
	PORTF.PIN2CTRL		=   PORT_OPC_PULLUP_gc|    // pull-up na F2
	PORT_ISC_FALLING_gc;   // przerwanie wywo�uje zbocze opadaj�ce
	PORTF.PIN3CTRL		=   PORT_OPC_PULLUP_gc|    // pull-up na F3
	PORT_ISC_FALLING_gc;   // przerwanie wywo�uje zbocze opadaj�ce
	PORTF.INTCTRL		=   PORT_INT0LVL_LO_gc| PORT_INT1LVL_LO_gc;   // poziom LO dla przerwania INT0 portu F2 F3
	//-------------------------------	enkodera R	------------------------------------------------------
	PORTC.PIN0CTRL		=	PORT_ISC_LEVEL_gc | PORT_OPC_PULLUP_gc;
	PORTC.PIN1CTRL		=	PORT_ISC_LEVEL_gc | PORT_OPC_PULLUP_gc;
	EVSYS.CH0MUX		=   EVSYS_CHMUX_PORTC_PIN0_gc;        // pin C0 wywo�uje zdarzenie
	EVSYS.CH0CTRL		=   EVSYS_QDEN_bm|                    // w��czenie dekodera w systemie zdarze�
	EVSYS_DIGFILT_8SAMPLES_gc;        // filtr cyfrowy
	TCC0.CTRLA			=   TC_CLKSEL_EVCH0_gc;               // taktowanie systemem zdarze�
	TCC0.CTRLD			=   TC_EVACT_QDEC_gc |                // w��czenie dekodera kwadraturowego
							TC_EVSEL_CH0_gc;                  // dekoder zlicza impulsy z kana�u 0*/
	//-------------------------------	enkodera L	------------------------------------------------------
	PORTF.PIN0CTRL		=	PORT_ISC_LEVEL_gc | PORT_OPC_PULLUP_gc;
	PORTF.PIN1CTRL		=	PORT_ISC_LEVEL_gc | PORT_OPC_PULLUP_gc;
	EVSYS.CH2MUX		=   EVSYS_CHMUX_PORTF_PIN0_gc;        // pin E0 wywo�uje zdarzenie
	EVSYS.CH2CTRL		=   EVSYS_QDEN_bm|                    // w��czenie dekodera w systemie zdarze�
	EVSYS_DIGFILT_8SAMPLES_gc;        // filtr cyfrowy
	TCF0.CTRLA			=   TC_CLKSEL_EVCH2_gc;               // taktowanie systemem zdarze�
	TCF0.CTRLD			=   TC_EVACT_QDEC_gc |                // w��czenie dekodera kwadraturowego
							TC_EVSEL_CH2_gc;                  // dekoder zlicza impulsy z kana�u 0*/
	//==============================	wyjscia		=========================================================
	setMotorL();
	setMotorR();
	//-----------------------------------diody IR--------------------------------------
	
	//-----------------------------------diody LED--------------------------------------
	PORTF.DIRSET		=	PIN5_bm|
	PIN6_bm|
	PIN7_bm;
	PORTF.OUTCLR		=	PIN5_bm|
	PIN6_bm|
	PIN7_bm;
	// ----------------------------		LCD		------------------------------
	
	setbat();
	bluetooth_init();
	
	//==============================	timery		=========================================================
	TCD1.INTCTRLA     =    TC_OVFINTLVL_LO_gc;         // przepe�nienie ma generowa� przerwanie LO
	TCD1.CTRLB        =    TC_WGMODE_NORMAL_gc;        // tryb normalny
	TCD1.PER = 156*5;
	
	TCC1.INTCTRLA     =    TC_OVFINTLVL_HI_gc;         // przepe�nienie ma generowa� przerwanie LO
	TCC1.CTRLB        =    TC_WGMODE_NORMAL_gc;        // tryb normalny
	TCC1.PER = 156;
	
	TCD1.CTRLA        =    TC_CLKSEL_DIV1024_gc;
	TCC1.CTRLA        =    TC_CLKSEL_DIV1024_gc;
	sei();
	
	L_ENKODER=0;
	R_ENKODER=0;

	
	}
	void ledYellow(void){
	PORTF_OUTTGL=PIN5_bm;
	}
	void ledGreen(void){
	PORTF_OUTTGL=PIN6_bm;
	}
	
	void setbat(void){//funkcja ustawiajaca przerwanie na za niski poziom baterii
	// konfiguracja komparatora 0 w porcie A
	PORTF_OUTSET=PIN7_bm;
	ACA.AC0MUXCTRL		=	AC_MUXPOS_PIN2_gc |
	AC_MUXNEG_SCALER_gc;    // wej�cie + PIN A6
	ACA.AC0CTRL			=	AC_ENABLE_bm|AC_HYSMODE_SMALL_gc|AC_INTLVL_LO_gc|AC_INTMODE_FALLING_gc;
	ACA.CTRLB			=	45;                    // pocz�tkowe ustawienie dzielnika napi�cia
	ACA.CTRLA			=	AC_AC0OUT_bm;
	}