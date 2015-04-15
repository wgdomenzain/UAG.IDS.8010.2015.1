// * main implementation: use this 'C' sample to create your own application
// */


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
//Macros del boton
#define	PULSE_INPUT		( 2 )
#define	PULSE_IN_PORT	( GPIOB_PDIR )


void configurarPuertos(void);
void initLCD(void);
void delay(long time);
void sendCode(int code, int data);
void clear(void);
void caracter(char arreglo[]);
void vfnPulseInInit(void);
unsigned char bfnGetPulseMeas(void);
// VARIABLES GLOBALES
unsigned char bPulseWidth = 0;

int main(void)
{
configurarPuertos();
initLCD();
clear();
vfnPulseInInit();


		for(;;) 
		{
			//LLama a la funcion donde se va a contar los pulsos de boton 
			bfnGetPulseMeas();
			//Compara el contador que se llama dwpulseWidth si es igual a 1 muestra el primer mensaje 
			if(bPulseWidth==1)
			{
				clear();
				sendCode(nInst, 0x85);
				sendCode(nData, 'K');
				sendCode(nData, 'A');
				sendCode(nData, 'R');
				sendCode(nData, 'L');
				sendCode(nData, 'A');
				
				sendCode(nInst, 0xC0);
				sendCode(nData, 'A');					 
				sendCode(nData, 'R');
			    sendCode(nData, 'A');
				sendCode(nData, 'C');
				sendCode(nData, 'E');
			    sendCode(nData, 'L');
				sendCode(nData, 'Y');
				
			}
			 else if(bPulseWidth==2)
			 {
				 clear();
				 sendCode(nInst, 0x80);
				 sendCode(nData, 'I');
				 sendCode(nData, 'N');
				 sendCode(nData, 'G');
				 sendCode(nData, 'E');
				 sendCode(nData, 'N');
				 sendCode(nData, 'I');
				 sendCode(nData, 'E');
				 sendCode(nData, 'R');
				 sendCode(nData, 'I');
				 sendCode(nData, 'A');
				 
				 sendCode(nInst, 0xC0);
				 sendCode(nData, 'B');
				 sendCode(nData, 'I');
				 sendCode(nData, 'O');
				 sendCode(nData, 'M');
				 sendCode(nData, 'E');
				 sendCode(nData, 'D');
				 sendCode(nData, 'I');
				 sendCode(nData, 'C');
				 sendCode(nData, 'A');
				 				
				 
				 
			 }
			 else if(bPulseWidth==3)
			 {
				 clear();
				 sendCode(nInst, 0x85);
				 sendCode(nData, 'E');
				 sendCode(nData, 'N');
				 sendCode(nData, 'E');
				 sendCode(nData, 'R');
				 sendCode(nData, 'O');
				 sendCode(nData, '.');
				 sendCode(nData, '2');
				 sendCode(nData, '6');
				 
				 sendCode(nInst, 0xC0);
				 sendCode(nData, '1');
				 sendCode(nData, '9');
				 sendCode(nData, '9');
				 sendCode(nData, '4');
		
			 } 
			 else if (bPulseWidth==4)
			 {
				 clear();
				 sendCode(nInst, 0x80);
				 sendCode(nData, 'S');
				 sendCode(nData, 'E');
				 sendCode(nData, 'P');
				 sendCode(nData, 'T');
				 sendCode(nData, 'I');
				 sendCode(nData, 'M');
				 sendCode(nData, 'O');
				 
				 sendCode(nInst, 0xC0);
				 sendCode(nData, 'C');
				 sendCode(nData, 'U');
				 sendCode(nData, 'A');
				 sendCode(nData, 'T');
				 sendCode(nData, 'R');
				 sendCode(nData, 'I');
				 sendCode(nData, 'M');
				 sendCode(nData, 'E');
				 sendCode(nData, 'S');
				 sendCode(nData, 'T');
				 sendCode(nData, 'R');
				 sendCode(nData, 'E');
			 }
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

//Button
PORTB_PCR2 = PORT_PCR_MUX(1);    //b
	

//PORT C
PORTC_PCR0 = PORT_PCR_MUX(1);
PORTC_PCR1 = PORT_PCR_MUX(1);
PORTC_PCR2 = PORT_PCR_MUX(1);
PORTC_PCR3 = PORT_PCR_MUX(1);
PORTC_PCR4 = PORT_PCR_MUX(1);
PORTC_PCR5 = PORT_PCR_MUX(1);
PORTC_PCR6 = PORT_PCR_MUX(1);
PORTC_PCR7 = PORT_PCR_MUX(1);
	
	

//Initialize PortB and Portc
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

void vfnPulseInInit(void)
{
	GPIOB_PDDR &= ~( 1 << PULSE_INPUT );
	PORTB_PCR2  =  PORT_PCR_MUX(1) | PORT_PCR_PE_MASK ;	
}
//

unsigned char bfnGetPulseMeas(void)
{
	while (( PULSE_IN_PORT & ( 1 << PULSE_INPUT)) );
		while (( PULSE_IN_PORT & ( 1 << PULSE_INPUT) ));
		do
		{
			bPulseWidth++;
		}while ( PULSE_IN_PORT & ( 1 << PULSE_INPUT) );
		return ( bPulseWidth );
}
