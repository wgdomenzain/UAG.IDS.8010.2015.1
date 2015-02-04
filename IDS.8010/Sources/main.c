/*
 * main implementation: use this 'C' sample to create your own application
 *
 */


//Karla Oliva


#include "derivative.h" /* 
include peripheral declarations
 */
//Time definitions
#define ntime_15msec    0x02E8
#define ntime_40usec	0x1AD0
#define nInst	0
#define nData	1


//MACROS
#define Port_LCD GPIOC_PDOR

//Enable connected to portb_01
#define PortEnable_1  GPIOB_PDOR |= 0x01
#define PortEnable_0  GPIOB_PDOR &= 0xFE
#define PortRS_1  GPIOB_PDOR |= 0x02
#define PortRS_0  GPIOB_PDOR &= 0xFD

void configurarPuertos(void);
void initLCD(void);
void delay(long time);
void sendCode(int code, int data);
void clear(void);
void caracter(char arreglo[]);



int main(void)
{
configurarPuertos();
initLCD();
clear();

sendCode(nInst, 0x85);
sendCode(nData, 'K');
sendCode(nData, 'A');
sendCode(nData, 'R');
sendCode(nData, 'L');
sendCode(nData, 'A');

		for(;;) {
	  }
	return 0;
}


void caracter(char arreglo[])
{
	
int i = 0;
for(i = 0;i < 5; i++)
{
sendCode(nData, arreglo[i]);
	}
}


void configurarPuertos(void){
//Turn on clock for portC
SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;
SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	
	
/* Set pins */

//PORT B
PORTB_PCR0 = PORT_PCR_MUX(1);     //E
PORTB_PCR1 = PORT_PCR_MUX(1);     //RS
	

//PORT C
PORTC_PCR0 = PORT_PCR_MUX(1);
PORTC_PCR1 = PORT_PCR_MUX(1);
PORTC_PCR2 = PORT_PCR_MUX(1);
PORTC_PCR3 = PORT_PCR_MUX(1);
PORTC_PCR4 = PORT_PCR_MUX(1);
PORTC_PCR5 = PORT_PCR_MUX(1);
PORTC_PCR6 = PORT_PCR_MUX(1);
PORTC_PCR7 = PORT_PCR_MUX(1);
	
	

//Initialize PortB and PortD
GPIOB_PDOR = 0x00;
GPIOC_PDOR = 0x00;
		
//Configurar de salida los puertos
GPIOB_PDDR = 0xFF;
GPIOC_PDDR = 0xFFFF;

}


void initLCD(void){
sendCode(nInst, 0x38);
sendCode(nInst, 0x38);
sendCode(nInst, 0x38);
sendCode(nInst, 0x0C);
sendCode(nInst, 0x01);
}



void delay(long time)
{
  while (time > 0)
    {
 time--;
}
 }


void sendCode(int code, int PortData)
{
	PortRS_0;
	PortEnable_0;
	
	Port_LCD = PortData;

	if(code == nInst)
 {
		PortRS_0;
		PortEnable_1;
		delay (ntime_40usec);
		PortEnable_0;
		PortRS_0;
	}
	else if (code == nData)
	{
		PortRS_1;
		PortEnable_1;
		delay(ntime_40usec);		
		PortEnable_0;
		PortRS_0;
	}
}



void clear(void){
	sendCode(nInst, 0x01);
}
