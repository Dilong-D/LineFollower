
/*
 * config.h
 *
 * Created: 21.07.2017 19:49:26
 *  Author: dmark
 */ 
#ifndef CONFIG_H_
#define CONFIG_H_

#define F_CPU 32000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stddef.h>
//-----------STALE--------------
#define STOP	1
#define LUZ		0
#define PRAWO	2
#define LEWO	3
//------------REJESTRY---------
#define L_PWM	TCD0.CCD //zwraca wypelnienie PWM silniku L
#define R_ENKODER TCF0.CNT //zwraca stan enkodera silnik L
#define R_PWM	TCD0.CCA //zwraca wypelnienie PWM silniku R
#define L_ENKODER TCC0.CNT //zwraca stan enkodera silnik R
#define t_int 5.12 //czas miedzy dwoma przerwaniami dla PID i wyliczania pozycji.

//------------FUNKCJE-----------
void setMotorL(void);//ustawia piny silnika L
void motorL(int8_t o, int8_t k);//zadaje wypelnienie PWM i kierunek silniku L
void setMotorR(void);//ustawia piny silnika R
void motorR(int8_t o, int8_t k);//zadaje wypelnienie PWM i kierunek silniku L
void OscXtal(void);
void setall(void);//ustawia wszystko

void ledYellow(void);
void ledGreen(void);
void setbat(void);
void Osc32MHz(void);

#endif /* CONFIG_H_ */