/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "lcd.h"

/*
 * Bus de datos LCD
 * PTC0 -> PTC7 Datos
 * PTC8 -> Rs
 * PTC9 -> E
 * */

char name [16]={"ms"};
//int cnt=0;

int main(void)
 {
unit=0;	
cfgPortsLCD();	
clrscr();//funcion
for(;;)
{
	 
	unit=0x30;
	for(unit=0x30;unit<=0x39;unit++) 
	{
		
		
		
		
		//RTC(1);
			
		print_lcd(1,'0');
		print_lcd(1,'.');
		print_lcd(1,unit);
		printstr_lcd(name);
		lcd_gotoxy(1,1);
		GPIOD_PDOR=0x000;
		delay(60000);
		GPIOD_PDOR=0x001;
		delay(60000);
		
		// 
	/*	printstr_lcd(name);
	 *
	*/	
			
		   
		 
		}
}
	return 0;
}

