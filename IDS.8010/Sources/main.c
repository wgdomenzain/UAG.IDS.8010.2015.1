
/*
 * LCD
 * 7/OCT/14
 * D0 = D0
 * D1 = D1
 * D2 = D2
 * D3 = D3
 * D4 = D4
 * D5 = D5
 * D6 = D6
 * D7 = D7
 * B0 = ENABLE
 * B1 = RS
 */

#include "derivative.h" /* include peripheral declarations */


//Time definitions
#define segundos1   16000
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
const unsigned char Bateria[8]={0x0E,0x1B,0x11,0x1F,0x1F,0x1F,0x1F,0x1F};
void caracter(char location, char arreglo[]);
int main(void){
	configurarPuertos();
	initLCD();
	clear();
	sendCode(nInst, 0x85);
	sendCode(nInst, 0x85);
	sendCode(nData, 'E');
	sendCode(nData, 'D');
	sendCode(nData, 'G');
	sendCode(nData, 'A');
	sendCode(nData, 'R');
	
	/*int x =0;
	    int y =0;
	    int z =0;
	    //Array Bateria    
	    sendCode(nInst, 0x84);
	    sendCode(nInst, 0x84);
	    sendCode(nInst, 0x40);
	    for(x=0;x<8;x++){
	    sendCode(nData,Bateria[x]);
	    }
	    sendCode(nInst,0x84);
	    sendCode(nData,0x00);
	    delay(1620000);*/
	for(;;){
		
	}
	
	return 0;
}


void configurarPuertos(void){
	
	//Turn on clock for portb
	SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	/* Set pins of PORTB as GPIO */
	//PORT B
	PORTB_PCR0 = PORT_PCR_MUX(1);
	PORTB_PCR1 = PORT_PCR_MUX(1);
			
	//PORT C
	PORTC_PCR0 = PORT_PCR_MUX(1);
	PORTC_PCR1 = PORT_PCR_MUX(1);
	PORTC_PCR2 = PORT_PCR_MUX(1);
	PORTC_PCR3 = PORT_PCR_MUX(1);
	PORTC_PCR4 = PORT_PCR_MUX(1);
	PORTC_PCR5 = PORT_PCR_MUX(1);
	PORTC_PCR6 = PORT_PCR_MUX(1);
	PORTC_PCR7 = PORT_PCR_MUX(1);
	
	//PORT D
	PORTD_PCR0 = PORT_PCR_MUX(1);
	PORTD_PCR1 = PORT_PCR_MUX(1);
	PORTD_PCR2 = PORT_PCR_MUX(1);
	PORTD_PCR3 = PORT_PCR_MUX(1);
	PORTD_PCR4 = PORT_PCR_MUX(1);
	PORTD_PCR5 = PORT_PCR_MUX(1);
	PORTD_PCR6 = PORT_PCR_MUX(1);
	PORTD_PCR7 = PORT_PCR_MUX(1);
	
	//Initialize PortB and PortD and PortC
	GPIOB_PDOR = 0x00;
	GPIOC_PDOR = 0x00;
	GPIOD_PDOR = 0x00;
		
	//Configure ports as outputs input 0, output 1	
	//Configurar de salida los puertos
	GPIOD_PDDR = 0xFFFF;
	GPIOB_PDDR = 0xFF;
	
	    GPIOA_PDDR = 0xFFFFE;	//1111 1111 1111 1111 1110
	    GPIOA_PDDR = 0xFFFF0;
	//GPIOC_PDDR = 0xFFFF;
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

void caracter(char location, char arreglo[]){
	
	sendCode(nInst, location);
	int i = 0;
	for(i = 0; i < 8; i++){
		sendCode(nData, arreglo[i]);
	}
}
