/*
 * i2c.c
 *
 *  Created on: Feb 9, 2015
 *      Author: Edgar
 */
#include "derivative.h"
#include "i2c.h"

void Init_I2C(void)
{
  SIM_SCGC5 = SIM_SCGC5_PORTE_MASK ;
  SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;
  PORTE_PCR0 = PORT_PCR_MUX(6) | PORT_PCR_PE_MASK;
  PORTE_PCR1 = PORT_PCR_MUX(6) | PORT_PCR_PE_MASK;
  I2C1_F  = 0x14;
  I2C1_S= 0x00;
  I2C1_C1=0x18;
  I2C1_C1 |= I2C_C1_IICEN_MASK;
  I2C1_C1|= I2C_C1_IICIE_MASK;

  I2C1_C1|= I2C_C1_MST_MASK;
  I2C1_D=0x55;
  I2C1_C2=0x00;
  I2C1_FLT=0x00;
  
  
}

/* Inicia  Transmicion i2c
 * SlaveID: 1010 {A0-A2}
 * "Mode" define  Read (1)  Write (0)
 */
void IIC_StartTransmission (char SlaveID, char Mode)
{
  SlaveID = SlaveID << 0;
  SlaveID |= (Mode & 0x01);
  i2c_Start();
  i2c_write_byte(SlaveID);
}

void Pause(void){
    int n;
    for(n=1;n<50;n++) {
      asm("nop");
    }
}

/* Lee un Registro I2C
 * SlaveID: 
 * RegisterAddress: Direccion del Registro
 * Regresa valor del registro
 */
char I2CReadRegister(char SlaveID, char RegisterAddress)
{
  char result;
//Recorrer la direccion >> , despues &0xff
  IIC_StartTransmission(SlaveID,MWSR);
  i2c_Wait();
  i2c_write_byte(RegisterAddress>>8); 
  i2c_Wait();
  i2c_write_byte(RegisterAddress&0XFF); 
  i2c_Wait();
  i2c_RepeatedStart();
  i2c_write_byte((SlaveID << 1) | 0x01);
  i2c_Wait();
  i2c_EnterRxMode();
  i2c_DisableAck(); 
  result = I2C0_D ; // lectura
  i2c_Wait();
  i2c_Stop(); 
  result = I2C0_D ; 
  Pause();
  return result;
}


void I2CWriteRegister(char SlaveID, char RegisterAddress, char Data)
{
  IIC_StartTransmission(SlaveID,MWSR);
  i2c_Wait();
  i2c_write_byte(RegisterAddress>>8);
  i2c_Wait();
  i2c_write_byte(RegisterAddress&0XFF);
  i2c_Wait();
  i2c_write_byte(Data);
  i2c_Wait();
  i2c_Stop();
  Pause();
}

/* Lee un Registro I2C
 * SlaveID: 
 * RegisterAddress: Direccion del Registro
 * Regresa valor del registro
 * MEM8
 */
char I2CReadRegister8(char SlaveID, char RegisterAddress)
{
  char result;
//Recorrer la direccion >> , despues &0xff
  IIC_StartTransmission(SlaveID,MWSR);
  i2c_Wait();
  i2c_write_byte(RegisterAddress); 
  i2c_Wait();
  i2c_RepeatedStart();
  i2c_write_byte((SlaveID << 1) | 0x01);
  i2c_Wait();
  i2c_EnterRxMode();
  i2c_DisableAck(); // Deshabilita ACK 
  result = I2C0_D ; // lectura
  i2c_Wait();
  i2c_Stop(); // Envia STOP 
  result = I2C0_D ; 
  Pause();
  return result;
}

/* Escribe un valor
 * SlaveID:
 * RegisterAddress: 
 * Data: 
 */
void I2CWriteRegister8(char SlaveID, char RegisterAddress, char Data)
{
  IIC_StartTransmission(SlaveID,MWSR);
  i2c_Wait();
  i2c_write_byte(RegisterAddress);
  i2c_Wait();
  i2c_write_byte(Data);
  i2c_Wait();
  i2c_Stop();
  Pause();
}
void I2C1_IRQHandler(){
	
	
	 I2C1_S|=I2C_S_IICIF_MASK;
	 
}
