/*
 * main implementation: use this 'C' sample to create your own application
 *
 * LCD
 * B0 = ENABLE
 * B1 = RS
 * 
 * TECLADO
 * E0 = Clock
 * E1 = Data
 * 
 * blanco=tierra
 * azul = voltaje
 * negro = clock
 * gris = Data
 */

#include "derivative.h" /* include peripheral declarations */

#define nt15_msec   16200
#define nt40_usec   1600
#define segundos 1600
#define nIns	0
#define nData	1
//MACROS
#define Port_LCD GPIOD_PDOR

int dife = 1;
int segundo = 0;
//Enable connected to portb_01
#define Enable_1    GPIOB_PDOR |= 0x01
#define Enable_0    GPIOB_PDOR &= 0xFE
#define RS_1        GPIOB_PDOR |= 0x02
#define RS_0        GPIOB_PDOR &= 0xFD
#define ReadBitPortE(x) ((GPIOE_PDIR >> x) & 0x00000001)
#define ECLOCK ReadBitPortE(0)
#define EDATA ReadBitPortE(1)
void configurarPuertos(void);
void initLCD(void);
void delay(long time);
void sendCode(int code, int data);

void cfgClock(void);
int count(int x);
void iniTimer(void);
void convertir(int numero);

int main(void) {
	configurarPuertos();
	initLCD();
	iniTimer();
	cfgClock();

	sendCode(nIns, 0x80);
	sendCode(nIns, 0x80);
	for (;;) {
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

	//PORT E
	PORTE_PCR0 = PORT_PCR_MUX(1);
	PORTE_PCR1 = PORT_PCR_MUX(1);

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

void initLCD(void) {

	sendCode(nIns, 0x38);
	sendCode(nIns, 0x38);
	sendCode(nIns, 0x38);
	sendCode(nIns, 0x0C);
	sendCode(nIns, 0x01);
}

void delay(long time) {
	while (time > 0) {
		time--;
	}
}

void sendCode(int code, int data) {
	RS_0;
	Enable_0;
	Port_LCD = data;
	if (code == nIns) {
		Enable_1;
		delay(nt40_usec);
		Enable_0;
	}

	if (code == nData) {
		RS_1;
		Enable_1;
		delay(nt40_usec);
		Enable_0;
	}
}


void cfgClock(void) {	//Configure 32 KHz output
// Add cfg of Clock Source Select
//Enable internal reference clock - page 372
	MCG_C1 = MCG_C1_IRCLKEN_MASK;
	//MCG_C1 |= MCG_C1 | MCG_C1_IRCLKEN_MASK; 

	//Internal Reference Clock -->Slow - page 373
	MCG_C2 &= ~(MCG_C2_IRCS_MASK);  //Internal Reference Clock -->Slow

	//Selects the 32 kHz clock source as RTC_CLKIN - page 194
	SIM_SOPT1 = SIM_SOPT1_OSC32KSEL(2);

	// Selects the clock to output on the CLKOUT pin as MCGIRCLK (32 Khz) - page 196
	SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL(4);
}

void iniTimer(void){
	 // turn on PIT
	PIT_MCR = PIT_MCR_FRZ_MASK; 	
	// Timer 1
	PIT_LDVAL1 = 0x122870; 
	PIT_TCTRL1 = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
	
	NVIC_ICPR |= 1 << ((INT_PIT - 16) % 32);

	NVIC_ISER |= 1 << ((INT_PIT - 16) % 32);
}

void PIT_IRQHandler(void){	
	dife++;
	if(dife>9){
		segundo++;
		dife=0;
	}
	convertir(segundo);
	sendCode(nData,'.');
	convertir(dife);
	GPIOB_PTOR=0x4;	
	
	PIT_TFLG1 |= PIT_TFLG_TIF_MASK;     // Clear the timer interrupt flag 

	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
}

int count(int x){
	int res = 0;
	 while (x > 0) {
	 res++;
	 x /= 10;
	}
	return res;
}

void convertir(int numero){
		int x = 0;
		int contador = count(numero);
		int y = contador;
		char arr[contador];
		while (contador--) {
		 arr[contador]=(char)(((int)'0')+numero%10);
		 numero/=10;
		}
		
		for( x = 0; x < y; x++){
			sendCode(nData, arr[x]);
		}
	}
