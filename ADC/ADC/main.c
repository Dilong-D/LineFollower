/*
 * ADC.c
 *
 * Created: 2017-08-02 13:58:14
 * Author : Marcin
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int czujnik[6]={0,0,0,0,0,0};
volatile int czujnik_int[6]={0,0,0,0,0,0};
volatile int prog=150;

int main(void)
{
	//IO configuration
	PORTA.DIR=~(PIN3_bm|PIN4_bm|PIN5_bm|PIN6_bm);						//Input from sensors
	PORTB.DIR=~(PIN4_bm|PIN5_bm|PIN6_bm|PIN7_bm);

	//Clock source configuration
	OscXtal();															//Configure clock signal

	//ADC
	//CLK.PSCTRL=CLK_PSADIV0_bm;										//Clock prescaler - no division
	ADCA.CTRLA=ADC_ENABLE_bm;											//ADCA - Enable
	ADCB.CTRLA=ADC_ENABLE_bm;											//ADCB - Enable
	ADCA.PRESCALER=ADC_PRESCALER2_bm;									//ADCA - Frequency devided by 64 (8MHz/64)
	ADCB.PRESCALER=ADC_PRESCALER2_bm;									//ADCB - Frequency devided by 64 (8MHz/64)
	ADCA.CTRLB=ADC_RESOLUTION_8BIT_gc|ADC_FREERUN_bm;					//ADCA - 8 bit resolution and freerun mode
	ADCB.CTRLB=ADC_RESOLUTION_8BIT_gc|ADC_FREERUN_bm;					//ADCB - 8 bit resolution and freerun mode
	ADCA.INTFLAGS=ADC_CH0IF_bm|ADC_CH1IF_bm|ADC_CH2IF_bm|ADC_CH3IF_bm;	//ADCA - Clear all interrupts
	ADCB.INTFLAGS=ADC_CH0IF_bm|ADC_CH1IF_bm|ADC_CH2IF_bm|ADC_CH3IF_bm;	//ADCB - Clear all interrupts
	ADCA.EVCTRL=ADC_SWEEP_0123_gc;										//ADCA - Channels 0, 1, 2, 3 in freerun mode
	ADCB.EVCTRL=ADC_SWEEP_0123_gc;										//ADCB - Channels 0, 1, 2, 3 in freerun mode
	ADCA.CH0.CTRL=ADC_CH_INPUTMODE_SINGLEENDED_gc;						//ADCA.CH0 - Single-ended mode
	ADCA.CH1.CTRL=ADC_CH_INPUTMODE_SINGLEENDED_gc;						//ADCA.CH1 - Single-ended mode
	ADCA.CH2.CTRL=ADC_CH_INPUTMODE_SINGLEENDED_gc;						//ADCA.CH2 - Single-ended mode
	ADCA.CH3.CTRL=ADC_CH_INPUTMODE_SINGLEENDED_gc;						//ADCA.CH3 - Single-ended mode
	ADCB.CH0.CTRL=ADC_CH_INPUTMODE_SINGLEENDED_gc;						//ADCB.CH0 - Single-ended mode
	ADCB.CH1.CTRL=ADC_CH_INPUTMODE_SINGLEENDED_gc;						//ADCB.CH1 - Single-ended mode
	ADCB.CH2.CTRL=ADC_CH_INPUTMODE_SINGLEENDED_gc;						//ADCB.CH2 - Single-ended mode
	ADCB.CH3.CTRL=ADC_CH_INPUTMODE_SINGLEENDED_gc;						//ADCB.CH3 - Single-ended mode
	ADCA.CH0.INTCTRL=ADC_CH_INTLVL0_bm;									//ADCA.CH0 - Low priority interrupt when conversion complete
	ADCA.CH1.INTCTRL=ADC_CH_INTLVL0_bm;									//ADCA.CH1 - Low priority interrupt when conversion complete
	ADCA.CH2.INTCTRL=ADC_CH_INTLVL0_bm;									//ADCA.CH2 - Low priority interrupt when conversion complete
	ADCA.CH3.INTCTRL=ADC_CH_INTLVL0_bm;									//ADCA.CH3 - Low priority interrupt when conversion complete
	ADCB.CH0.INTCTRL=ADC_CH_INTLVL0_bm;									//ADCB.CH0 - Low priority interrupt when conversion complete
	ADCB.CH1.INTCTRL=ADC_CH_INTLVL0_bm;									//ADCB.CH1 - Low priority interrupt when conversion complete
	ADCB.CH2.INTCTRL=ADC_CH_INTLVL0_bm;									//ADCB.CH2 - Low priority interrupt when conversion complete
	ADCB.CH3.INTCTRL=ADC_CH_INTLVL0_bm;									//ADCB.CH3 - Low priority interrupt when conversion complete
	ADCA.CH0.MUXCTRL=ADC_CH_MUXPOS_PIN3_gc;								//ADCA.CH0 - Connect to PA3
	ADCA.CH1.MUXCTRL=ADC_CH_MUXPOS_PIN4_gc;								//ADCA.CH1 - Connect to PA4
	ADCA.CH2.MUXCTRL=ADC_CH_MUXPOS_PIN5_gc;								//ADCA.CH2 - Connect to PA5
	ADCA.CH3.MUXCTRL=ADC_CH_MUXPOS_PIN6_gc;								//ADCA.CH3 - Connect to PA6
	ADCB.CH0.MUXCTRL=ADC_CH_MUXPOS_PIN4_gc;								//ADCB.CH0 - Connect to PB4
	ADCB.CH1.MUXCTRL=ADC_CH_MUXPOS_PIN5_gc;								//ADCB.CH1 - Connect to PB5
	ADCB.CH2.MUXCTRL=ADC_CH_MUXPOS_PIN6_gc;								//ADCB.CH2 - Connect to PB6
	ADCB.CH3.MUXCTRL=ADC_CH_MUXPOS_PIN7_gc;								//ADCB.CH3 - Connect to PB7
	ADCB.CH0.CTRL|=ADC_CH_START_bm;										//Start conversion
	//Comments to ADC:
	//We need to control ADCB.CH0-3.MUXCTRL(=ADC_CH_MUXPOS_PINx_gc;)
	//ADCB.CHx.RESL is result of conversion
	//ADCB.CHx.CTRL|=ADC_CH_START_bm; start conversion on channel (not in free-run)
	
	//Enable interrupts
	PMIC.CTRL=PMIC_LOLVLEN_bm;											//Enable low level interrupts
	sei();																//Enable all interrupts

	//Main loop
    while (1)
    {
		//Here results should be sent over BT to check correctness.
    }
}

ISR(ADCA_CH0_vect)														//ADCA.CH0 - Interrupt
{
	czujnik_int[0]=ADCA.CH0.RESL;
	czujnik[0]=(czujnik_int[0]>prog);									//Check whether is above line
}

ISR(ADCA_CH1_vect)														//ADCA.CH1 - Interrupt
{
	czujnik_int[1]=ADCA.CH0.RESL;
	czujnik[1]=(czujnik_int[1]>prog);									//Check whether is above line
}

ISR(ADCA_CH2_vect)														//ADCA.CH2 - Interrupt
{
	czujnik_int[2]=ADCA.CH0.RESL;
	czujnik[2]=(czujnik_int[2]>prog);									//Check whether is above line
}

ISR(ADCA_CH3_vect)														//ADCA.CH3 - Interrupt
{
	czujnik_int[3]=ADCA.CH0.RESL;
	czujnik[3]=(czujnik_int[3]>prog);									//Check whether is above line
}

ISR(ADCB_CH0_vect)														//ADCB.CH0 - Interrupt
{
	czujnik_int[4]=ADCA.CH0.RESL;
	czujnik[4]=(czujnik_int[4]>prog);									//Check whether is above line
}

ISR(ADCB_CH1_vect)														//ADCB.CH1 - Interrupt
{
	czujnik_int[5]=ADCA.CH0.RESL;
	czujnik[5]=(czujnik_int[5]>prog);									//Check whether is above line
}

ISR(ADCB_CH2_vect)														//ADCB.CH2 - Interrupt
{
	czujnik_int[6]=ADCA.CH0.RESL;
	czujnik[6]=(czujnik_int[6]>prog);									//Check whether is above line
}

ISR(ADCB_CH3_vect)														//ADCB.CH3 - Interrupt
{
	czujnik_int[7]=ADCA.CH0.RESL;
	czujnik[7]=(czujnik_int[7]>prog);									//Check whether is above line
}