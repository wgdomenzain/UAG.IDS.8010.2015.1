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

char name [16]={"BiancaJ"};
//int cnt=0;

int main(void)
{
	for(;;) {	
	cfgPortsLCD();
	
	clrscr();//funcion 
	printstr_lcd(name);
	lcd_gotoxy(1,2);// 
/*	printstr_lcd(name);
*/
	
	
	   
	 
	}
	
	return 0;
}

