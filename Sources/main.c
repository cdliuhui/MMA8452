#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */

#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library	
#define   uchar unsigned char
#define   uint unsigned int	

#define  SCL   PTS_PTS3      //I2C CLK pin define 
#define  SDA 	 PTS_PTS2      //I2C data pin define

#define	SlaveAddress   0x38	//define the slave device address



  	

void delay(unsigned int k);
void Init_acceleration(void);             //init acceleration


void  Single_Write_acceleration(uchar REG_Address,uchar REG_data);   //single write acceleration sensor
uchar Single_Read_acceleration(uchar REG_Address);                   //single read acceleration sensor
void Multiple_read_acceleration(void);                                //continuous read acceleration sensor
void Delay5us();
void Delay5ms();
void acceleration_Start();
void acceleration_Stop();
void acceleration_SendACK(uchar ack);
uchar  acceleration_RecvACK();
void acceleration_SendByte(uchar dat);
uchar acceleration_RecvByte();

uchar CY;
uchar  RX_DATA;
int BUF[50];
uchar test;
float acc_x,acc_y,acc_z;
float ax,ay,az;
//-----------------------------------




/*******************************/
void delay(unsigned int k)	
{						
unsigned int i,j;				
for(i=0;i<k;i++)
{			
for(j=0;j<121;j++)			
{;}}						
}


/**************************************

**************************************/
void Delay5us()	//delay 5us
{
    asm NOP;
    asm NOP;
    asm NOP;
    asm NOP;
    asm NOP;
    asm NOP;
    asm NOP;
    asm NOP;
    asm NOP;
}

/**************************************

**************************************/
void Delay5ms()	//delay 5ms
{
    uint n = 5000;

    while (n--);
}

/**************************************
start signal
**************************************/
void acceleration_Start()
{
    DDRS_DDRS2=1;
    Delay5us();
    SDA = 1;                    
    SCL = 1;                    
    Delay5us();                 
    SDA = 0;                    
    Delay5us();                 
    SCL = 0;                    
}

/**************************************
stop signal
**************************************/
void acceleration_Stop()
{
     DDRS_DDRS2=1;
     Delay5us();
    SDA = 0;                    
    SCL = 1;                   
    Delay5us();                
    SDA = 1;                    
    Delay5us();               
}

/**************************************
send ack signal

**************************************/
void acceleration_SendACK(uchar ack)
{
    DDRS_DDRS2=1;
    Delay5us();
    SDA = ack;                  //write the ack signal 
    SCL = 1;                    
    Delay5us();                
    SCL = 0;                    
    Delay5us();                 
}

/**************************************
receive ack signal
**************************************/
uchar acceleration_RecvACK()
{

     
    DDRS_DDRS2=0;
    Delay5us(); 
    SCL = 1;                                   
    CY = SDA;                   //read the ack signal
    SCL = 0;                    
    Delay5us();                 

    return CY;
}

/**************************************

**************************************/
void acceleration_SendByte(uchar dat)
{
    uchar i;
    DDRS_DDRS2=1;
    Delay5us();
    for (i=0; i<8; i++)        
    {
        if(dat&0x80)
         CY=1;
        else
         CY=0;            
        SDA = CY;              
        SCL = 1;              
        Delay5us();             
        SCL = 0;               
        Delay5us(); 
        dat <<= 1;             
    }
    acceleration_RecvACK();
}

/**************************************

**************************************/
uchar acceleration_RecvByte()
{
    uchar i;
    uchar dat = 0;
      SDA = 1;
     DDRS_DDRS2=0;
      Delay5us();
                       
    for (i=0; i<8; i++)         
    {
        dat <<= 1;
        SCL = 1;                
        Delay5us();             
        dat |= SDA;                       
        SCL = 0;                
        Delay5us();            
    }
    return dat;
}

//*********************************************************

void Single_Write_acceleration(uchar REG_Address,uchar REG_data)
{
    acceleration_Start();                  
    acceleration_SendByte(SlaveAddress);   
    acceleration_SendByte(REG_Address);    
    acceleration_SendByte(REG_data);      
    acceleration_Stop();                  
}



//*****************************************************

uchar Single_Read_acceleration(uchar REG_Address)
{  uchar REG_data;
    acceleration_Start();                          
    acceleration_SendByte(SlaveAddress);          
    acceleration_SendByte(REG_Address);                  
    acceleration_Start();                         
    acceleration_SendByte(SlaveAddress+1);         
    REG_data=acceleration_RecvByte();              
	acceleration_SendACK(1);   
	acceleration_Stop();                          
    return REG_data; 
}

//*********************************************************7

//*********************************************************
void Multiple_read_acceleration(void)
{   uchar i;
    acceleration_Start();                          
    acceleration_SendByte(SlaveAddress);          
    acceleration_SendByte(0x01);                 	
    acceleration_Start();                         
    acceleration_SendByte(SlaveAddress+1);       
	 for (i=0; i<6; i++)                     
    {
        BUF[i] = acceleration_RecvByte();          
        if (i == 5)
        {
           acceleration_SendACK(1);              
        }
        else
        {
          acceleration_SendACK(0);               
       }
   }
    acceleration_Stop();                          
  //  Delay5ms();
    
 
      BUF[6]=((BUF[0]<<8)+BUF[1])>>4;
      
      acc_x=(BUF[6]/2047.00)*2;
      ax=sin(atan(acc_x/sqrt(acc_y*acc_y+acc_z*acc_z)));
      
  
     BUF[7]=((BUF[2]<<8)+BUF[3])>>4;
      acc_y=(BUF[7]/2047.00)*2;
      ay=sin(atan(acc_y/sqrt(acc_x*acc_x+acc_z*acc_z)));
   
      BUF[8]=((BUF[4]<<8)+BUF[5])>>4;
      acc_z=(BUF[8]/2047.00)*2;
      az=sin(atan(acc_z/sqrt(acc_x*acc_x+acc_y*acc_y)));
    
}


//*****************************************************************

//**************************************************************
void Init_acceleration()
{
  
    //Single_Write_acceleration(0x0e,0x10);   //
   // Single_Write_acceleration(0x0f,0x13);   //
  // Single_Write_acceleration(0x2B,0x02);   //
     Single_Write_acceleration(0x2A,0x01);   //
 /***************************demo AN4071******************************/
  //  Single_Write_acceleration(0x2A,0x18);
  //  Single_Write_acceleration(0x1D,0x16); 
  //  Single_Write_acceleration(0x1f,0x08);
  //  Single_Write_acceleration(0x20,0x05);
  //  Single_Write_acceleration(0x2A,Single_Read_acceleration(0x2A)|0x01);
}


//*********************************************************

//*********************************************************


void main(void) {
  /* put your own code here */
  


	EnableInterrupts;
//	PERS_PERS2=1;
//	PPSS_PPSS2=0;
  DDRS_DDRS2=1;
  
// 	PERS_PERS2=1;
//	PPSS_PPSS2=0;
  DDRS_DDRS3=1;

  for(;;) 
  {
   _FEED_COP(); /* feeds the dog */
    //  uchar devid;
  delay(2);	                   //delay power on	
  	

  Init_acceleration();                 //init acceleration

    while(1)                         
    { 

     // Init_acceleration();                
        Multiple_read_acceleration();       //continues read the acceleraiton and stored the buf
        
        delay(50);
       
   
    }
  } /* loop forever */
  /* please make sure that you never leave main */
}
