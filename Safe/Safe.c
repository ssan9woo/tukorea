#include <mega128.h>
#include <delay.h>
#include "C:\Users\상우\Desktop\vhd\header\lcd2.h"

#define FND_Null        17
#define FND_Star        14
#define FND_Sharp       15

#define     Do      1908
#define     Re      1700
#define     Mi       1515
#define     Pa       1432
#define     Sol      1275
#define     La      1136
#define     Si      1012

unsigned char mode, i, key_set, key_set_num, key_input, key_input_num, wrong_count, key_admin, admin_num, key_lock, key_lock_num, count;
unsigned char password_set[4] = { 0,0,0,0 };    //초기 세팅 배열 설정 
unsigned char password_in[4];    //비밀번호 입력 배열
unsigned char admin[10];    //관리자 비밀번호 설정 배열
unsigned char lock[10];     //관리자 비밀번호 입력 배열 
bit open_flag = 0;          //초기화면에서 open/close 를 표현하기 위한 flag
bit setting_flag = 0;       //  { 세팅을 하고 비밀번호를 입력하는 과정이 이루어져야 금고 시스템이 동작하게 하기위해
bit input_flag = 0;         //    flag 설정. 둘다 1일때만 맞는지/틀린지 확인가능 } 
bit lock_flag = 0;          //영구잠금 시 관리자 코드 입력하는 곳에서 빠져나오지 못하게 하기위한 flag
bit delete_flag = 0;        //한글자 씩 삭제하기 위한 flag
bit dir;                    //4/10자리를 초과하면 더이상 입력 받지 못하게 하기위한 flag
bit escape_flag;            //비밀번호를 다 설정/입력 한 후 따로 설정한 버튼을 통해 main으로 가기위한 flag 
bit flag_chattering;        //채터링을 방지하기 위해 플래그 설정 


void init_reg(void)
{
	DDRB = 0xff;
	DDRC = 0x0f;            // 상위 4bit Col(입력), 하위 4bit Row(출력)
	PORTC = 0x0f;           // 하위4비트로 검출 
	DDRG |= (1 << 4);         // 부저 사용가능        
	SREG |= 0x80;
}

void myDelay_us(unsigned int delay)     //부저를 쓰기위한 딜레이함수 
{
	int i;
	for (i = 0; i < delay; i++)
	{
		delay_us(1);
	}
}

void SSound(int time, int lng)          //부저 출력함수 
{
	int i;
	for (i = 0; i < lng; i++) // 음계마다 같은 시간 동안 울리도록 time 변수 사용
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
void password_setting(void)                //비밀번호 설정 함수 
{
	LCD_Clear();                           //세팅 함수에 들어오면 초기에 LCD에 출력하는 화면설정 
	delay_ms(30);
	LCD_Pos(0, 0);
	LCD_Str("Password setting");
	delay_ms(1500);
	LCD_Pos(0, 0);
	LCD_Str("Initial Password");
	LCD_Pos(1, 7);
	LCD_Char(':');
	i = 0;

	while (1)
	{
		key_set = KeyScan();                //키를 받고 

		if (key_set == 0)                 //채터링 방지 코드 
		{
			flag_chattering = 0;
		}

		else
		{
			if (!flag_chattering)      //채터링 방지 코드 
			{
				flag_chattering = 1;

				if (key_set)                        //키를 누르면 
				{
					SSound(Si, 20);                 //키 누를 시 부저 울리게 설정 
					if (key_set == 12)              //m3버튼을 누르면 초기상태로 돌아간다[main] 
					{
						LCD_Clear();
						delay_ms(30);
						break;
					}
					else if (key_set == 15)         //#을 누르면 한글자씩 삭제된다 
					{
						delete_flag = 1;          //삭제하기 위해 플래그 설정 
					}
					else if (key_set == 4)         //4자리 입력후 main 복귀하는 버튼     
					{
						escape_flag = 1;
					}


					if (key_set == 14)
					{
						key_set_num = 0;
					}
					else if (key_set % 4 != 0)       // 3x4 키패드 사용을 위한 구문 
					{
						key_set_num = (key_set / 4) * 3 + (key_set % 4);
					}
					else
					{
						key_set_num = (key_set / 4) + 9;
					}

					if (delete_flag == 0)
					{
						if (dir == 0)     //4자리 넘게 입력하지못하게 하는 flag
						{
							password_set[i] = (key_set_num + 0x30);    //passwore_set이라는 배열에 내가 입력한 숫자들을 넣고 LCD로 출력 
							LCD_Char(password_set[i]);
							delay_ms(300);
							i++;
						}
						else;

						if (escape_flag == 1)      //4자리 설정 완료 후 탈출코드 
						{
							LCD_Clear();
							delay_ms(30);
							LCD_Pos(0, 0);
							LCD_Str("Setting Complite");           //비밀번호 4자리를 다 설정시에 LCD에 출력되는 화면
							delay_ms(1000);

							for (i = 0; i < 3; i++)
							{
								SSound(Si, 30);                     //설정 다 하면 부저 울리게 설정 
								SSound(Sol, 30);
							}
							LCD_Clear();
							delay_ms(30);

							setting_flag = 1;                      //다른 설정으로 들어가기위한 flag설정 
							open_flag = 0;
							wrong_count = 3;
							escape_flag = 0;
							dir = 0;
							break;
						}


						if (i == 4)                                //4자리 입력 시 
						{
							dir = 1;
						}
					}
					else if (delete_flag == 1)                      //#을 눌렀을 때 flag가 1이된다 -> 삭제하기 위한 코드 
					{
						if (i >= 1 && i <= 4)                       //4자리를 입력할 시 바로 메인문으로 빠지기 때문에 3자리까지 입력을 받았을때 삭제할 수 있다. 
						{                                          //3자리 삭제를 위한 코드 
							dir = 0;
							LCD_Pos(1, i + 7);
							LCD_Char(' ');
							LCD_Pos(1, i + 7);
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
void password_input(void)                            //비밀번호 입력 함수 
{
	LCD_Clear();
	delay_ms(30);
	LCD_Pos(0, 0);
	LCD_Str("Press Password");                      //비밀번호 입력 함수로 들어올 시 LCD에 출력되는 화면 
	LCD_Pos(1, 6);
	LCD_Char(':');
	i = 0;

	LCD_Pos(1, 7);
	while (1)
	{
		key_input = KeyScan();           //키를 받고 

		if (key_input == 0)                 //채터링 방지 코드 
		{
			flag_chattering = 0;
		}
		else
		{
			if (!flag_chattering)         //채터링 방지 코드 
			{
				flag_chattering = 1;

				if (key_input)
				{
					SSound(Si, 20);       //키 누르면 부저 울리게 

					if (key_input == 12)
					{
						LCD_Clear();
						delay_ms(30);
						break;
					}
					else if (key_input == 15)               //#키를 누르면 삭제 시키기 위해 flag 설정 
					{
						delete_flag = 1;
					}
					else if (key_input == 8)
					{
						escape_flag = 1;
					}


					if (key_input == 14)
					{
						key_input_num = 0;
					}
					else if (key_input % 4 != 0)
					{
						key_input_num = (key_input / 4) * 3 + (key_input % 4);
					}
					else
					{
						key_input_num = (key_input / 4) + 9;
					}

					if (delete_flag == 0)
					{
						if (dir == 0)
						{
							password_in[i] = (key_input_num + 0x30);           //입력한 숫자를 배열에 넣고 출력한다
							LCD_Char('*');                                     //비밀번호는 보이면 안되니까 *로 출력한다
							delay_ms(300);
							i++;
						}
						else;

						if (escape_flag == 1)                               //4자리 다 입력시에 main문으로 빠져나온다 
						{
							LCD_Clear();
							delay_ms(30);

							input_flag = 1;
							escape_flag = 0;
							dir = 0;
							break;
						}
						if (i == 4) dir = 1;                                //4자리 이상 입력 x
					}
					else if (delete_flag == 1)                              //4자리를 입력할 시 바로 메인문으로 빠지기 때문에 3자리까지 입력을 받았을때 삭제할 수 있다. 
					{                                                      //3자리 삭제를 위한 코드
						if (i >= 1 && i <= 4)
						{
							LCD_Pos(1, i + 6);
							LCD_Char(' ');
							LCD_Pos(1, i + 6);
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
void admin_code(void)                           //관리자 코드 설정 함수 
{
	LCD_Clear();
	delay_ms(30);
	LCD_Pos(0, 0);
	LCD_Str("Admin Setting..!");                //함수에 들어오면 초기화면에 출력되는 LCD 설정  
	LCD_Pos(1, 0);
	LCD_Char(':');
	delay_ms(300);
	i = 0;

	while (1)
	{
		key_admin = KeyScan();

		if (key_admin == 0)                 //채터링 방지 코드 
		{
			flag_chattering = 0;
		}
		else
		{
			if (!flag_chattering)      //채터링 방지 코드 
			{
				flag_chattering = 1;

				if (key_admin)
				{
					SSound(Si, 20);         //키 누르면 부저 울리게 
					if (key_admin == 12)
					{
						LCD_Clear();
						delay_ms(30);
						break;
					}
					else if (key_admin == 16)
					{
						escape_flag = 1;
					}
					else if (key_admin == 15)           //#키를 누를 시 삭제하기위해 flag설정 
					{
						delete_flag = 1;
					}
					else if (key_admin == 16)          //설정 완료 후 M4누르면 탈출 
					{
						escape_flag = 1;
					}


					if (key_admin == 14)
					{
						admin_num = 0;
					}
					else if (key_admin % 4 != 0)
					{
						admin_num = (key_admin / 4) * 3 + (key_admin % 4);
					}
					else
					{
						admin_num = (key_admin / 4) + 9;
					}

					if (delete_flag == 0)
					{
						if (dir == 0)
						{
							admin[i] = (admin_num + 0x30);               //입력한 숫자를 배열에 넣고 출력하는 과정 
							LCD_Char(admin[i]);
							delay_ms(300);
							i++;
						}
						else;

						if (escape_flag == 1)                             //10자리 전부 입력시 main문으로 
						{
							LCD_Clear();
							delay_ms(30);
							LCD_Pos(0, 0);
							LCD_Str("Setting Complite");           //비밀번호 4자리를 다 설정시에 LCD에 출력되는 화면
							delay_ms(1000);

							for (i = 0; i < 3; i++)                       //설정 완료 시 부저 울리게 
							{
								SSound(Si, 30);
								SSound(Sol, 30);
							}
							LCD_Clear();
							delay_ms(30);
							escape_flag = 0;
							dir = 0;
							break;
						}                                                //10자리 이상 입력 X
						if (i == 10)
						{
							dir = 1;
						}
					}
					else if (delete_flag == 1)                      //#눌렀을 시 삭제하기 위한 코드 (10자리) 
					{
						if (i >= 1)                                 //pos값을 계속 바꿔주기 위해 위에 사용되는 변수 i를 썼다 
						{
							dir = 0;
							LCD_Pos(1, i);
							LCD_Char(' ');
							LCD_Pos(1, i);
							delay_ms(200);
							delete_flag = 0;
							i--;
						}
						else  delete_flag = 0;   //삭제해서 처음으로 돌아갈 시 다시 입력을 받기위해       
					}
				}
			}

		}
	}
}
void lock_release(void)                               //관리자 코드 입력함수 
{
	LCD_Clear();
	delay_ms(30);
	LCD_Pos(0, 0);
	LCD_Str("Press Code!!!");                         //코드 입력 함수에 들어왔을 시 lcd출력 설정 
	LCD_Pos(1, 0);
	LCD_Char(':');
	i = 0;

	while (1)
	{
		key_lock = KeyScan();

		if (key_lock == 0)                 //채터링 방지 코드 
		{
			flag_chattering = 0;
		}
		else
		{
			if (!flag_chattering)      //채터링 방지 코드 
			{
				flag_chattering = 1;

				if (key_lock)
				{
					SSound(Si, 20);                     //키 눌렀을 때 부저 울리게 설정 
					if (key_lock == 15)                 //#을 눌렀을 시 글자 삭제 
					{
						delete_flag = 1;
					}
					else if (key_lock == 16)            //10자리 입력 완료 후 M4누를 시 탈출 
					{
						escape_flag = 1;
					}

					if (key_lock == 14)
					{
						key_lock_num = 0;
					}
					else if (key_lock % 4 != 0)
					{
						key_lock_num = (key_lock / 4) * 3 + (key_lock % 4);
					}
					else
					{
						key_lock_num = (key_lock / 4) + 9;
					}

					if (delete_flag == 0)                   //10자리 입력하기 위해 배열에 키 값들을 넣고 출력 
					{
						if (dir == 0)                       //10자리 넘으면 입력 안받기 위한 FLAG
						{
							lock[i] = (key_lock_num + 0x30);
							LCD_Char(lock[i]);
							delay_ms(300);
							i++;
						}
						else;

						if (escape_flag == 1)               //탈출버튼 누르면 
						{
							LCD_Clear();
							delay_ms(30);
							escape_flag = 0;
							dir = 0;
							break;
						}                                 //10자리 이상 입력 X
						if (i == 10)
						{
							dir = 1;
						}
					}
					else if (delete_flag == 1)             //한글자 씩(10글자) 삭제하기 위한 코드 
					{
						if (i >= 1)
						{
							dir = 0;
							LCD_Pos(1, i);
							LCD_Char(' ');
							LCD_Pos(1, i);
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

void main()
{
	LCD_Init();                              //LCD 초기화
	delay_ms(30);
	init_reg();                             //각종 포트들 설정 
	setting_flag = input_flag = count = 0;  //변수들, flag 초기화 
	wrong_count = 3;                        //횟수제한 변수 


	while (1)
	{
		if (lock_flag == 0)                    //영구잠금 했을때 초기화면으로 돌아오지 못하게 flag설정 
		{
			LCD_Pos(0, 0);
			LCD_Str("   Kpu Safe...  ");      //초기화면 
			delay_ms(30);
			LCD_Pos(0, 14);
			LCD_Pos(1, 13);
			LCD_Char('[');
			LCD_Char(wrong_count + 0x30);     //틀린횟수 lcd에 표기하기위한 구문 
			LCD_Char(']');
			delay_ms(50);


			if (open_flag == 1)                   //화면에 현재상태 (열림,닫힘)을 표시하기 위해서 flag설정 
			{
				LCD_Pos(1, 6);
				LCD_Str("[Close]");
				delay_ms(30);
			}
			else if (open_flag == 0)
			{
				LCD_Pos(1, 7);
				LCD_Str("[Open]");
				delay_ms(30);
			}
		}
		mode = KeyScan();

		if (mode == 4)                     //m1 : 비밀번호 설정
		{
			password_setting();
		}
		else if (mode == 16)               //m4 : 관리자 코드 설정 
		{
			admin_code();

			delay_ms(30);
			LCD_Pos(0, 0);
			LCD_Str("Setting Complite");
			delay_ms(1500);
			LCD_Clear();
			delay_ms(30);
		}
		else if (mode == 8)                //m2 : 비밀번호 입력 
		{
			password_input();
		}

		//상위조건 비밀번호 설정하고 나서 플래그 변화 
		if ((setting_flag == 1) && (input_flag == 1))     //최초에 비밀번호를 [설정 / 입력] 해야만 비밀번호를 비교할수있게 flag설정  
		{
			if ((password_set[0] == password_in[0]) && (password_set[1] == password_in[1]) && (password_set[2] == password_in[2]) && (password_set[3] == password_in[3]))
			{
				input_flag = 0;                        //비밀번호 설정값과 비밀번호 입력값이 같으면 실행 
				open_flag = 0;
				wrong_count = 3;

				LCD_Clear();
				delay_ms(50);
				LCD_Pos(0, 3);
				LCD_Str("Safe Open!");

				SSound(Do, 100);                      //부저 on
				SSound(Mi, 100);
				SSound(Sol, 100);
				delay_ms(2000);
			}
			else
			{
				input_flag = 0;                     //비밀번호가 틀릴시 
				open_flag = 1;
				wrong_count--;                      //비밀번호 횟수를 표시하는 wrong count를 줄인다

				LCD_Clear();
				delay_ms(50);
				LCD_Pos(0, 0);
				LCD_Str("Wrong Password!!");

				for (i = 0; i < 3; i++)
				{                                  //부저 on
					SSound(Do, 70);
					SSound(Si, 70);
				}
				delay_ms(2000);
			}
		}
		if (wrong_count == 0)                      //3번이 다 틀릴시에 
		{
			LCD_Pos(0, 0);
			LCD_Str("  Safe Lock!!!");
			delay_ms(2000);

			lock_release();    //10자리 받고        //관리자코드를 입력할수있는 함수를 불러온다 

			for (i = 0; i < 10; i++)
			{
				if (admin[i] == lock[i])          //10자리를 비교해서 count값을 증가시킨다 
				{
					count++;
				}
			}

			if (count == 10)    //10자리 다 맞으면
			{
				lock_flag = 0;                  //safe open 이후 main으로 돌아가기 위한 flag설정 
				wrong_count = 3;                //초기의 open상태와 wrong count를 3으로만들어 초기와 같게 만든다
				open_flag = 0;                  //비밀번호 맞을 시 main문에서 open을 표시 하기위한 flag설정 

				count = 0;
				LCD_Clear();
				delay_ms(30);
				LCD_Pos(0, 3);
				LCD_Str("Safe Open!");

				SSound(Do, 100);
				SSound(Mi, 100);
				SSound(Sol, 100);
				delay_ms(2000);

			}
			else if (count != 10)   //한자리라도 틀릴경우 다시 관리자코드를 칠수있는 함수로 돌아가기위해서 flag를 설정해줬다 
			{
				wrong_count = 0;   //횟수를 0으로 만들어서 다시 10자리 입력받게 함 
				count = 0;         //10자리 비교 카운트 다시설정 
				lock_flag = 1;     //lock count 를 1로 설정할 시에 다시 429라인[관리자 비밀번호 입력함수] 로 들어온다 

				LCD_Clear();
				delay_ms(50);
				LCD_Pos(0, 0);
				LCD_Str("Wrong Admin-Code");

				for (i = 0; i < 3; i++)
				{
					SSound(Do, 70);
					SSound(Si, 70);
				}
				delay_ms(2000);
				LCD_Clear();
			}
		}
	}
}