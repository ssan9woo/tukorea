#include <delay.h>
#include <mega128.h>

#define LCD_WDATA    PORTA    // LCD 데이터 포트 정의
#define LCD_WINST    PORTA

#define LCD_CTRL PORTG    //LCD 제어 포트 정의
#define LCD_EN    0
#define LCD_RW    1
#define LCD_RS    2

#define RIGHT     1
#define LEFT      0


void LCD_PORT_Init(void);
void LCD_Data(unsigned char ch);
void LCD_Comm(unsigned char ch);
void LCD_Delay(unsigned char ms);
void LCD_Char(unsigned char c);
void LCD_Str(unsigned char *str);
void LCD_Pos(unsigned char x, unsigned char y);
void LCD_Clear(void);
void LCD_Init(void);
void LCD_Display_Shift(unsigned char p);
void LCD_Cursor_Shift(unsigned char p);
void LCD_Cursor_Home(void);
