/*
 * ADC-Konvertierung eines PS2 Analog-Sticks mittels Polling
 * Poti Schleifer an PC0(ADC0),PC1(PC1)
 * Ausgabe auf LCD, PORTB und UART
 *
 * Jonas Heim - 01.09.2015
 *
 */
#include <stdlib.h>
#include <avr/io.h>
#define F_CPU 1000000UL
#define BAUD 4800UL
#include <uart.h>
#include <util/setbaud.h>
#include <util/delay.h>
#include "lcd-routines.h"
#define TRENNCHAR '|'
#define BEGINNCHAR '#'


static long channel0;
static long channel1;

//Prototypen
void adc_init();
uint16_t adc_read(uint8_t channel);


int main(void){

	char buffer[10];
	lcd_init();
	adc_init();
	uart_init();

	while(1){
		lcd_clear();

		channel0 = adc_read(0x00);
		itoa(channel0, buffer, 10);
		lcd_string("x-Achse :");
		lcd_string(buffer);
		lcd_setcursor(0,2);

		while (!(UCSRA & (1<<UDRE)))  /* warten bis Senden moeglich */
		    {
		    }
		uart_put_s(buffer);
		uart_put_c(TRENNCHAR);

		channel1 = adc_read(0x01);
		itoa(channel1, buffer, 10);
		lcd_string("y-Achse :");
		lcd_string(buffer);

		while (!(UCSRA & (1<<UDRE)))  /* warten bis Senden moeglich */
		    {
		    }
		uart_put_s(buffer);
		uart_put_c(BEGINNCHAR);

		_delay_ms(200);
	}
}

//
//ADC
//
void adc_init(){
	//ADC 128Bit
		ADCSRA |= 1<<ADEN | 1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0;

		//VCC als Referenz
		ADMUX |= 1<<REFS0;
}

uint16_t adc_read(uint8_t channel){
	ADMUX = (ADMUX & ~(0x1F)) | (channel & 0x1F);
	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC) ) {
	}
	return ADCW;
}
