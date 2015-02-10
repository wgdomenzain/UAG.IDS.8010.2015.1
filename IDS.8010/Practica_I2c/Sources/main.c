#include "derivative.h" 
#include "i2c.h"
#define nt15_msec   16200 
#define nt40_usec   1600 
char result;
char slave,memory;
char dato=0;
void configurarPuertos(void);
void initLCD(void);	
int main(void)
{
slave=0x50;
dato=1;
memory=0x50;

		
	Init_I2C();
	
	for (;;)
	{
		
	I2CWriteRegister(slave,memory,'a');
	result=I2CReadRegister(slave,memory);

	}
	
	
return 0;
}

void configurarPuertos(void) {	
	
	//Turn on clock for portb	
	SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;	
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;	
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;	
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;	
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;	

	/* Set pins of PORTB as GPIO */
	//PORT B
	PORTB_PCR0 = PORT_PCR_MUX(1);	
	PORTB_PCR1 = PORT_PCR_MUX(1);	
	PORTB_PCR2 = PORT_PCR_MUX(1);	

	//PORT E
	PORTE_PCR0 = PORT_PCR_MUX(6);	
	PORTE_PCR1 = PORT_PCR_MUX(6);	
	PORTE_PCR2 = PORT_PCR_MUX(1);

	//PORT D
	PORTD_PCR0 = PORT_PCR_MUX(1);	
	PORTD_PCR1 = PORT_PCR_MUX(1);	
	PORTD_PCR2 = PORT_PCR_MUX(1);		
	PORTD_PCR3 = PORT_PCR_MUX(1);	
	PORTD_PCR4 = PORT_PCR_MUX(1);	
	PORTD_PCR5 = PORT_PCR_MUX(1);	
	PORTD_PCR6 = PORT_PCR_MUX(1);	
	PORTD_PCR7 = PORT_PCR_MUX(1);	
	
	//Portc	
	//Set PTC1 as RTC_CLKIN - check pinouts table
		PORTC_PCR1 |= PORT_PCR_MUX(1);

	//Set PTC3 as RTC_CLKOUT - check pinouts table
		PORTC_PCR3 |= PORT_PCR_MUX(5);

	/* Set pins of PORTC as GPIO */
		/*	PORTC_PCR0 = PORT_PCR_MUX(1);
	 	 PORTC_PCR2 = PORT_PCR_MUX(1);*/

	//Initialize PortB and PortD and PortE
		GPIOB_PDOR = 0x00;
		GPIOD_PDOR = 0x00;
		GPIOE_PDOR = 0x00;

	//Configure Port as outputs input 0, output 1	
		GPIOD_PDDR = 0xFFFF;
		GPIOB_PDDR = 0xFF;
		GPIOE_PDDR = 0xF0;
		GPIOC_PDDR = 0x00;
}	

