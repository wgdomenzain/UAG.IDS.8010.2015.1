/*
 * main implementation: use this 'C' sample to create your own application
 *gegrgrg
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
const unsigned char InitializeLCD[5] = {0x38, 0x38, 0x38, 0x0C, 0x01};
const unsigned char customChar[6] = {0x00, 0x04, 0x04, 0x04, 0x04, 0x0E};
const unsigned char customChar2[5] = {0x00, 0x0E, 0x0E, 0x04, 0x1F};
const unsigned char customCharSword[8] = {0x04,0x04,0x04,0x04,0x0E,0x15,0x04,0x00};

int const caracter0_direccion[8]={0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47};
int const caracter0_datos[8]={0b00100,0b00100,0b00100,0b00100,0b01110,0b10101,0b00100,0b0};
void cfgPorts(void);

void sendCode(int Code, int Data);
void delay(long time);
void initLCD(void);
int main(void)
{
	int counter = 0;
	
    //Configure ports
    cfgPorts();
    //Initialize LCD
    initLCD();
    //Set position to print character
    sendCode(nIns, 0x85);
    sendCode(nIns, 0x85);
    //Print character
    delay(nt40_usec);
    sendCode(nData, 'P');
    delay(nt40_usec);
    sendCode(nData, 'A');
    delay(nt40_usec);
    sendCode(nData, 'B');
    delay(nt40_usec);
    sendCode(nData, 'L');
    delay(nt40_usec);
    sendCode(nData, 'O');
    delay(nt40_usec);
    sendCode(nData, ' ');
    delay(nt40_usec);
    sendCode(nIns, 0x80);    
    sendCode(nIns,0xC0);
    delay(nt40_usec);
	//sendCode(nIns,0x00);
	delay(nt40_usec);
    
	for(;;) {	   
	   	counter++;
	}
	
	return 0;
}
void cfgPorts(void)
{
    //Turn on clock for portb
    SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;   
    SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
 
    /* Set pins of PORTB as GPIO */
    
    PORTB_PCR0 = PORT_PCR_MUX(1);
    PORTB_PCR1 = PORT_PCR_MUX(1);
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
    //Configure PortB as outputs (including B18 as output and B0 as input)
    GPIOB_PDDR = 0xFF;   //1111 1111 1111 1111 1000
    GPIOD_PDDR = 0xFFFF;//11XX XXXX XXXX XXXX XXX0
}
void initLCD(void)
{
    int i;
    delay(nt15_msec);
     
    /* Send initialization instructions */
    /* Loop for sending each character from the array */
    for(i=0;i<5;i++)
    {                                      
        sendCode(nIns, InitializeLCD[i]);   /* send initialization instructions */         
    }
     
}
 
void sendCode(int Code, int Data)
{
    //Assign a value to pin RS
    /*HINT: When RS is 1, then the LCD receives a data
    when RS is 0, then the LCD receives an instruction */
    // Initialize RS and Enable with 0
    RS_0;
    Enable_0;
    //Assign the value we want to send to the LCD
    PortLCD = Data;
     
    //We make the algorithm to establish if its an instruction we start with 0 on RS value, otherwise if its a data command we start with RS as 1;
    if (Code == nIns)
    {
        Enable_1;
        delay(nt40_usec);
        Enable_0;
        RS_0;
    }      
    else if(Code == nData)
    {
        RS_1;
        Enable_1;
        delay(nt40_usec);
        Enable_0;
        RS_0;
    }
}

void delay(long time)
{
    while (time > 0)
    {
        time--;
    }
}
