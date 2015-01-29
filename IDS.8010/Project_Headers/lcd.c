	
#include "derivative.h"
#include "lcd.h"

void cfgPortsLCD(void) 
{ 
    //Turn on clock for portD and portB 
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
    //Also, we can use predefined masks created by freescale, such as: 
    //SIM_SCGC5_PORTB_MASK; 
    
	/* Set pins of PORTD as GPIO */
	PORTD_PCR0=(PORT_PCR_MUX(1)); 	//LED
	
    /* Set pins of PORTC as GPIO */
		
    PORTC_PCR0=(PORT_PCR_MUX(1)); 	//D0
    PORTC_PCR1=(PORT_PCR_MUX(1));	//D1
    PORTC_PCR2=(PORT_PCR_MUX(1)); 	//D2
    PORTC_PCR3=(PORT_PCR_MUX(1)); 	//D3
    PORTC_PCR4=(PORT_PCR_MUX(1)); 	//D4
    PORTC_PCR5=(PORT_PCR_MUX(1)); 	//D5
    PORTC_PCR6=(PORT_PCR_MUX(1)); 	//D6
    PORTC_PCR7=(PORT_PCR_MUX(1));	//D7
	/* Set pins of PORTB as GPIO */
    PORTC_PCR9=(PORT_PCR_MUX(1)); 	//E
    PORTC_PCR8=(PORT_PCR_MUX(1)); 	//RS
   
      
    //Reasure first PortB and PortD value 
    GPIOC_PDOR = 0x000;
 
    //Reasure first PortD value 
        GPIOD_PDOR = 0x000;
    //Configure PortC as outputs 
    GPIOC_PDDR |= 0xFFF;
    //Configure PortD as outputs 
       GPIOD_PDDR |= 0xFFF;
   
	initLCD();
} 
void delay (long time) 
{  
    x = time; 
    while(x>0) 
    { 
    x--; 
    } 
}
void initLCD(void) 
{ 
    delay(ntime_15msec); 
    //Commands = {0x38, 0x38, 0x38, 0x0C, 0x01}; 
    print_lcd(0, 0b00001110); // prender lcd, activar cursor, desactivar parpadeo
    delay(ntime_40usec);
	print_lcd(0, 0b00000110); // activar mover cursor a la derecha
	delay(ntime_40usec);
	print_lcd(0, 0b00111000); //bus de 8 bits, 4 lineas , fuente 5x8
	delay(ntime_40usec);
}  
void print_lcd(char Code, char Data) 
{   
    PortRS_0; 
    PortEnable_0; 
      
    PortLCD = Data;
      
    if (Code == 0)// 0 instruccion 
    { 
        PortRS_0; 
		PortEnable_1;
		delay(ntime_40usec);
		PortEnable_0;
		PortRS_0;
		delay(ntime_15msec);
    } 
    else if (Code == 1)// 1 dato 
    { 
        PortRS_1; 
		PortEnable_1; 
        delay(ntime_40usec); 
        PortEnable_0; 
        PortRS_0; 
		delay(ntime_15msec);
	}
} 

void printstr_lcd (char str[16])
{
	int i=0;
	while(str[i] != '\0')
	{
		print_lcd(1,str[i]);
		delay(ntime_15msec);
		i++;
	}
}

void clrscr(void)
{
	print_lcd(0,1);
}

void lcd_gotoxy(int x,int y) {
   int address;

   switch(y) {
     case 1 : address=0x80;break;
     case 2 : address=0xc0;break;
     case 3 : address=0x94;break;
     case 4 : address=0xd4;break;
   }
   address+=x-1;
   print_lcd(0,address);
}

void printvarxy_lcd(int x,int y,int data)
{
	lcd_gotoxy(x,y);
	PortRS_0; 
	PortEnable_0; 
	//Assign the value we want to send to the LCD
	PortLCD = data;   
	
	 PortRS_1; 
	 PortEnable_1; 
	 delay(ntime_40usec); 
	 PortEnable_0; 
	 PortRS_0; 
	 delay(ntime_15msec);
	
}

void RTC(int multi)
		{
			GPIOD_PDOR=0x000;
			delay(120000);
			GPIOD_PDOR=0x001;
			delay(120000);
		}


