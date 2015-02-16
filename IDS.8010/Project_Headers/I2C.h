


#define MWSR                   0x00  /* Master write  */
#define MRSW                   0x01  /* Master read */
#define i2c_DisableAck()       I2C1_C1 |= I2C_C1_TXAK_MASK
#define i2c_EnableAck()        I2C1_C1 &= ~I2C_C1_TXAK_MASK
#define i2c_RepeatedStart()    I2C1_C1 |= I2C_C1_RSTA_MASK
#define i2c_EnterRxMode()      I2C1_C1 &= ~I2C_C1_TX_MASK
#define i2c_write_byte(data)   I2C1_D = data

#define i2c_Start()            I2C1_C1 |= I2C_C1_TX_MASK;\
                               I2C1_C1 |= I2C_C1_MST_MASK

#define i2c_Stop()             I2C1_C1 &= ~I2C_C1_MST_MASK;\
                               I2C1_C1 &= ~I2C_C1_TX_MASK

#define i2c_Wait()               while((I2C1_S & I2C_S_IICIF_MASK)==0) {} \
                                  I2C1_S |= I2C_S_IICIF_MASK;

void Init_I2C(void);
void IIC_StartTransmission (char SlaveID, char Mode);
void I2CWriteRegister(char SlaveID, char u8RegisterAddress, char u8Data);
char I2CReadRegister(char SlaveID, char u8RegisterAddress);
void I2CWriteRegister8(char SlaveID, char u8RegisterAddress, char u8Data);
char I2CReadRegister8(char SlaveID, char u8RegisterAddress);
