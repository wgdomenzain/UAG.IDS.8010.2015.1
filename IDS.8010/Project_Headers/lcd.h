//LCD.h

#ifndef lcd_H_
#define GPIO_PIN_MASK 0x1Fu 
#define GPIO_PIN(x) (((1)<<(x & GPIO_PIN_MASK)))
#define PortLCD         GPIOC_PDOR  
#define PortEnable_1    FGPIOC_PSOR |= (1<<9) 
#define PortEnable_0    FGPIOC_PCOR |= (1<<9) 
#define PortRS_1        FGPIOC_PSOR |= (1<<8) 
#define PortRS_0        FGPIOC_PCOR |= (1<<8) 
#define ntime_1seg      420000
#define ntime_15msec    3500
#define ntime_10usec    12
#define ntime_40usec    48  
#define ntime_100ms     240000
void cfgPortsLCD(void); //configuracion de puertos de la LCD
void delay(long time);  //retardo necesario para el control de la LCD
void initLCD(void); //sintaxis de inicializacion del LCD
void print_lcd(char Code, char Data); //sintaxis de envio de datos 0 => instruccion,1 => dato
int x,i,unit; //contadores
void printstr_lcd(char str[16]);
void clrscr(void);
void printvarxy_lcd(int x,int y,int data);
void lcd_gotoxy(int x,int y);
void RTC(int multi);
#endif /* lcd_H_ */
