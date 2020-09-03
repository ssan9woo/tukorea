#include <delay.h>
#include <mega128.h>

#include "C:\Users\상우\Desktop\vhd\header\lcd2.h"


void LCD_PORT_Init(void)
{
    DDRA = 0xFF;    //PORTA를 출력으로
    DDRG = 0x0F;    //PORTG의 하위 4비트를 출력으로
}

void LCD_Data(unsigned char ch)
{
    LCD_CTRL |= (1 << LCD_RS);    // RS=1, R/W바=0으로 데이터 쓰기 사이클
    LCD_CTRL &= ~(1 << LCD_RW);
    LCD_CTRL |= (1 << LCD_EN);    //LCD Enable
    delay_us(50);    //시간 지연
    LCD_WDATA = ch;    //데이터 출력
    delay_us(50);    //시간 지연
    LCD_CTRL &= ~(1 << LCD_EN);    //LCD Disable
}

void LCD_Comm(unsigned char ch)
{
    LCD_CTRL &= ~(1 << LCD_RS);    //RS=0으로 명령어 쓰기 사이클
    LCD_CTRL &= ~(1 << LCD_RW);
    LCD_CTRL |= (1 << LCD_EN);    //LCD Enable
    delay_us(50);    //시간 지연
    LCD_WINST = ch;    //명령어 쓰기
    delay_us(50);    //시간 지연
    LCD_CTRL &= ~(1 << LCD_EN);
}

void LCD_Delay(unsigned char ms)
{
    delay_ms(ms);
}

void LCD_Char(unsigned char c)    // 한 문자 출력
{
    LCD_Data(c);
    delay_ms(1);
}

void LCD_Str(unsigned char *str)    //문자열 출력
{
    while (*str != 0) 
    {
        LCD_Char(*str);
        str++;
    }
}

void LCD_Pos(unsigned char x, unsigned char y)    // LCD 위치 설정, x와 y는 행과 열을 의미함
{
    LCD_Comm(0x80 | (x * 0x40 + y));    // x= 가로행(0,1), y = 세로열(0-15)
}

void LCD_Clear(void)    //화면 클리어(1)
{
    LCD_Comm(0x01);
    LCD_Delay(2);
}

void LCD_Display_Shift(unsigned char p)
{
    if(p == RIGHT)
    {
        LCD_Comm(0x1c);
        LCD_Delay(1);
    }
    else if(p == LEFT)
    {
        LCD_Comm(0x18);
        LCD_Delay(1);
    }
}

void LCD_Cursor_Shift(unsigned char p)
{
    if(p == RIGHT)
    {
        LCD_Comm(0x14);
        LCD_Delay(1);
    }
    else if(p == LEFT)
    {
        LCD_Comm(0x10);
        LCD_Delay(1);
    }
}

void LCD_Cursor_Home(void)
{
    LCD_Comm(0x02);
    LCD_Delay(2);
}



void LCD_Init(void)    //LCD 초기화
{
    LCD_PORT_Init();    // LCD 출력 포트 설정

    LCD_Comm(0x38);    //DDRAM, 데이터 8비트 사용, LCD 2열로 사용(6)
    LCD_Delay(4);    //4ms 지연
    LCD_Comm(0x38);    //DDRAM, 데이터 8비트 사용, LCD 2열로 사용(6)
    LCD_Delay(4);    //4ms 지연
    LCD_Comm(0x38);    //DDRAM, 데이터 8비트 사용, LCD 2열로 사용(6)
    LCD_Delay(4);    //4ms 지연
    LCD_Comm(0x0e);    //화면표시 ON/OFF
    LCD_Delay(2);    //2ms 지연
    LCD_Comm(0x06);    //주소+1, 커서를 우측으로 이동(3)
    LCD_Delay(2);    //2ms 지연
    LCD_Clear();    //LCD 화면 클리어
}