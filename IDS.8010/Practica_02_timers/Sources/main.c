/*
 * main implementation: use this 'C' sample to create your own application
 *
 */
/*
 * LCD
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
#define segundos1   1000
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

#define ReadBitPortB(x) ((GPIOB_PDIR >> x) & 0x00000001)

void configurarPuertos(void);
void initLCD(void);
void delay(long time);
void sendCode(int code, int data);
void clear(void);
void caracter(char location, char arreglo[]);
void cfgClock (void);
void initRTC(void);
void cronometro(void);
void RTC_Seconds_IRQHandler(void);
void convertir(int numero);
void changePIT(int ms, int init);
int count( int i);

//Variables cronometro
int bandera = 0;
int mseg = 0;
int segundo_d = 0;
int seg = 0;
static int Counter = 0;
int msval = 0;
int v1 = 0;
int v2 = 0;
int v3 = 0;
int main(void){
	configurarPuertos();
	initLCD();
	clear();
	//cfgClock();
	//initRTC();
	sendCode(nInst, 0x80);
	sendCode(nInst, 0x80);
	
	for(;;) {
			if(ReadBitPortB(2) == 0){
				delay(500000);
				if(ReadBitPortB(2) == 0){
					msval++;
					if(msval > 4)
						msval = 0;
					changePIT(msval,0);
					mseg = 0;
					seg = 0;
				}
		}
	}
	
	return 0;
}

void configurarPuertos(void){
	
	//Turn on clock
	SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;

	/* Set pins of PORTB as GPIO */
	//PORT E
	PORTE_PCR0 = PORT_PCR_MUX(1);
	
	//PORT B
	PORTB_PCR0 = PORT_PCR_MUX(1);
	PORTB_PCR1 = PORT_PCR_MUX(1);
	PORTB_PCR2 = PORT_PCR_MUX(1);		
	//PORT C
	PORTC_PCR0 = PORT_PCR_MUX(1);
	PORTC_PCR1 = PORT_PCR_MUX(1);
	PORTC_PCR2 = PORT_PCR_MUX(1);
	PORTC_PCR3 = PORT_PCR_MUX(1);
	PORTC_PCR4 = PORT_PCR_MUX(1);
	PORTC_PCR5 = PORT_PCR_MUX(1);
	PORTC_PCR6 = PORT_PCR_MUX(1);
	PORTC_PCR7 = PORT_PCR_MUX(1);
	PORTC_PCR8 = PORT_PCR_MUX(1);
		
	//PORT D
	PORTD_PCR0 = PORT_PCR_MUX(1);
	PORTD_PCR1 = PORT_PCR_MUX(1);
	PORTD_PCR2 = PORT_PCR_MUX(1);
	PORTD_PCR3 = PORT_PCR_MUX(1);
	PORTD_PCR4 = PORT_PCR_MUX(1);
	PORTD_PCR5 = PORT_PCR_MUX(1);
	PORTD_PCR6 = PORT_PCR_MUX(1);
	PORTD_PCR7 = PORT_PCR_MUX(1);
	
	changePIT(msval,1); //25mseg, 1 = init
	
	 //PORT C
	//Set PTC1 as RTC_CLKIN - check pinouts table
	PORTC_PCR1|= PORT_PCR_MUX(1);
	 
	//Set PTC3 as RTC_CLKOUT - check pinouts table
	PORTC_PCR3|= PORT_PCR_MUX(5);
	 
	/* Set pins of PORTC as GPIO */
	PORTC_PCR0= PORT_PCR_MUX(1);
	PORTC_PCR2= PORT_PCR_MUX(1);
	     
	
	//Initialize PortB and PortD and PortC
	GPIOB_PDOR = 0x00;
	GPIOC_PDOR = 0x00;
	GPIOD_PDOR = 0x00;
	GPIOE_PDOR = 0x00;
			
	//Configure ports as outputs input 0, output 1	
	//Configurar de salida los puertos
	GPIOD_PDDR = 0xFFFF;
	GPIOB_PDDR = 0xFB;
	GPIOC_PDDR = 0x00;
	GPIOE_PDDR = 0xFF;
		
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

void cfgClock (void)
{//Configure 32 KHz output
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
 
void initRTC(void)
{
    //Enable the clock to RTC module register space - page 208
    //Hint: Check if this register was initialized before
    SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;
 
    //Clear Registers
    //First, clear all RTC registers - page 603
    RTC_CR  = RTC_CR_SWR_MASK;
    //Clear SWR bit
    //Send 0 to SWR bit
    RTC_CR  &= ~RTC_CR_SWR_MASK;  
   
    if (RTC_SR_TIF_MASK)
    {
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
    NVIC_ICPR |= 1 <<(21%32);
     
    // Interrupt Set-enable Register
    // Interrupt Clear-pending Register - Manual 2 page 110
    NVIC_ISER |= 1 <<(21%32);     
     
    // Enable Seconds Interrupt - page 606
    RTC_IER |= RTC_IER_TSIE_MASK; 
   
    // Time counter is enabled. This actions starts RTC counting - page 603
    RTC_SR |= RTC_SR_TCE_MASK;
     
    // Configure the timer seconds and alarm registers - page 599
    RTC_TSR = 0xFF;
 
}
 
void RTC_Seconds_IRQHandler(void)
{
	//sendCode(nInst, 0x80);
	//convertir(Counter);
}

void cronometro(void){
	clear();
}

int count( int i) {
 int ret=1;
 while (i/=10) ret++;
 return ret;
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

void changePIT(int ms, int init) {
	if(init == 1) {
	//PIT initialization
	   SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;         // Turn on clock to to the PIT module
	   PIT_LDVAL0 = 0x122870 / 4;                       // Timeout period = 500us
	   PIT_MCR = PIT_MCR_FRZ_MASK;              // Enable clock for PIT, freeze PIT in debug mode
	   PIT_TCTRL0 = PIT_TCTRL_TIE_MASK |        // Enable PIT interrupt
				   PIT_TCTRL_TEN_MASK;          // and PIT
	
			//Enable PIT interrupt on NVIC   
	   NVIC_ICPR |= 1 << ((INT_PIT - 16) % 32);
	   NVIC_ISER |= 1 << ((INT_PIT - 16) % 32);	
	}
	if(init != 1) {
		
		//PIT initialization
		   PIT_LDVAL0 = 0;                       // Timeout period = 500us
		   PIT_MCR = 0;              // Enable clock for PIT, freeze PIT in debug mode
		   PIT_TCTRL0 = 0;          // and PIT
		
				//Enable PIT interrupt on NVIC   
		   NVIC_ICPR = 0;
		   NVIC_ISER = 0;		
		
		switch(ms) {
		case 0:	//25 miliseconds
			   PIT_LDVAL0 = 0x122870 / 4;
			break;
		case 1:	//50 miliseconds
			   PIT_LDVAL0 = 0x122870 / 2;
			break;
		case 2:	//100 miliseconds
			   PIT_LDVAL0 = 0x122870;
			break;
		case 3:	//250 miliseconds
			   PIT_LDVAL0 = (0x122870 / 4) * 10;	
			break;
		case 4: //500 miliseconds
			   PIT_LDVAL0 = 0x122870 * 5;
			break;
		default:	//default 100 miliseconds
			   PIT_LDVAL0 = 0x122870;
			break;
		}
		
	   PIT_MCR = PIT_MCR_FRZ_MASK;
	   PIT_TCTRL0 = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
	   NVIC_ICPR |= 1 << ((INT_PIT - 16) % 32);
	   NVIC_ISER |= 1 << ((INT_PIT - 16) % 32);	
	}
}

void PIT_IRQHandler()
{
	switch(msval) {
	case 0:
		mseg += 25;
		break;
	case 1:
		mseg += 50;
		break;
	case 2:
		mseg += 100;
		break;
	case 3:
		mseg += 250;
		break;
	case 4:
		mseg += 500;
		break;
	default: 
		break;
	}
	
	if(mseg > 999){
		mseg = 0;
		seg++;
	}	
	sendCode(nInst, 0x80);
	sendCode(nInst, 0x80);
	convertir(seg);
	sendCode(nData, '.');
	convertir(mseg);
				
	 Counter++;                                     // Each increment represents 500us
     PIT_TFLG0 |= PIT_TFLG_TIF_MASK;              // Clear PIT interrupt flag
}
