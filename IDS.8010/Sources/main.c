/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

/*
 @author:        Alejandro Villarreal Fernández
 @description:   7010 Group
 @functions:     LCD
 @environment:   KL25Z
 @date:          07/10/2014
 @comments:      
 @version:       2.0 - Initial
 */

//Time definitions
#define nt15_msec   16200
#define nt40_usec   1600

//LCD Control
#define nIns    0
#define nData   1

#define PortLCD     GPIOD_PDOR
//Enable connected to portb_01
#define Enable_1    GPIOB_PDOR |= 0x01
#define Enable_0    GPIOB_PDOR &= 0xFE
#define RS_1        GPIOB_PDOR |= 0x02
#define RS_0        GPIOB_PDOR &= 0xFD

#include "derivative.h" /* include peripheral declarations */
const unsigned char InitializeLCD[5] = { 0x38, 0x38, 0x38, 0x0C, 0x01 };
void cfgPorts(void);
void sendCode(int Code, int Data);
void delay(long time);
void initLCD(void);
void printArray(char text[]);
void customChar(char position, char values[]);
int main(void) {
	int counter = 0;
	char name[10] = { 'A', 'L', 'E', 'J', 'A', 'N', 'D', 'R', 'O', '\0' };
	char bomb[8] = { 0x2, 0x4, 0x4, 0xE, 0x11, 0x11, 0x11, 0xE };
	char ghost[8] = { 0xE0, 0x1F, 0x1F, 0x15, 0x1F, 0x1F, 0x15, 0x00 };

	cfgPorts();
	initLCD();
	sendCode(nIns, 0x87);
	sendCode(nIns, 0x87);
	printArray(name);
	customChar(0, ghost);
	customChar(1, bomb);
	sendCode(nIns, 0x85);
	delay(nt40_usec);
	//sendCode(nData, 0x00);
	sendCode(nIns, 0x91);
	delay(nt40_usec);
	//sendCode(nData, 0x01);
	for (;;) {
		counter++;
	}

	return 0;
}
void cfgPorts(void) {
	//Turn on clock for portb
	SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;

	/* Set pins of PORTB as GPIO */

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

	//Inicializacion
	GPIOB_PDOR = 0x00;
	GPIOD_PDOR = 0x0000;
	GPIOB_PDDR = 0xFF;
	GPIOD_PDDR = 0xFFFF;
}
void initLCD(void) {
	int i;
	delay(nt15_msec);

	/* Send initialization instructions */
	/* Loop for sending each character from the array */
	for (i = 0; i < 5; i++) {
		sendCode(nIns, InitializeLCD[i]); /* send initialization instructions */
	}

}

void printArray(char text[]) {
	int i = 0;
	do {
		delay(nt40_usec);
		sendCode(nData, text[i]);
		i++;
	} while (text[i] != '\0');
}

void customChar(char position, char values[]) {
	int i;
	if (position < 8) {
		sendCode(nIns, (0x40 + (position * 8)));
		for (i = 0; i < 8; i++) {
			sendCode(nData, values[i]);
		}
		sendCode(nIns, 0x80);
	}
}

void sendCode(int Code, int Data) {
	//Assign a value to pin RS
	/*HINT: When RS is 1, then the LCD receives a data
	 when RS is 0, then the LCD receives an instruction */
	// Initialize RS and Enable with 0
	RS_0;
	Enable_0;
	//Assign the value we want to send to the LCD
	PortLCD = Data;

	//We make the algorithm to establish if its an instruction we start with 0 on RS value, otherwise if its a data command we start with RS as 1;
	if (Code == nIns) {
		Enable_1;
		delay(nt40_usec);
		Enable_0;
		RS_0;
	} else if (Code == nData) {
		RS_1;
		Enable_1;
		delay(nt40_usec);
		Enable_0;
		RS_0;
	}
}
void delay(long time) {
	while (time > 0) {
		time--;
	}
}
