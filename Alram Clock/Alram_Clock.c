#include <mega128.h>
#include <delay.h>
#include "C:\Users\상우\Desktop\vhd\header\lcd2.h"


#define FND_data PORTB
#define FND_dig PORTE

#define FND_Null        17
#define FND_Star        14
#define FND_Sharp       15

#define     Do      1908
#define     Re      1700
#define     Mi      1515
#define     Fa      1432
#define     Sol     1275
#define     Ra      1136
#define     Si      1012


unsigned char key_Num = 0;
unsigned char key, sec = 0, mode, min = 0, k, hour, day, month, keynum, alram_key, alram_keynum;    //키값을 받기 위한 변수들과 초기값들 
unsigned int i, j, cnt, year;
unsigned char first[] = "2014-12-05";         //초기화면 세팅 문자열 
unsigned char second[] = "AM 12:00:00";
unsigned char AM[] = "AM";
unsigned char PM[] = "PM";
unsigned char str[] = "Key Setting!!";
unsigned char str2[] = "---Insert Key---";
unsigned char str3[] = "alram set mode!!";
unsigned char str4[] = "--insert alram--";
unsigned char str5[] = "     CLEAR!     ";
unsigned char alram[3][10];  //10자리 2차원배열 
unsigned char str7[] = "Setting Complite!";
unsigned char time[14];      //여기에 시간 세팅한 값 들어옴.
bit timer_flag;              //갹종 화면출력과 딜리트/탈출 을 위한 플래그들 
bit dir, flag_chattering;
bit buzzer_flag;
bit escape_flag;
bit dir;
bit delete_flag;

unsigned char alram_month1, alram_month2, alram_month3, alram_day1, alram_day2, alram_day3, alram_hour1, alram_hour2;  //알람시간을 저장하기 위한 변수들 
unsigned char alram_hour3, alram_min1, alram_min2, alram_min3, alram_sec1, alram_sec2, alram_sec3;

interrupt[EXT_INT0] void ext_int0(void)      //인터럽트 0번 사용 -> PIND.0
{
	buzzer_flag = 0;
	LCD_Clear();
	delay_ms(100);
}

void init_reg(void)
{
	EIMSK = 0x01;           //인터럽트 0사용
	EICRA = 0x02;           //하강엣지 동작 
	DDRB = 0xff;
	DDRC = 0x0f;            // 상위 4bit Col(입력), 하위 4bit Row(출력)
	PORTC = 0x0f;
	DDRG |= (1 << 4);         //부저사용 
	DDRD = 0x00;
	SREG |= 0x80;           //SEI
}
void init_timer(void)
{
	TIMSK = (1 << TOIE0);
	TCCR0 = (1 << CS02) | (1 << CS01) | (1 << CS00); //1024 타이머카운터 0
	TCNT0 = -5;
	SREG |= 0x80;
}

interrupt[TIM0_OVF] void timer0_overflow(void)
{
	TCNT0 = -5;
	if (timer_flag == 0) cnt++;  //timer_flag가 1이면 카운터 동작 멈춤 (시간 설정시)

	if (cnt == 2881)            //시계설정. 1씩증가
	{
		sec++;
		cnt = 0;
	}
	if (sec == 60)
	{
		sec = 0;
		min++;
	}
	if (min == 60)
	{
		min = 0; hour++;
	}
	if (hour == 24)
	{
		hour = 0; day++;
	}
}

void myDelay_us(unsigned int delay)     //부저를 사용하기 위한 딜레이함수 
{
	int i;
	for (i = 0; i < delay; i++)
	{
		delay_us(1);
	}
}

void SSound(int time)                   //부저 함수 
{
	int i, tim;
	tim = 50000 / time;
	for (i = 0; i < tim; i++) // 음계마다 같은 시간 동안 울리도록 time 변수 사용
	{
		PORTG |= 1 << PORTG4; //부저ON, PORTG의 4번 핀 ON(out 1)
		myDelay_us(time);
		PORTG &= ~(1 << PORTG4);  //부저OFF, PORTG의 4번 핀 OFF(out 0)
		myDelay_us(time);
	}
}

unsigned char KeyScan(void)                     // 4X4 키패드 스캔 함수, 출력 값은 10진수 1~16  {
{
	unsigned int Key_Scan_Line_Sel = 0xf7;      // Init_data 하위 니블의 출력을 결정  
	// 하위 니블(4bit)을 스위칭(연속적으로 돌아가면서)하면서 출력    
	unsigned char Key_Scan_sel = 0, key_scan_num = 0;
	unsigned char Get_Key_Data = 0;            // 실제 키 데이터        

	//키스캔 부분  
	for (Key_Scan_sel = 0; Key_Scan_sel < 4; Key_Scan_sel++)
	{
		// 초기화 
		PORTC = Key_Scan_Line_Sel;
		delay_us(10);

		//수신 부분
		Get_Key_Data = (PINC & 0xf0);   // 74LS14의 데이터 취득      

		if (Get_Key_Data != 0x00)
		{
			switch (Get_Key_Data)        // C포트 내용의 상위 니블(4bit)만 받음            
			{
			case 0x10:            // 0001이 맞으면 현재 count값에 4를 곱한후                                        
								  //  1을 더하고 key_scan_num변수에 저장
				key_scan_num = Key_Scan_sel * 4 + 1;
				break;
			case 0x20:            // 0010이 맞으면 현재 count값에 4를 곱한후
								  //  2를 더하고 key_scan_num변수에 저장
				key_scan_num = Key_Scan_sel * 4 + 2;
				break;
			case 0x40:            // 0100이 맞으면 현재 count값에 4를 곱한후 
								  //  3를 더하고 key_scan_num변수에 저장           
				key_scan_num = Key_Scan_sel * 4 + 3;
				break;
			case 0x80:            // 1000이 맞으면 현재 count값에 4를 곱한후                                                      
								  //  4를 더하고 key_scan_num변수에 저장                                         
				key_scan_num = Key_Scan_sel * 4 + 4;
				break;
			default:
				key_scan_num = FND_Null;
				break;
			}
			return key_scan_num;
		}
		Key_Scan_Line_Sel = (Key_Scan_Line_Sel >> 1);
		//Init_data를 시프트 함, Key_Scan_Line 이동     
	}
	return key_scan_num;
}
void time_set(void)
{

	for (k = 0; k <= 13; k++)           //초기화 
	{
		time[k] = 0;
	}
	i = 0;
	LCD_Pos(0, 5);
	LCD_Char('-');

	LCD_Pos(0, 8);
	LCD_Char('-');

	LCD_Pos(1, 6);
	LCD_Char(':');

	LCD_Pos(1, 9);
	LCD_Char(':');
	LCD_Pos(0, 1);

	while (1)
	{


		key = KeyScan();
		if (key == 0)                 //채터링 방지 코드 
		{
			flag_chattering = 0;
		}

		else
		{
			if (!flag_chattering)      //채터링 방지 코드 
			{
				flag_chattering = 1;

				if (key)
				{
					if (key == 16)            //중간 탈출버튼 
					{
						timer_flag = 0;
						break;
					}
					else if (key == 4)        //설정 완료 후 탈출버튼
					{
						escape_flag = 1;
					}
					else if (key == 15)       //한글자 씩 삭제하는 버튼 
					{
						delete_flag = 1;
					}

					if (key == 14)            //0이 안나와서 0을 만들어주려고 사용 
					{
						keynum = 0;
					}
					else if (key % 4 != 0)
					{
						keynum = (key / 4) * 3 + (key % 4);
					}
					else
					{
						keynum = (key / 4) + 9;
					}
					if (delete_flag == 0)
					{
						if (dir == 0)
						{
							time[i] = (keynum + 0x30);     //키값 받고 배열에 넣고 lcd로 출력 
							LCD_Char(time[i]);
							delay_ms(300);
							i++;
						}
						else;

						if (escape_flag == 1)               //설정 다하고 탈출 버튼을 누를 시 
						{
							timer_flag = 0;
							LCD_Clear();     //입력 다하면 clear시키고 setting complite! 띄우기 
							delay_ms(10);
							LCD_Pos(0, 0);
							LCD_Str(str7);   //하고 배열 14개 값들 timer_intr 에서 시계구동. 
							delay_ms(1500);
							LCD_Clear();
							delay_ms(50);

							year = ((time[0] - 0x30) * 1000) + ((time[1] - 0x30) * 100) + ((time[2] - 0x30) * 10) + (time[3] - 0x30);
							month = ((time[4] - 0x30) * 10) + (time[5] - 0x30);        //화면에 각종 날짜/사간 등을 출력하기위해 변수에 저장 
							day = ((time[6] - 0x30) * 10) + (time[7] - 0x30);
							hour = ((time[8] - 0x30) * 10) + (time[9] - 0x30);
							min = ((time[10] - 0x30) * 10) + (time[11] - 0x30);
							sec = ((time[12] - 0x30) * 10) + (time[13] - 0x30);
							escape_flag = 0;
							dir = 0;
							break;
						}
						if (i == 4)  LCD_Pos(0, 6);     //pos값 조절 
						if (i == 6)  LCD_Pos(0, 9);
						if (i == 8)  LCD_Pos(1, 4);
						if (i == 10) LCD_Pos(1, 7);
						if (i == 12) LCD_Pos(1, 10);
						if (i == 14) dir = 1;
					}
					else if (delete_flag == 1)         //삭제를 위한 코드 ↓
					{
						if ((i >= 1) && (i <= 4))      //pos값이 자꾸 바뀌기 때문에 변수 위치마다 딜리트하는 위치(pos)를 설정하고
						{                             //공백문자를 추가하여 삭제한것처럼 구현 
							dir = 0;
							LCD_Pos(0, i);
							LCD_Char(' ');
							LCD_Pos(0, i);
							delay_ms(200);
							delete_flag = 0;
							i--;
						}
						else if (i == 5 || i == 6)
						{
							dir = 0;
							LCD_Pos(0, i + 1);
							LCD_Char(' ');
							LCD_Pos(0, i + 1);
							delay_ms(200);
							delete_flag = 0;
							i--;
						}
						else if (i == 7 || i == 8)
						{
							dir = 0;
							LCD_Pos(0, i + 2);
							LCD_Char(' ');
							LCD_Pos(0, i + 2);
							delay_ms(200);
							delete_flag = 0;
							i--;
						}
						else if (i == 9 || i == 10)
						{
							dir = 0;
							LCD_Pos(1, i - 5);
							LCD_Char(' ');
							LCD_Pos(1, i - 5);
							delay_ms(200);
							delete_flag = 0;
							i--;
						}
						else if (i == 11 || i == 12)
						{
							dir = 0;
							LCD_Pos(1, i - 4);
							LCD_Char(' ');
							LCD_Pos(1, i - 4);
							delay_ms(200);
							delete_flag = 0;
							i--;
						}
						else if (i == 13 || i == 14)
						{
							dir = 0;
							LCD_Pos(1, i - 3);
							LCD_Char(' ');
							LCD_Pos(1, i - 3);
							delay_ms(200);
							delete_flag = 0;
							i--;
						}
						else delete_flag = 0;
					}
				}
			}
		}
	}
}

void alram_set(void)                        //10자리 입력x3 시 바로탈출 -> main
{
	for (j = 0; j < 3; j++)                        //초기화 
	{
		for (k = 0; k < 10; k++)
		{
			alram[j][k] = 0;
		}
	}
	i = j = 0;

	LCD_Pos(0, 6);
	while (1)
	{
		if (j == 0)                           //초기에 년도만 표시하기 위해서 설정 
		{
			LCD_Pos(0, 1);
			LCD_Char((year / 1000) + '0');
			LCD_Char(((year % 1000) - (year % 100)) / 100 + '0');
			LCD_Char(((year % 100) - (year % 10)) / 10 + '0');
			LCD_Char((year % 10) + '0');

			LCD_Pos(0, 5);
			LCD_Char('-');

			LCD_Pos(0, 8);
			LCD_Char('-');
			LCD_Pos(1, 6);
			LCD_Char(':');
			LCD_Pos(1, 9);
			LCD_Char(':');
			LCD_Pos(0, 6);
		}

		alram_key = KeyScan();                //키 받고 
		if (alram_key == 0)                    //채터링 방지 코드 
		{
			flag_chattering = 0;
		}
		else
		{
			if (!flag_chattering)              //채터링 방지 코드 
			{
				flag_chattering = 1;

				if (alram_key)
				{
					if (alram_key == 16)       //탈출 
					{
						break;
					}
					if (alram_key == 14)
					{
						alram_keynum = 0;
					}
					else if (alram_key % 4 != 0)
					{
						alram_keynum = (alram_key / 4) * 3 + (alram_key % 4);
					}
					else
					{
						alram_keynum = (alram_key / 4) + 9;
					}

					alram[i][j] = (alram_keynum + 0x30);   //키 받고 화면에 출력 
					LCD_Char(alram[i][j]);
					delay_ms(300);

					if (j != 10)
					{
						j++;
					}

					if (j == 10)                         //10자리 다 입력시 화면출력 후 메인 
					{
						i++;
						j = 0;
						LCD_Clear();
						delay_ms(10);
						LCD_Str(str5);
						delay_ms(1000);
						LCD_Clear();
						delay_ms(10);
						LCD_Pos(0, 6);
						delay_ms(50);
					}
					if (j == 2)  LCD_Pos(0, 9);
					else if (j == 4)  LCD_Pos(1, 4);
					else if (j == 6)  LCD_Pos(1, 7);
					else if (j == 8)  LCD_Pos(1, 10);

					if (i == 3)  break;
				}
				alram_month1 = ((alram[0][0] - 0x30) * 10) + (alram[0][1] - 0x30);      //비교하기 위한 변수들에 배열값들 저장 
				alram_day1 = ((alram[0][2] - 0x30) * 10) + (alram[0][3] - 0x30);
				alram_hour1 = ((alram[0][4] - 0x30) * 10) + (alram[0][5] - 0x30);
				alram_min1 = ((alram[0][6] - 0x30) * 10) + (alram[0][7] - 0x30);
				alram_sec1 = ((alram[0][8] - 0x30) * 10) + (alram[0][8] - 0x30);

				alram_month2 = ((alram[1][0] - 0x30) * 10) + (alram[1][1] - 0x30);
				alram_day2 = ((alram[1][2] - 0x30) * 10) + (alram[1][3] - 0x30);
				alram_hour2 = ((alram[1][4] - 0x30) * 10) + (alram[1][5] - 0x30);
				alram_min2 = ((alram[1][6] - 0x30) * 10) + (alram[1][7] - 0x30);
				alram_sec2 = ((alram[1][8] - 0x30) * 10) + (alram[1][8] - 0x30);

				alram_month3 = ((alram[2][0] - 0x30) * 10) + (alram[2][1] - 0x30);
				alram_day3 = ((alram[2][2] - 0x30) * 10) + (alram[2][3] - 0x30);
				alram_hour3 = ((alram[2][4] - 0x30) * 10) + (alram[2][5] - 0x30);
				alram_min3 = ((alram[2][6] - 0x30) * 10) + (alram[2][7] - 0x30);
				alram_sec3 = ((alram[2][8] - 0x30) * 10) + (alram[2][8] - 0x30);
			}
		}
	}

}



void main(void)
{
	init_timer();                      //초기 타이머 설정 
	LCD_Init();                        //lcd사용 설정 
	delay_ms(50);
	init_reg();                        //각종 포트 설정 

	mode = sec = min = 0;              //초기값 설정 
	month = 12;
	day = 5;
	hour = 12;
	year = 2019;
	dir = 0;

	LCD_Pos(0, 1);                      //초기화면 출력 
	LCD_Str(first);
	LCD_Pos(1, 1);
	LCD_Str(second);
	delay_ms(50);

	while (1)
	{

		key_Num = KeyScan();       //키를 받고 모드에 따라 동작 
		mode = key_Num;

		if (mode == 4) //m1 -> time_setting 
		{
			timer_flag = 1;
			LCD_Clear();
			delay_ms(10);
			LCD_Pos(0, 1);
			LCD_Str(str);
			delay_ms(2000);
			LCD_Clear();
			delay_ms(10);
			LCD_Str(str2);
			delay_ms(2000);
			LCD_Clear();
			delay_ms(10);
			mode = 0;
			time_set();
		}
		if (mode == 8) //m2 -> alram set mode
		{
			LCD_Clear();
			delay_ms(10);
			LCD_Str(str3);
			delay_ms(2000);
			LCD_Clear();
			LCD_Str(str4);
			delay_ms(2000);
			LCD_Clear();
			delay_ms(10);
			mode = 0;
			alram_set();
		}

		else if (mode == 0)
		{
			flag_chattering = 0;
		}
		else
		{
			if (!flag_chattering)
			{
				flag_chattering = 1;
				if (mode == 12)        //m3 -> am/pm변경 (m3버튼 중복눌림을 방지하는 채터링 코드 추가) 
				{
					dir = ~dir;
				}
			}
		}


		LCD_Pos(0, 5);
		LCD_Char('-');
		delay_ms(50);

		LCD_Pos(0, 8);
		LCD_Char('-');
		delay_ms(50);

		LCD_Pos(1, 6);
		LCD_Char(':');
		delay_ms(50);

		LCD_Pos(1, 9);
		LCD_Char(':');
		delay_ms(50);
		if (dir == 1)                          //m3가 눌렸을 시 am/pm변환 코드 
		{
			if (hour <= 12)                    //am -> pm 일때 am 을 지우고 12시간을 더한다 
			{                                 //ex) am 11:00 -> "  "23:00
				LCD_Pos(1, 1);
				LCD_Str("  ");
				LCD_Pos(1, 4);
				LCD_Char(((hour + 12) / 10) + '0');
				LCD_Char(((hour + 12) % 10) + '0');
			}
			else if (hour > 12)
			{
				LCD_Pos(1, 1);
				LCD_Str(AM);
				LCD_Pos(1, 4);
				LCD_Char(((hour - 12) / 10) + '0');
				LCD_Char(((hour - 12) % 10) + '0');
			}
		}

		else if (dir == 0)                     //초기상태에서 am/pm조절하기 위한 코드 
		{                                     //hour 에 23이 들어오면 -> pm 11시로 바꾸기 위함 
			if (hour > 12)
			{
				LCD_Pos(1, 1);
				LCD_Str(PM);
				LCD_Pos(1, 4);
				LCD_Char(((hour - 12) / 10) + '0');
				LCD_Char(((hour - 12) % 10) + '0');
			}
			else
			{
				LCD_Pos(1, 1);
				LCD_Str(AM);
				LCD_Pos(1, 4);
				LCD_Char((hour / 10) + '0');
				LCD_Char((hour % 10) + '0');
			}
		}
		else;

		LCD_Pos(0, 1);
		LCD_Char((year / 1000) + '0');                    //출력 함수 
		LCD_Char(((year % 1000) - (year % 100)) / 100 + '0');
		LCD_Char(((year % 100) - (year % 10)) / 10 + '0');
		LCD_Char((year % 10) + '0');

		LCD_Pos(0, 6);
		LCD_Char(month / 10 + '0');
		LCD_Char(month % 10 + '0');

		LCD_Pos(0, 9);
		LCD_Char(day / 10 + '0');
		LCD_Char(day % 10 + '0');

		LCD_Pos(1, 7);
		LCD_Char((min / 10) + '0');
		LCD_Char((min % 10) + '0');
		LCD_Pos(1, 10);
		LCD_Char((sec / 10) + '0');
		LCD_Char((sec % 10) + '0');
		delay_ms(50);

		if ((alram_month1 == month) && (alram_day1 == day) && (alram_hour1 == hour) && (alram_min1 == min))// && (alram_sec1 == sec))
		{
			alram_month1 = alram_day1 = alram_hour1 = alram_min1 = 0;    //분단위까지 비교해서 맞을 시 부저 + 화면출력 
			LCD_Clear();
			buzzer_flag = 1;
			while (buzzer_flag == 1)
			{
				LCD_Pos(0, 0);
				LCD_Str("!!!!Stand Up!!!!");
				LCD_Pos(1, 0);
				LCD_Str("press  PIND.0!!");
				delay_ms(10);

				SSound(Do);
				SSound(Mi);
			}
		}

		if ((alram_month2 == month) && (alram_day2 == day) && (alram_hour2 == hour) && (alram_min2 == min))
		{
			alram_month2 = alram_day2 = alram_hour2 = alram_min2 = 0;     //분단위까지 비교해서 맞을 시 부저 + 화면출력 
			buzzer_flag = 1;
			while (buzzer_flag == 1)
			{
				LCD_Pos(0, 0);
				LCD_Str("!!!!Stand Up!!!!");
				LCD_Pos(1, 0);
				LCD_Str("press  PIND.0!!");
				delay_ms(10);

				SSound(Do);
				SSound(Mi);
				SSound(Ra);
			}
		}
		if ((alram_month3 == month) && (alram_day3 == day) && (alram_hour3 == hour) && (alram_min3 == min))
		{
			alram_month3 = alram_day3 = alram_hour3 = alram_min3 = 0;      //분단위까지 비교해서 맞을 시 부저 + 화면출력 
			buzzer_flag = 1;
			while (buzzer_flag == 1)
			{
				LCD_Pos(0, 0);
				LCD_Str("!!!!Stand Up!!!!");
				LCD_Pos(1, 0);
				LCD_Str("press  PIND.0!!");
				delay_ms(10);

				SSound(Do);
				SSound(Mi);
				SSound(Ra);
				SSound(Si);
			}
		}
	}
}
