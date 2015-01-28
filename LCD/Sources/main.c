#include "derivative.h" /* include peripheral declarations */
  
 
//Time definitions
#define nt15_msec   162000
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
  
  
  
  
#define Set_GPIOB_PDOR(x)   (GPIOB_PDOR |= (1 << (x-1)))
  
int int_Temp;
  
//Cursor Blink off initialization
const unsigned char InitializeLCD[5] = {0x38, 0x38, 0x38, 0x0C, 0x01};
//--------------------------------------------------------------
//Declare Prototypes
/* Functions */
void cfgPorts(void);
void initLCD(void);
void delay(long time);
void sendCode(int Code, int Data);
  
/*@description: Initial Port Cfg
*/
              
int main(void)
{
    //Configure ports
    cfgPorts();
    //Initialize LCD
    initLCD();
    //Set position to print character
    sendCode(nIns, 0x85);
    sendCode(nIns, 0x85);
    sendCode(nIns, 0x85);
    //Print character
    sendCode(nData, 'I');
    sendCode(nData, 'M');
    sendCode(nData, 'A');
    sendCode(nData, 'N');
    sendCode(nData, 'O');
    sendCode(nData, 'L');
 
    /*sendCode(nIns, 0x40);
    sendCode(nData,0x1F);
    sendCode(nData,0x15);
    sendCode(nData,0x1F);
    sendCode(nData,0x4);
    sendCode(nData,0xE);
    sendCode(nData,0x4);
    sendCode(nData,0x4);
    sendCode(nData,0xA);
    sendCode(nIns,0x8B);
    sendCode(nData,0x00);*/
     
      
  
    for(;;)
    {/* The logic for the buttons works if a pull-down
        resistor is used */
          
   
    }
      
    return 0;
}
  
void cfgPorts(void)
{
    //Turn on clock for portb
    SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;  
    //Turn on clock for portd
    SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; 
    ////Turn on clock for portc
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
      
    /* Set pins of PORTB as GPIO */
    PORTB_PCR0= PORT_PCR_MUX(1);
    PORTB_PCR1= PORT_PCR_MUX(1);
    PORTB_PCR2=(0|PORT_PCR_MUX(1));
    PORTB_PCR3=(0|PORT_PCR_MUX(1));
    PORTB_PCR4=(0|PORT_PCR_MUX(1));
    PORTB_PCR5=(0|PORT_PCR_MUX(1));
    PORTB_PCR6=(0|PORT_PCR_MUX(1));
    PORTB_PCR7=(0|PORT_PCR_MUX(1));
      
    /* Set pins of PORTC as GPIO */
    PORTC_PCR0= PORT_PCR_MUX(1);
    PORTC_PCR1= PORT_PCR_MUX(1);
    PORTC_PCR2= PORT_PCR_MUX(1);
    PORTC_PCR3= PORT_PCR_MUX(1);
      
    /* Set pins of PORTD as GPIO */
    PORTD_PCR0= PORT_PCR_MUX(1);
    PORTD_PCR1= PORT_PCR_MUX(1);
    PORTD_PCR2=(0|PORT_PCR_MUX(1));
    PORTD_PCR3=(0|PORT_PCR_MUX(1));
    PORTD_PCR4=(0|PORT_PCR_MUX(1));
    PORTD_PCR5=(0|PORT_PCR_MUX(1));
    PORTD_PCR6=(0|PORT_PCR_MUX(1));
    PORTD_PCR7=(0|PORT_PCR_MUX(1));
      
    //Initialize PortB
    GPIOB_PDOR = 0x00;
      
    //Initialize PortD
    GPIOD_PDOR = 0x00;
  
    //Configure PortB as outputs
    GPIOB_PDDR = 0xFF;
      
    //Configure PortD as outputs
    GPIOD_PDDR = 0xFF;
      
    //Configure PortC as inputs
    GPIOC_PDDR = 0x00;
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
