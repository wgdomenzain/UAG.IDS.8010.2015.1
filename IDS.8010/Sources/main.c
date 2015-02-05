#include "derivative.h" /* include peripheral declarations */
 
#define nt15_msec   16200
#define nt40_usec   1600
#define segundos 1600
#define nIns    0
#define nData   1

//MACROS
#define Port_LCD GPIOD_PDOR
#define GPIO_PIN_MASK 0x1Fu
#define GPIO_PIN(x) (((1)<<(x & GPIO_PIN_MASK)))

int counter = 1;
int decena = 0;
int mili=0;
int num=0;
long valor = 0x30717;


#define Enable_1    GPIOB_PDOR |= 0x01
#define Enable_0    GPIOB_PDOR &= 0xFE
#define RS_1        GPIOB_PDOR |= 0x02
#define RS_0        GPIOB_PDOR &= 0xFD
#define ReadBitPortC(x) ((GPIOC_PDIR >> x) & 0x00000001)
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
void imprimeNum(int x, int y);
 
int main(void) {

	int estado = 0;
	cfgPorts();
    initLCD();
    valor=0x30717;
    iniTimer();
    sendCode(nIns, 0x84);
    sendCode(nIns, 0x84);
    
    for (;;) 
    {
    	if(ReadBitPortC(2) == 0){
    		int x;
    		for(x=0;x<150000;x++){    			
    		}
    		if(ReadBitPortC(2) == 0){
    			estado++;
    		if(estado==5)
    		{
    			estado=0;
    		}
    		switch(estado){
    		case 0:
    			mili=0;
    			num=0;
    			sendCode(nIns, 0x01);
    			valor=0;
    			iniTimer();
    			counter = 0;
    			decena = 0;
    			valor = 0x30717;
    			iniTimer();
    			break;
    		case 1:
    			mili=0;
    			num=1;
    			sendCode(nIns, 0x01);
    			valor=0;
    			iniTimer();
    			counter = 0;
    			decena = 0;
    			valor=0x61434;
    			iniTimer();
    			break;
    		case 2:
    			mili=0;
    			num=2;
    			sendCode(nIns, 0x01);
    			valor=0;
    			iniTimer();
    			counter = 0;
    			decena = 0;
    			valor=0x122870;
    			iniTimer();
    			break;
    		case 3:
    			mili=0;
    			num=3;
    			sendCode(nIns, 0x01);
    			valor=0;
    			iniTimer();
    			counter = 0;
    			decena = 0;
    			valor=0x307175;
    			iniTimer();
    			break;
    		case 4:
    			mili=0;
    			num=4;
    			sendCode(nIns, 0x01);
    			valor=0;
    			iniTimer();
    			counter = 0;
    			decena = 0;
    			valor=0x614350;
    			iniTimer();
    			break;
    		}
    		}
    	}
    }
 
    return 0;
}
void delay(long time) {
	while (time > 0) {
		time--;
	}
}
 
void cfgPorts(void) {
 
    SIM_SCGC5 = SIM_SCGC5_PORTB_MASK;
    SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
    SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
    SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;

    //PORT B
    PORTB_PCR0 = PORT_PCR_MUX(1);
    PORTB_PCR1 = PORT_PCR_MUX(1);
    PORTB_PCR2 = PORT_PCR_MUX(1);
    
    //BUTTON
    PORTC_PCR2 = PORT_PCR_MUX(1);
 
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
 
    //Init
    GPIOB_PDOR = 0x00;
    GPIOC_PDOR = 0x00;
    GPIOD_PDOR = 0x00;
    GPIOE_PDOR = 0x00;

    GPIOD_PDDR = 0xFFFF;
    GPIOB_PDDR = 0xFFEFF;
    GPIOE_PDDR = 0xF0;
    GPIOC_PDDR = 0x00;
}
 
void initLCD(void) 
{
 
    sendCode(nIns, 0x38);
    sendCode(nIns, 0x38);
    sendCode(nIns, 0x38);
    sendCode(nIns, 0x0C);
    sendCode(nIns, 0x01);
}
 
 
void sendCode(int code, int data) 
{
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
 
void iniTimer(void)
{
    PIT_MCR = PIT_MCR_FRZ_MASK;    

    PIT_LDVAL1 = valor;
    PIT_TCTRL1 = PIT_TCTRL_TIE_MASK | PIT_TCTRL_TEN_MASK;
     
    NVIC_ICPR |= 1 << ((INT_PIT - 16) % 32);
 
    NVIC_ISER |= 1 << ((INT_PIT - 16) % 32);
}
 
void PIT_IRQHandler(void)
{
    int cantDec = 0;
    sendCode(nIns, 0x01);
    switch(num){
    case 0:
    	mili=mili+25;
    	if(mili==100)
    	{
    		mili=0;
    		counter++;
    		if(counter == 10){
    			counter = 0;
    		    decena++;
    		}    		
    	}    
    	cantDec = count(decena);
    	sendCode(nIns, 0x84);
    	sendCode(nIns, 0x84);
    	if(cantDec == 0){
    		sendCode(nData, '0');
    	} else {
    		imprimeNum(cantDec, decena);
    	 }
    	sendCode(nData, '.');
    	cantDec = count(counter);
    	if(cantDec == 0){
    		sendCode(nData, '0');
    	} else {
    		imprimeNum(cantDec, counter);
    	}
    	if(cantDec==0){
    		sendCode(nData,'0');
    	}
    	else{
    		imprimeNum(cantDec, mili);
    	}
    	PIT_TFLG1 |= PIT_TFLG_TIF_MASK;   
    	 
    	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
    	 
    	break;
    case 1:
    	mili=mili+5;
    	if(mili==10)
    	{
    		counter++;
    		mili=0;
    		if(counter == 10){
    			counter = 0;
    		    decena++;
    		    }    		
    	}    	
    	cantDec = count(decena);
    	sendCode(nIns, 0x84);
    	sendCode(nIns, 0x84);
    	if(cantDec == 0){
    		sendCode(nData, '0');
    	} else {
    		imprimeNum(cantDec, decena);
    	 }
    	sendCode(nData, '.');
    	cantDec = count(counter);
    	if(cantDec == 0){
    		sendCode(nData, '0');
    	} else {
    		imprimeNum(cantDec, counter);
    	}
    	if(cantDec==0){
    		sendCode(nData,'0');
    	}
    	else{
    		imprimeNum(cantDec, mili);
    	}
    	PIT_TFLG1 |= PIT_TFLG_TIF_MASK;    
    	 
    	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
    	break;
    case 2:
    	counter++;
    	if(counter == 10){
        	counter = 0;
        	decena++;
    	}
    	cantDec = count(decena);
    	sendCode(nIns, 0x84);
    	sendCode(nIns, 0x84);
    	if(cantDec == 0){
        	sendCode(nData, '0');
    	} else {
    		imprimeNum(cantDec, decena);
    	}
    	sendCode(nData, '.');
    	cantDec = count(counter);
    	if(cantDec == 0){
        	sendCode(nData, '0');
    	} else {
        	imprimeNum(cantDec, counter);
    	}
    	imprimeNum(cantDec,mili);
    	PIT_TFLG1 |= PIT_TFLG_TIF_MASK;  
 
    	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
    	break;
    case 3:
    	counter=counter+25;
    	if(counter == 100){
        	counter = 0;
        	decena++;
    	}
    	cantDec = count(decena);
    	sendCode(nIns, 0x84);
    	sendCode(nIns, 0x84);
    	if(cantDec == 0){
        	sendCode(nData, '0');
    	} else {
    		imprimeNum(cantDec, decena);
    	}
    	sendCode(nData, '.');
    	cantDec = count(counter);
    	if(cantDec == 0){
        	sendCode(nData, '0');
    	} else {
        	imprimeNum(cantDec, counter);
    	}
    	PIT_TFLG1 |= PIT_TFLG_TIF_MASK;
 
    	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
    	break;
    case 4: 
    	counter=counter+5;
    	if(counter == 10){
        	counter = 0;
        	decena++;
    	}
    	cantDec = count(decena);
    	sendCode(nIns, 0x84);
    	sendCode(nIns, 0x84);
    	if(cantDec == 0){
        	sendCode(nData, '0');
    	} else {
    		imprimeNum(cantDec, decena);
    	}
    	sendCode(nData, '.');
    	cantDec = count(counter);
    	if(cantDec == 0){
        	sendCode(nData, '0');
    	} else {
        	imprimeNum(cantDec, counter);
    	}
    	imprimeNum(cantDec,mili);
    	PIT_TFLG1 |= PIT_TFLG_TIF_MASK;  
 
    	PIT_TCTRL1 |= PIT_TCTRL_TEN_MASK | PIT_TCTRL_TIE_MASK;
    	break;
    }
}

int count(int x)
{
    int res = 0;
     while (x > 0) {
     res++;
     x /= 10;
    }
    return res;
}
 
void imprimeNum(int x, int y)
{
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
