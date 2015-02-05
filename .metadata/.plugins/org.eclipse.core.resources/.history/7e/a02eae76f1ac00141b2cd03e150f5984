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
#define nIns    0
#define nData   1
//MACROS
#define Port_LCD GPIOD_PDOR
 
int dife = 1;
int canto = 0;
int estado = 0;
//Enable connected to portb_01
#define Enable_1    GPIOB_PDOR |= 0x01
#define Enable_0    GPIOB_PDOR &= 0xFE
#define RS_1        GPIOB_PDOR |= 0x02
#define RS_0        GPIOB_PDOR &= 0xFD
#define readBitB(x)  (((GPIOB_PDIR) >> x) & 0x01) 
#define ReadBitPortE(x) ((GPIOE_PDIR >> x) & 0x00000001)
#define ECLOCK ReadBitPortE(0)
#define EDATA ReadBitPortE(1)
void configurarPuertos(void);
void initLCD(void);
void delay(long time);
void sendCode(int code, int data);
void initRTC(void);
void cfgClock(void);
int count(int x);
void iniTimer(void);
void imprimeNum(int x, int y);
 
int main(void) {
    configurarPuertos();
    initLCD();
    iniTimer();
    //cfgClock();
    //initRTC();
    sendCode(nIns, 0x80);
    sendCode(nIns, 0x80);
    for (;;) {
    	if(readBitB(2) == 0){
			delay(262000);
			if(readBitB(2) == 0){
				sendCode(nIns, 0x01);
				estado++;
				dife = 0;
				canto = 0;
				switch(estado){
				case 1: PIT_LDVAL1 = 0x48A1C * 2;
					break;
				case 2: PIT_LDVAL1 = 0x48A1C * 4;
					break;
				case 3: PIT_LDVAL1 = 0x48A1C * 10;
					break;
				case 4: PIT_LDVAL1 = 0x48A1C * 20;
					break;
				}
				if(estado == 5){
					estado = 0;
					PIT_LDVAL1 = 0x48A1C;
				}
			}
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
    /*  PORTC_PCR0 = PORT_PCR_MUX(1);
     PORTC_PCR2 = PORT_PCR_MUX(1);*/
 
    //Initialize PortB and PortD and PortE
    GPIOB_PDOR = 0x00;
    GPIOD_PDOR = 0x00;
    GPIOE_PDOR = 0x00;
 
    //Configure Port as outputs input 0, output 1  
    GPIOD_PDDR = 0xFFFF;
    GPIOB_PDDR = 0xFB;
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
 
 
/*void cfgClock(void) {   //Configure 32 KHz output
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
    int cant = 0;
    cant = count(dife);
    sendCode(nIns, 0x80);
    if(cant == 0){
        sendCode(nData, '0');
    } else {
        imprimeNum(cant, dife);
    }
}*/
 
void iniTimer(void){
     // turn on PIT
    PIT_MCR = PIT_MCR_FRZ_MASK;    
    // Timer 1
    PIT_LDVAL1 = 0x48A1C;
    PIT_TCTRL1 = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
     
    NVIC_ICPR |= 1 << ((INT_PIT - 16) % 32);
 
    NVIC_ISER |= 1 << ((INT_PIT - 16) % 32);
}
 
void PIT_IRQHandler(void){
	int cant = 0;
	switch(estado){
	case 0: 
		dife += 25;
		break;
	case 1:
		dife += 50;
		break;
	case 2:
		dife += 100;
		break;
	case 3:
		dife += 250;
		break;
	case 4:
		dife += 500;
		break;
	}
	if(dife >= 1000){
		dife = 0;
		canto++;
	}
	cant = count(canto);
	sendCode(nIns, 0x80);
	if(cant == 0){
		sendCode(nData, '0');
	} else {
		imprimeNum(cant, canto);
	}
	sendCode(nData, '.');
	cant = count(dife);
	if(cant == 0){
		sendCode(nData, '0');
	} else {
		imprimeNum(cant, dife);
	}
	GPIOB_PTOR = 0x4;
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
