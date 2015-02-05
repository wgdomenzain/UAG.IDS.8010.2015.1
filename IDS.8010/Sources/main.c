//Isaac de Jesús Figueroa Alfaro

#include "derivative.h"

#define Port_LCD GPIOD_PDOR
#define nt15_msec   16200 
#define nt40_usec   1600 
#define segundos 1600 
#define nIns	0 
#define nData	1  
 
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
void cfgClock(void);	
void sendCode(int code, int data);	
void initRTC(void);	
void convertir(int numero);
void imprimeNum(int x, int y);
int count(int x);	
void iniTimer(void);

int milisegundos=0;	
int segundo=0;
int bandera=0;

int main(void) {	
	configurarPuertos();	
	initLCD();	
	iniTimer();	
	cfgClock();		
	initRTC();		
		
		
	for (;;) {	
		if(ReadBitPortE(2)==0){
			segundo=0;
			milisegundos=0;
			delay(600000);
			bandera++;
			if(bandera>4)
				bandera=0;
		}
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
	PORTE_PCR0 = PORT_PCR_MUX(1);	
	PORTE_PCR1 = PORT_PCR_MUX(1);	
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

void initRTC(void) {	
	//Enable the clock to RTC module register space - page 208	
	//Hint: Check if this register was initialized before	
	SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;	
		
	//Clear Registers	
	//First, clear all RTC registers - page 603	
	RTC_CR = RTC_CR_SWR_MASK;	
	//Clear SWR bit	
	//Send 0 to SWR bit	
	RTC_CR &= ~RTC_CR_SWR_MASK;	

	if (RTC_SR_TIF_MASK) {	
		// This action clears the TIF. Remember:	
		// This bit is cleared by writing the TSR register when the time counter is disabled	
		// page 604	
		RTC_TSR = 0;	
	}	

	// Set time compensation parameters (not strictly required) - page 600	
	RTC_TCR = RTC_TCR_CIR(1);	
	RTC_TCR |= RTC_TCR_TCR(255);	

	// Enable RTC seconds irq - page 53 (remember modulus explanation) 	  
	// Interrupt Clear-pending Register - Manual 2 page 112	
	NVIC_ICPR |= 1 << (21 % 32);	

	// Interrupt Set-enable Register	
	// Interrupt Clear-pending Register - Manual 2 page 110	
	NVIC_ISER |= 1 << (21 % 32);	

	// Enable Seconds Interrupt - page 606	
	RTC_IER |= RTC_IER_TSIE_MASK;	

	// Time counter is enabled. This actions starts RTC counting - page 603	
	RTC_SR |= RTC_SR_TCE_MASK;	

	// Configure the timer seconds and alarm registers - page 599	
	RTC_TSR = 0xFF;	

}

void RTC_Seconds_IRQHandler(void) {	
	
}	

void iniTimer(void){	
	 // turn on PIT	
	PIT_MCR = PIT_MCR_FRZ_MASK;	 	
	// Timer 1	

	PIT_TCTRL1 = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;	
	
	NVIC_ICPR |= 1 << ((INT_PIT - 16) % 32);	

	NVIC_ISER |= 1 << ((INT_PIT - 16) % 32);	
}

void PIT_IRQHandler(void){
	//Interrepcion
	 // turn on PIT	
	PIT_MCR = 0;	 	
	// Timer 1	 	
	PIT_TCTRL1 = 0;	
	NVIC_ICPR = 0;
	NVIC_ISER = 0;

	
	switch(bandera){
	case 0://25ms
		milisegundos+=25;
		PIT_LDVAL1 = 0x122870/4;
	break;
	case 1://50ms	
		milisegundos+=50;
		PIT_LDVAL1 = 0x122870/2;
	break;
	case 2://100ms		    
		milisegundos+=100;
		PIT_LDVAL1 = 0x122870;
	break;
	case 3://250ms
		milisegundos+=250;
		PIT_LDVAL1 = 0x122870*2.5;
	break;
	case 4://500ms
		milisegundos+=500;
		PIT_LDVAL1 = 0x122870*5;
	break;
	
	}
	if(milisegundos>999){	
		segundo++;	
		milisegundos=0;	
	}
	
	sendCode(nIns, 0x86);
	sendCode(nIns, 0x86);
	
	if(segundo==0){
		sendCode(nData,'0');
	}
	else{
		convertir(segundo);	
	}
	
	sendCode(nData,'.');
	
	if(milisegundos==0){
		sendCode(nData,'0');
	}
	else{
		convertir(milisegundos);
	}
	
	GPIOB_PTOR=0x4;
	
	PIT_TFLG1 |= PIT_TFLG_TIF_MASK;     
	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;	
	
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
		
		for( x = 0; x <y; x++){	
			sendCode(nData, arr[x]);	
		}	
	}

void delay(long time) {	
	while (time > 0) {	
		time--;	
	}	
}	

int count(int x){	
	int res = 0;	
	 while (x > 0) {	
	 res++;	
	 x /= 10;	
	}	
	return res;	
}

void imprimeNum(int x, int y){	
	char numero[x];	
	int pos = 0;	
	int digit = 0;	
	int z = 0;	
	if(x == 1){	
		numero[pos] = (char)(((int)'0')+y);	
		sendCode(nData, numero[0]);	
	} else {	
		while(y > 0){	
			digit = y % 10;	
			numero[pos] = (char)(((int)'0')+digit);	
			pos++;	
			y /= 10;	
		}	
		for(z = x-1; z > -1; z--){	
			sendCode(nData, numero[z]);	
		}	
	}	
}
