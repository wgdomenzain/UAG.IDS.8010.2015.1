/*
 * main implementation: use this 'C' sample to create your own application
 *
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
 * asa
 * keyboard
 * E0 = Clock
 * E1 = Data
 */
#include "derivative.h" /* include peripheral declarations */
//PAG 584
//
//MACROS
#define segundos 1600
#define nInst	0
#define nData	1
#define Port_LCD GPIOD_PDOR
#define Enable_1    GPIOB_PDOR |= 0x01
#define Enable_0    GPIOB_PDOR &= 0xFE
#define RS_1        GPIOB_PDOR |= 0x02
#define RS_0        GPIOB_PDOR &= 0xFD
#define ReadBitPortE(x) ((GPIOE_PDIR >> x) & 0x00000001)
#define ECLOCK ReadBitPortE(0)
#define EDATA ReadBitPortE(1)

#define DATA_UPDATE_PERIOD		200			// 200 * 500us = 100ms
#define EN_HIGH_TIME			3			// (3 - 1) * 500us = 1ms
#define SENSITIVITY				1024		// MMA8491Q Sensitivity

//FUNCIONES
void configPorts(void);
void initLCD(void);
void delay(long time);
void sendCode(int code, int data);
void clearLCD(void);
void readKeyboard(void);
void keyboard(void);
void initPages(void);
void paintPage(int numero);
void paintName(void);
void cfgClock (void);
void initRTC(void);
void timer(void);
void RTC_Seconds_IRQHandler(void);
void InitUART(void);
void uart0_putchar(char data);
void initPIT(void);
void PIT(void);

//VARIABLES GLOBALES
unsigned char MAIN_DATA;
int lastCharacter = 0;
int capsLock = 0;
int shift = 0;
int page = 0;
int count[4] = {0, 0, 0, 0};
int flag = 0;
int f2state = 0;
unsigned char pages[5][32];//fila, columna
int f1state = 0;
int f3state = 0;
int f2state2 = 0;
int f3state2 = 0;
int main(void)
{
	configPorts();
	initLCD();
	cfgClock();
	initRTC();
	/*
	InitUART();
	initPages();
	initPIT();
	sendCode(nInst,0x80);
	sendCode(nInst,0x80);
	*/
	sendCode(nInst, 0x80);
	sendCode(nInst, 0x80);

	paintName();
	
	return 0;
}

void initPages(void)
{
	int x, y;
	
	for(x = 0; x < 6; x++)
	{
		for(y = 0; y < 32; y++)
		{
			pages[x][y] = ' ';
		}
	}
}

void initPIT(void){
	NVIC_ICPR |= 1 << ((INT_PIT - 16) % 32);
	NVIC_ISER |= 1 << ((INT_PIT - 16) % 32);
}

void PIT(void)
{
    PIT_LDVAL0 = 5240;                       // Timeout period = 500us
    PIT_MCR = PIT_MCR_FRZ_MASK;              // Enable clock for PIT, freeze PIT in debug mode
    PIT_TCTRL0 = PIT_TCTRL_TIE_MASK |        // Enable PIT interrupt
                PIT_TCTRL_TEN_MASK;          // and PIT
	
}

void paintPage(int numero){
	int x;
	clearLCD();
	sendCode(nInst, 0x80);
	sendCode(nInst, 0x80);
	for(x = 0; x < 16; x++){
		sendCode(nData, pages[numero][x]);
	}
	sendCode(nInst, 0xC0);
	sendCode(nInst, 0xC0);
	for(x = 16; x < 32; x++){
		sendCode(nData,pages[numero][x]);
	}
}

void paintName(void)
{
	/*
	page++; //Increase Page
	paintPage(page);
	*/
	sendCode(nInst,0x80);
	sendCode(nInst,0x80);
	sendCode(nData,'M');
	sendCode(nData,'A');
	sendCode(nData,'R');
	sendCode(nData,'T');
	sendCode(nData,'I');
	sendCode(nData,'N');
	return;
}

void keyboard(void)
{
	readKeyboard();
	switch(MAIN_DATA)
	{
		case 0x58:	// CapsLock, Mayus
			capsLock++;
			if(capsLock > 3) capsLock = 0;
			MAIN_DATA = 0x00; 
			break;
		case 0xF0: MAIN_DATA = 0x00; lastCharacter = 1; break;	// Free Key
		case 0x03: if(page > 0) page--; paintPage(page); MAIN_DATA = 0x00; break;	// Paint Page Before
		case 0x0B: //(?)
			if(page)
				page--;
			else
				page = 0;
				
			paintPage(page);
			MAIN_DATA = 0x00;
			break;
		case 0x83: if(page < 4) page++; paintPage(page); MAIN_DATA = 0x00; break;	// Paint Page After
		//LINE 1
		case 0x15: if(capsLock) MAIN_DATA = 'Q'; else MAIN_DATA = 'q'; break;	// Qq
		case 0x1D: if(capsLock) MAIN_DATA = 'W'; else MAIN_DATA = 'w'; break;	// Ww
		case 0x24: if(capsLock) MAIN_DATA = 'E'; else MAIN_DATA = 'e'; break;	// Ee
		case 0x2D: if(capsLock) MAIN_DATA = 'R'; else MAIN_DATA = 'r'; break;	// Rr
		case 0x2C: if(capsLock) MAIN_DATA = 'T'; else MAIN_DATA = 't'; break;	// Tt
		case 0x35: if(capsLock) MAIN_DATA = 'Y'; else MAIN_DATA = 'y'; break;	// Yy
		case 0x3C: if(capsLock) MAIN_DATA = 'U'; else MAIN_DATA = 'u'; break;	// Uu
		case 0x43: if(capsLock) MAIN_DATA = 'I'; else MAIN_DATA = 'i'; break;	// Ii
		case 0x44: if(capsLock) MAIN_DATA = 'O'; else MAIN_DATA = 'o'; break;	// Oo
		case 0x4D: if(capsLock) MAIN_DATA = 'P'; else MAIN_DATA = 'p'; break;	// Pp
		//LINE 2
		case 0x1C: if(capsLock) MAIN_DATA = 'A'; else MAIN_DATA = 'a'; break;	// Aa
		case 0x1B: if(capsLock) MAIN_DATA = 'S'; else MAIN_DATA = 's'; break;	// Ss
		case 0x23: if(capsLock) MAIN_DATA = 'D'; else MAIN_DATA = 'd'; break;	// Dd
		case 0x2B: if(capsLock) MAIN_DATA = 'F'; else MAIN_DATA = 'f'; break;	// Ff
		case 0x34: if(capsLock) MAIN_DATA = 'G'; else MAIN_DATA = 'g'; break;	// Gg
		case 0x33: if(capsLock) MAIN_DATA = 'H'; else MAIN_DATA = 'h'; break;	// Hh
		case 0x3B: if(capsLock) MAIN_DATA = 'J'; else MAIN_DATA = 'j'; break;	// Jj
		case 0x42: if(capsLock) MAIN_DATA = 'K'; else MAIN_DATA = 'k'; break;	// Kk
		case 0x4B: if(capsLock) MAIN_DATA = 'L'; else MAIN_DATA = 'l'; break;	// Ll
		//LINE 3
		case 0x1A: if(capsLock) MAIN_DATA = 'Z'; else MAIN_DATA = 'z'; break;	// Zz
		case 0x22: if(capsLock) MAIN_DATA = 'X'; else MAIN_DATA = 'x'; break;	// Xx
		case 0x21: if(capsLock) MAIN_DATA = 'C'; else MAIN_DATA = 'c'; break;	// Cc
		case 0x2A: if(capsLock) MAIN_DATA = 'V'; else MAIN_DATA = 'v'; break;	// Vv
		case 0x32: if(capsLock) MAIN_DATA = 'B'; else MAIN_DATA = 'b'; break;	// Bb
		case 0x31: if(capsLock) MAIN_DATA = 'N'; else MAIN_DATA = 'n'; break;	// Nn
		case 0x3A: if(capsLock) MAIN_DATA = 'M'; else MAIN_DATA = 'm'; break;	// Mm
		//SPECIAL KEYS
		case 0x29: MAIN_DATA = ' '; break;										// Space Key
		case 0x12: 	// Left Shift Key
		case 0x59:	// Right Shift Key
			if(shift) {
				shift = 0; 
				capsLock = 0;
			} else {
				shift = 1;
				capsLock = 1;
			}
			MAIN_DATA = 0x00;
			break;
		case 0x16: page = 0; paintPage(page); MAIN_DATA = 0x00; break;	// N1
		case 0x1E: page = 1; paintPage(page); MAIN_DATA = 0x00; break;	// N2
		case 0x26: page = 2; paintPage(page); MAIN_DATA = 0x00; break;	// N3
		case 0x25: page = 3; paintPage(page); MAIN_DATA = 0x00; break;	// N4
		case 0x66: 	// BackSpace
			if(count[page] > 0) 
				count[page]--; 
			pages[page][count[page]] = ' '; 
			if(page > 0 && count[page] == 0)
				page--;
			MAIN_DATA = ' '; 
			break;
		case 0x05: flag = 1; MAIN_DATA = 0x00; break;	//F1 Key
		case 0x06: flag = 2; MAIN_DATA = 0x00; break;	//F2 Key
		case 0x04: flag = 3; MAIN_DATA = 0x00; break;	//F3 Key
		case 0x09: flag = 0; MAIN_DATA = 0x00; break;	//F10 Key
		case 0x0C: paintName(); MAIN_DATA = 0x00; break;	// Call Function paintName()
		default: flag = 0; break;	// Unknown Key
	}
}

void readKeyboard(void){
	unsigned char i, ReadKeybTemp;
	ReadKeybTemp = 0x01;
	MAIN_DATA = 0x00;
	
	while(ECLOCK == 1)
	{//Waits for CLK falling edge */
	//__RESET_WATCHDOG(); /* feeds the dog */
	}
			
	for(i = 0; i < 8; i++){
		
		while(ECLOCK == 0){//Waits for CLK recovery after start */
			 //__RESET_WATCHDOG(); /* feeds the dog */
		}
		while(ECLOCK == 1){//Waits for CLK falling edge */
			//__RESET_WATCHDOG(); /* feeds the dog */
		}
		
		if (EDATA == 1){//Assign a bit equal to 1
			MAIN_DATA = MAIN_DATA | ReadKeybTemp;
			ReadKeybTemp = ReadKeybTemp * 2;
			
		}else{//Assign a bit equal to 0
			ReadKeybTemp = ReadKeybTemp * 2;
		}
	}
	
	while(ECLOCK == 0)
	{//Waits for CLK recovery D0-D7 */
		//__RESET_WATCHDOG(); /* feeds the dog */
	}
	while(ECLOCK == 1)
	{//Waits for CLK falling edge */
		//__RESET_WATCHDOG(); /* feeds the dog */
	}
	while(ECLOCK == 0)
	{//Waits for CLK rising edge */
		//__RESET_WATCHDOG(); /* feeds the dog */
	}
	while(ECLOCK == 1)
	{//Waits for CLK falling edge */
		//__RESET_WATCHDOG(); /* feeds the dog */
	}
	while(ECLOCK == 0)
	{//Waits for CLK rising edge */
		//__RESET_WATCHDOG(); /* feeds the dog */
	}
	
}

void PIT_IRQHandler()
{
       static int Counter = 0;
       Counter++;                                   // Each increment represents 500us
       switch (Counter)
    {
            case 1:
            GPIOA_PSOR |= 1<<13;                 // Set EN pin high
                     break;
            case DATA_UPDATE_PERIOD:                  // 100ms passed
            Counter = 0;                         // Clear Counter at the end of the sample period
                     break;
            default:
                     break;
    }
       PIT_TFLG0 |= PIT_TFLG_TIF_MASK;              // Clear PIT interrupt flag
}

void configPorts(void){
	
	//Turn on clock for portb
	SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;     // Enable the clock to the PIT module
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
	
    /* Set pins of PORTA as GPIO */
    PORTA_PCR1= PORT_PCR_MUX(2);
    PORTA_PCR2= PORT_PCR_MUX(2);
	
	PORTC_PCR1|= PORT_PCR_MUX(1);
			
	//Set PTC3 as RTC_CLKOUT - check pinouts table
	PORTC_PCR3|= PORT_PCR_MUX(5);
	
	//Initialize PortB and PortD and PortE
	GPIOB_PDOR = 0x00;
	GPIOD_PDOR = 0x00;
	GPIOC_PDOR = 0x00;
	GPIOE_PDOR = 0x00;
	GPIOA_PDOR = 0x00;
	
	//Configure Port as outputs input 0, output 1	
	GPIOD_PDDR = 0xFFFF;
	GPIOB_PDDR = 0xFF;
	GPIOC_PDDR = 0x00;
	GPIOE_PDDR = 0xF0;	
}

void initLCD(void) {
	sendCode(nInst, 0x38);
	sendCode(nInst, 0x38);
	sendCode(nInst, 0x38);
	sendCode(nInst, 0x0C);
	sendCode(nInst, 0x01);
}

void uart0_putchar(char data)
{
	/* Warten bis Speicher verfügbar im FIFO */
	while(!(UART0_S1 & UART_S1_TDRE_MASK));
	/* Send the character */
	UART0_D =  data;
}

void delay(long time)
{
    while (time > 0)
    {
        time--;
    }
}

void InitUART(void)
{
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
     
    /* PORTA_PCR1: ISF=0,MUX=2 */
    PORTA_PCR1 = (uint32_t)((PORTA_PCR1 & (uint32_t)~0x01000500UL) | (uint32_t)0x0200UL);
    /* PORTA_PCR2: ISF=0,MUX=2 */
    PORTA_PCR2 = (uint32_t)((PORTA_PCR2 & (uint32_t)~0x01000500UL) | (uint32_t)0x0200UL);
     
    /* Disable TX & RX while we configure settings */
    UART0_C2 &= ~(UART0_C2_TE_MASK); //disable transmitter
    UART0_C2 &= ~(UART0_C2_RE_MASK); //disable receiver
     
    /* UART0_C1: LOOPS=0,DOZEEN=0,RSRC=0,M=0,WAKE=0,ILT=0,PE=0,PT=0 */
    UART0_C1 = 0x00U; /* Set the C1 register */
    /* UART0_C3: R8T9=0,R9T8=0,TXDIR=0,TXINV=0,ORIE=0,NEIE=0,FEIE=0,PEIE=0 */
    UART0_C3 = 0x00U; /* Set the C3 register */
    /* UART0_S2: LBKDIF=0,RXEDGIF=0,MSBF=0,RXINV=0,RWUID=0,BRK13=0,LBKDE=0,RAF=0 */
    UART0_S2 = 0x00U; /* Set the S2 register */
     
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1); //set clock source to be from PLL/FLL
    SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL(0b100);
    unsigned SBR = 546;//137; //Set the baud rate register, SBR = 137
    UART0_BDH |= (~UART0_BDH_SBR_MASK) | SBR >> 8;
    UART0_BDL |= (~UART0_BDL_SBR_MASK) | SBR;
     
    char OSR = 3; //set the oversampling ratio to option #3 = 4x
    UART0_C4 &= (~UART0_C4_OSR_MASK) | OSR;
     
    UART0_C5 |= UART0_C5_BOTHEDGE_MASK; //enable sampling on both edges of the clock
    UART0_C2 |= UART0_C2_TE_MASK; //enable transmitter
    UART0_C2 |= UART0_C2_RE_MASK; //enable receiver 
}

void sendCode(int code, int data){
	RS_0;
	Enable_0;
	Port_LCD = data;
	
	if(code == nData) RS_1;
	Enable_1;
	delay(segundos);
	Enable_0;
}

void clearLCD(void)
{
	sendCode(nInst, 0x01);
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

}

void timer(void)
{
	switch(flag)
	{
	case 1: // F1 KEY
		if(f1state == 0) {
			clearLCD();
			f1state = 1;
		} else {
			f1state = 0;
			paintPage(page);
		}
		break;
	case 2: // F2 KEY
			if(f2state2 == 0) {
				if(f2state == 0) clearLCD();
				f2state++;
				if(f2state == 2) f2state2 = 1;
			}
			if(f2state2 == 1) {
				if(f2state == 0) paintPage(page);
				f2state++;
				if(f2state == 2) f2state2 = 0;
			}
			if(f2state > 1) f2state = 0;
		break;
	case 3:
		clearLCD();
		delay(900000);
		paintPage(page);
		break;
	default:
		break;
	}
}

