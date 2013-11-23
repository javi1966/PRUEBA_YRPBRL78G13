#ifndef __LCD_H
#define __LCD_H

#define LCD_RS P14_bit.no0
#define LCD_E  P14_bit.no1
#define LCD_DATA(data) P0 |= (data)
#define LCD_DB4 P0_bit.no0
#define LCD_DB5 P0_bit.no1
#define LCD_DB6 P0_bit.no2
#define LCD_DB7 P0_bit.no3

#define LCD_FUNCTION_SET      0x38 // 0b00110000
#define LCD_FUNCTION_SET_4BIT 0x28 // 0b00101000
#define LCD_DISPLAY_OFF       0x08 // 0b00001000
#define LCD_DISPLAY_ON        0x0F // 0b00001111
#define LCD_DISPLAY_CLEAR     0x01 // 0b00000001
#define LCD_ENTRY_MODE_SET    0x06 // 0b00000110
#define LCD_CURSOR_HOME       0x02 // 0b00000010

#define LCDDATA 1       // address of data register
#define LCDCMD  0       // address of command register
#define putLCD(d) writeLCD(LCDDATA,(d))
#define cmdLCD(c) writeLCD(LCDCMD,(c))
#define homeLCD() writeLCD(LCDCMD,2)
#define clrLCD()  writeLCD(LCDCMD,1)
#define setLCDG(a) writeLCD( LCDCMD,(a & 0x3F) | 0x40)
#define setLCDC(a) writeLCD( LCDCMD,(a & 0x7F) | 0x80)


#define lcd_type     2        // 0=5x7, 1=5x10, 2=2 lines
#define lcd_line_two 0x40 // LCD RAM address for the 2nd line








void initLCD();
void sndNibbleLCD(unsigned char);
void writeLCD(unsigned char,char);
void putsLCD(const char *s);
void setCGRAM(unsigned char addr,unsigned char *bmp);
void lcd_gotoxy(unsigned char x,unsigned char y);
void drawProgressBar(int index,int imax,int size);




#endif 