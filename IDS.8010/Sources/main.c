/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
/*
 * LCD
 * Alvaro Zetina Ayala
 * 7/OCT/14
 * 
 * B0= Enable
 * B1= RS
 */


#include "derivative.h" /* include peripheral declarations */


//Time definitions
#define segundos   1600

#define nInst	0
#define nData	1
//MACROS
#define Port_LCD GPIOD_PDOR

//Enable connected to portb_01
#define Enable_1    GPIOB_PDOR |= 0x01
#define Enable_0    GPIOB_PDOR &= 0xFE
#define RS_1        GPIOB_PDOR |= 0x02
#define RS_0        GPIOB_PDOR &= 0xFD

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
	sendCode(nData, 'A');
	sendCode(nData, 'l');
	sendCode(nData, 'v');
	sendCode(nData, 'a');
	sendCode(nData, 'r');
	sendCode(nData, 'o');
		
	for(;;) {
	  
	}
	
	return 0;
}

void caracter(char arreglo[]){
	int i = 0;
	for(i = 0; i < 8; i++){
		sendCode(nData, arreglo[i]);
	}
}

void configurarPuertos(void){
	
	//Turn on clock for portb
	SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	
	/* Set pins of PORTB as GPIO */
	//PORT B
	PORTB_PCR0 = PORT_PCR_MUX(1);
	PORTB_PCR1 = PORT_PCR_MUX(1);
	//PORT D
	PORTD_PCR0 = PORT_PCR_MUX(1);
	PORTD_PCR1 = PORT_PCR_MUX(1);
	PORTD_PCR2 = PORT_PCR_MUX(1);
	PORTD_PCR3 = PORT_PCR_MUX(1);
	PORTD_PCR4 = PORT_PCR_MUX(1);
	PORTD_PCR5 = PORT_PCR_MUX(1);
	PORTD_PCR6 = PORT_PCR_MUX(1);
	PORTD_PCR7 = PORT_PCR_MUX(1);
	
	//Initialize PortB and PortD
	GPIOB_PDOR = 0x00;
	GPIOD_PDOR = 0x00;
		
	//Configurar de salida los puertos
	GPIOB_PDDR = 0xFF;
	GPIOD_PDDR = 0xFFFF;
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

void sendCode(int code, int data){
	RS_0;
	Enable_0;
	Port_LCD = data;
	if(code == nInst){
		Enable_1;
		delay(segundos);
		Enable_0;
	}
	
	if(code == nData){
		RS_1;
		Enable_1;
		delay(segundos);
		Enable_0;
	}
}

void clear(void){
	sendCode(nInst, 0x01);
}

