#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include  "lcd.h"
#include  "delay.h"


unsigned char b1[8]={0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10};
unsigned char b2[8]={0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18};
unsigned char b3[8]={0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C,0x1C};
unsigned char b4[8]={0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E,0x1E};
unsigned char b5[8]={0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};


unsigned char const LCD_INIT_STRING[4] =
{
 0x28,// | (lcd_type << 2), // Func set: 4-bit, 2 lines, 5x8 dots
 0xc,                    // Display on
 1,                      // Clear display
 6                       // Increment cursor
 };
//************************************************************************
void writeLCD( unsigned char addr,  char n ) 
{
       if(addr)
         LCD_RS = 1;
       else
         LCD_RS = 0;


      sndNibbleLCD(n >> 4);
      sndNibbleLCD(n & 0xf);
}

//*************************************************************************
void sndNibbleLCD( unsigned char nibble )
{
  
     LCD_DB4 = !!(nibble & 1);
     LCD_DB5 = !!(nibble & 2);
     LCD_DB6 = !!(nibble & 4);
     LCD_DB7 = !!(nibble & 8);
  
     
      delay_ms(1);
      LCD_E = 1;
      delay_ms(1);
      LCD_E = 0;
}

//*************************************************************************
void initLCD()
{
  
  int i;
  
  PM14_bit.no0=0;  //RS y Enable
  PM14_bit.no1=0;
  
  P0   = 0x00U;
  PMC0 = 0xF3;
  PM0  = 0xF0U;    //P00..04 out
  
    delay_ms(30);
    LCD_RS = 0;
    LCD_E = 0;
    delay_ms(15);
    for(i=1;i<=3;++i) {
       sndNibbleLCD(0x03);
       delay_ms(5);
    }
    sndNibbleLCD(0x02);
    for(i=0;i<=sizeof(LCD_INIT_STRING)-1;++i)
    { 
       writeLCD(LCDCMD,LCD_INIT_STRING[i]);
       delay_ms(5);
    }    
    putsLCD("\f");
    delay_ms(5);

    LCD_RS = 1;
    
     setCGRAM(0,b1);  //carga nuevos caracteres.
     setCGRAM(1,b2);
     setCGRAM(2,b3);
     setCGRAM(3,b4);
     setCGRAM(4,b5);
     //setCGRAM(5,b5);

} 

//**********************************************************************************

void lcd_gotoxy(unsigned char x,unsigned char y)
{
	unsigned char address;

	if(y != 1)
	   address = lcd_line_two;
	else
	   address=0;

	address += x-1;
	cmdLCD( 0x80 | address);
}
//*************************************************************************************
void putsLCD(const char *s)
{

    while(*s)
    {
        switch (*s)
        {
        case '\n':          // point to second line
                  lcd_gotoxy(1,2);
                  break;
        case '\f':
                 
                 homeLCD();
		 delay_ms(2);

                  break;
        case '\r':          // home, point to first line
                 
                 clrLCD();
                 break;
        default:            // print character
           
            putLCD(*s);
            break;
        } //switch
        s++;
        delay_ms(1);
    } //while

}

//********************************************************************************************

void setCGRAM(unsigned char addr,unsigned char *bmp)
{
    int j;


    setLCDG(addr * 8);

    for(j=0;j<8;j++)
        putLCD(bmp[j]);

    setLCDC( 0x40 );

}


//*************************************************************************
void drawProgressBar(int index,int imax,int size)
{
   
   unsigned char i;
    int pixelprogress;
     unsigned char  c;

    pixelprogress=((index*(size*5))/imax);

    for(i=0;i<size;i++) {
       if((i*5)+5 > pixelprogress)
       {
           if(i*5 > pixelprogress)
               c=0;
           else
               c=pixelprogress % 5;
       }
       else
           c=4;

     if(!c)break;

     putLCD(c);
    }

    for(i=0;i<=size;i++)  //borra resto
        putLCD(' ');
}
