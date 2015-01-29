/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
#include "derivative.h" /* include peripheral declarations */
#define GPIO_PIN_MASK 0x1Fu
#define GPIO_PIN(x) (((1)<<(x & GPIO_PIN_MASK)))
#define PortLCD 		GPIOC_PDOR
#define PortEnable_1 	GPIOB_PDOR |= 0x02
#define PortEnable_0 	GPIOB_PDOR &= 0xFD
#define PortRS_1 		GPIOB_PDOR |= 0x01
#define PortRS_0 		GPIOB_PDOR &= 0xFE
#define ntime_1sec		1500000
#define ntime_0_5sec    450000
#define ntime_15msec    0x02E8
#define ntime_40usec	0x1AD0
#define	nIns	0x00
#define	nData	0x01

#define ReadBitPortE(x) ((GPIOE_PDIR >> x) & 0x00000001)
#define ECLOCK ReadBitPortE(0)
#define EDATA ReadBitPortE(1)



//Commands = {0x38, 0x38, 0x38, 0x0C, 0x01};
const unsigned char InitializeLCD[5] = {0x38, 0x38, 0x38, 0x0F, 0x01};

//--------------------------------------------------------------
//Declare Prototypes
/* Functions */
void cfgPorts(void);
void delay(long time);
void initLCD(void);
void sendCode(int Code, int PortData);
void iniTimer(void);
int count(int x);
void number(int x, int y);

/* Variables */
long x;
int d = 1;
int c = 0;
/*@description: Initial Port Cfg 
*/

int main(void)
{
	cfgPorts();
	initLCD();
	iniTimer();
	
	sendCode(nIns, 0x80);
	sendCode(nIns, 0x80);
	
	for(;;)
	{
	sendCode(1, 0x38);
	}

	return 0;
}

void cfgPorts(void)
{
	//Turn on clock for portC and portB
	SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;//0x0C00;//0000 1100 0000 0000 see page 206
	//Also, we can use predefined masks created by freescale, such as:
	//SIM_SCGC5_PORTB_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
	
	/* Set pins of PORTD as GPIO */
	PORTC_PCR0=(PORT_PCR_MUX(1));
	PORTC_PCR1=(PORT_PCR_MUX(1));
	PORTC_PCR2=(PORT_PCR_MUX(1));
	PORTC_PCR3=(PORT_PCR_MUX(1));
	PORTC_PCR4=(PORT_PCR_MUX(1));
	PORTC_PCR5=(PORT_PCR_MUX(1));
	PORTC_PCR6=(PORT_PCR_MUX(1));
	PORTC_PCR7=(PORT_PCR_MUX(1));	

	/* Set pins of PORTB as GPIO */
	PORTB_PCR0=(PORT_PCR_MUX(1));
	PORTB_PCR1=(PORT_PCR_MUX(1));
	
	//Reasure first PortC value
	GPIOC_PDOR = 0x00;
	//Reasure first PortB value
	GPIOB_PDOR = 0x00;
	
	//Configure PortB as outputs
	GPIOB_PDDR = 0xFF;
	//Configure PortC as outputs
	GPIOC_PDDR = 0xFF;
}
void delay (long time)
{
	long x;
	x = time;
	while(x!=0)
	{
	x--;
	}
}

void initLCD(void)
{
	unsigned int i;
	
	//LCD Requires 15ms
	
	//delay(ntime_15msec);
	 for(i=0;i<5;i++)
	{
		sendCode(nIns, InitializeLCD[i]);
	}
	sendCode(nIns, 0x38);
    sendCode(nIns, 0x38);
    sendCode(nIns, 0x38);
    sendCode(nIns, 0x0C);
    sendCode(nIns, 0x01);
	
}

void sendCode(int Code, int PortData)
{
	PortRS_0;
	PortEnable_0;
	
	PortLCD = PortData;
	
	if (Code == nIns)
	{
		PortRS_0;
		PortEnable_1;
		delay(ntime_40usec);
		PortEnable_0;
		PortRS_0;
	}
	else if (Code == nData)
	{
		PortRS_1;
		PortEnable_1;
		delay(ntime_40usec);		
		PortEnable_0;
		PortRS_0;
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

int count(int x){
    int res = 0;
     while (x > 0) {
     res++;
     x /= 10;
    }
    return res;
}
void number(int x, int y){
    char numero[x];
    int a = 0;
    int num = 0;
    int z = 0;
    if(x == 1){
        numero[a] = (char)(((int)'0')+y);
        sendCode(nData, numero[0]);
    } else {
        while(y > 0){
            num = y % 10;
            numero[a] = (char)(((int)'0')+num);
            a++;
            y /= 10;
        }
        for(z = x-1; z > -1; z--){
            sendCode(nData, numero[z]);
        }
    }
}
void PIT_IRQHandler(void){
	int cc = 0;
    d++;
    /*
    	int dife = 1;
    	int canto = 0;*/
    if(d == 10){
    	d = 0;
    	c++;
    }
	cc = count(c);
	sendCode(nIns, 0x80);
	if(cc == 0){
		sendCode(nData, '0');
	} else {
		number(cc, c);
	}
	sendCode(nData, '.');
	cc = count(d);
	if(cc == 0){
		sendCode(nData, '0');
	} else {
		number(cc, d);
	}
	GPIOB_PTOR = 0x4;
    PIT_TFLG1 |= PIT_TFLG_TIF_MASK;     // Clear the timer interrupt flag
 
    PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
}
