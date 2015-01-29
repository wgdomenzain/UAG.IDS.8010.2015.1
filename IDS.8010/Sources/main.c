
#include "derivative.h" /* include peripheral declarations */
 
#define nt15_msec   16200
#define nt40_usec   1600
#define segundos 1600
#define nIns    0
#define nData   1
#define Port_LCD GPIOD_PDOR
 
int dife = 1;
int canto = 0;

#define Enable_1    GPIOB_PDOR |= 0x01
#define Enable_0    GPIOB_PDOR &= 0xFE
#define RS_1        GPIOB_PDOR |= 0x02
#define RS_0        GPIOB_PDOR &= 0xFD
#define ReadBitPortE(x) ((GPIOE_PDIR >> x) & 0x00000001)
#define ECLOCK ReadBitPortE(0)
#define EDATA ReadBitPortE(1)
void cfgPorts(void);
void initLCD(void);
void delay(long time);
void sendCode(int code, int data);
void initRTC(void);
void cfgClock(void);
int count(int x);
void iniTimer(void);
void printNum(int x, int y);
 
int main(void) {
	cfgPorts();
    initLCD();
    iniTimer();
    sendCode(nIns, 0x80);
    sendCode(nIns, 0x80);
    for (;;) {
    }
 
    return 0;
}
 
 void initLCD(void) {
 
    sendCode(nIns, 0x38);
    sendCode(nIns, 0x38);
    sendCode(nIns, 0x38);
    sendCode(nIns, 0x0C);
    sendCode(nIns, 0x01);
}
 
void cfgPorts(void) {
 
    //Turn on clock for portb
    SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;
    SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
    SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
 
    
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
    
    PORTC_PCR1 |= PORT_PCR_MUX(1);
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
	int cant = 0;
    dife++;
    if(dife == 10){
    	dife = 0;
    	canto++;
    }
	cant = count(canto);
	sendCode(nIns, 0x80);
	if(cant == 0){
		sendCode(nData, '0');
	} else {
		printNum(cant, canto);
	}
	sendCode(nData, '.');
	cant = count(dife);
	if(cant == 0){
		sendCode(nData, '0');
	} else {
		printNum(cant, dife);
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
 
void printNum(int x, int y){
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
