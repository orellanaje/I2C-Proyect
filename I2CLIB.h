/*Libreia tomada de:
 * LIBRERÍA I2C ELABORADA POR ELECTROSOME
 */

//MASTER FUNCTIONS

void I2C_Master_Init(const unsigned long c)
{
  SSPCON = 0b00101000;            //SSP Module as Master
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ/(4*c))-1; //Setting Clock Speed
  SSPSTAT = 0;
  TRISC3 = 1;                   //Setting as input as given in datasheet
  TRISC4 = 1;                   //Setting as input as given in datasheet
}

//FOR WAITING
void I2C_Master_Wait()
{
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); //Transmit is in progress
}

//START CONDITION
void I2C_Master_Start()
{
  I2C_Master_Wait();    
  SEN = 1;             //Initiate start condition
}

//REPEATED START
void I2C_Master_RepeatedStart()
{
  I2C_Master_Wait();
  RSEN = 1;           //Initiate repeated start condition
}

//STOP CONDITION
void I2C_Master_Stop()
{
  I2C_Master_Wait();
  PEN = 1;           //Initiate stop condition
}

//WRITE DATA
void I2C_Master_Write(unsigned d)
{
  I2C_Master_Wait();
  SSPBUF = d;         //Write data to SSPBUF
}

//READ DATA
unsigned short I2C_Master_Read(unsigned short a)
{
  unsigned short temp;
  I2C_Master_Wait();
  RCEN = 1;
  I2C_Master_Wait();
  temp = SSPBUF;      //Read data from SSPBUF
  I2C_Master_Wait();
  ACKDT = (a)?0:1;    //Acknowledge bit
  ACKEN = 1;          //Acknowledge sequence
  return temp;
}

//SLAVE FUNCTIONS
void I2C_Slave_Init(short address) 
{
  SSPSTAT = 0x80;    
  SSPADD = address; //Setting address
  SSPCON = 0x36;    //As a slave device
  SSPCON2 = 0x01;
  TRISC3 = 1;       //Setting as input as given in datasheet
  TRISC4 = 1;       //Setting as input as given in datasheet
  GIE = 1;          //Global interrupt enable
  PEIE = 1;         //Peripheral interrupt enable
  SSPIF = 0;        //Clear interrupt flag
  SSPIE = 1;        //Synchronous serial port interrupt enable
}

