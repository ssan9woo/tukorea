#include<iostream>
#include<string>
#define SIZE 4
using namespace std;

class date {  //월 일 클래스
protected:
	int month;
	int day;
public:
	date() :month(0), day(0) {};
	date(int m, int d) :month(m), day(d) {};
	void setdate(int m, int d);
	void showdate();

};
void date::setdate(int m, int d) { //월 일 세팅
	month = m;
	day = d;
}

void date::showdate() {  //월 일 출력
	cout << month << "월" << day << "일,    ";
}



class firstmoney :public date {                // 지출금액
protected:
	int fmoney;
	int cmoney;
public:
	firstmoney() :cmoney(0) {};
	firstmoney(int m, int d, int c) :date(m, d), cmoney(c) {};
	void setmoney(int m, int d, int c);
	void show();
	void showfmoney();
	void inputfmoney();
	friend void output(firstmoney *first); //출력에서 멤버변수 접근
	int getday();
	int getmonth();
	friend void input(firstmoney *first);
};
int firstmoney::getmonth() { //검색시 월 리턴
	return month;
}

int firstmoney::getday() { //검색시 날짜 리턴

	return day;
}

void firstmoney::inputfmoney() {           //초기금액 설정
	cout.width(44); cout << "초기금액을 적어주세용 :[   ]원" << endl << endl;
	cin >> fmoney;
}

void firstmoney::showfmoney() {           //초기금액 출력
	cout << "초기금액" << fmoney << "원" << endl;
}

void firstmoney::setmoney(int m, int d, int c) {  //월 일 지출금액 설정
	date::setdate(m, d);
	cmoney = c;

}

void firstmoney::show() {                     // 월 일 지출금액 출력
	showdate();
	cout << "지출금액" << cmoney << "원 " << endl;

}

//입력
void input(firstmoney *first) {
	int month, day, cmoney;
	first->inputfmoney();
	cout.width(50); cout << "   [본 프로그램은 일단 입력을 4개로 구성하였습니다.]   " << endl << endl << endl;
	cout.width(56); cout << "---------------------------------------------------------" << endl;
	cout.width(55); cout << "1.가게부를 적어주시기바랍니다. [월] [일] [금액]순" << endl << endl;
	cout.width(52); cout << "2.시간을 역행순으로 적으면 오류가 날 수 있으니" << endl << endl;
	cout.width(54); cout << "반드시 날짜/월 을 순행적으로 적어주시기바랍니다." << endl << endl;
	cout.width(45); cout << "ex] 5  3  12000  =  5월 3일 12000원" << endl;
	for (int i = 0; i < SIZE; i++)
	{
		cout.width(42); cout << endl << "[ ]월 , [ ]일, 지출금액[    ]원 " << endl;

		cin >> month >> day >> cmoney;
		if (month > 12, day > 31)
		{
			cout << "잘못 입력하셨습니다." << endl << endl;
			cout << "초기화면으로 돌아갑니다." << endl << endl; break;


		}

		first[i].setmoney(month, day, cmoney);

	}


}
void Sort(firstmoney *first)// 월별로 정렬
{
	firstmoney fm;

	for (int i = 0; i < SIZE - 1; i++)
	{
		for (int j = 0; j < (SIZE - i - 1); j++)
		{
			if (first[j].getmonth() > first[j + 1].getmonth())
			{
				fm = first[j + 1];
				first[j + 1] = first[j];
				first[j] = fm;
			}
		}

	}
	for (int i = 0; i < SIZE; i++) {
		first[i].show();
	}
}
//출력
void output(firstmoney *first) {
	int sum, i;
	first->showfmoney();
	for (i = 0, sum = 0; i < SIZE; i++)
	{
		sum += first[i].cmoney;
		cout << "[" << i + 1 << "]" << endl;
		first[i].show();
		cout << endl << "잔액 :" << first->fmoney - sum << endl << endl;

	}

}

void searchday(firstmoney *first)//검색[날짜]
{
	int day;


	cout << "검색할 날짜 : [ ]일 ";
	cin >> day;

	for (int i = 0; i < SIZE; i++)
	{
		if (first[i].getday() == day)
		{
			first[i].show();

		}
	}
}
void searchmonth(firstmoney *first)//검색[월]
{
	int month;

	
	cout << "검색할 월 : [ ]월 ";
	cin >> month;

	for (int i = 0; i < SIZE; i++)
	{
		if (first[i].getmonth() == month)
		{
			first[i].show();

		}
		else
			cout << "     " << endl;
	}
	
}
void search(firstmoney *first)//검색
{
	int a;
	cout.width(54); cout << "□□□□□□□□□□□□□□□□□□□□□□□□□" << endl;
	cout.width(39); cout << "지출내용 검색기능입니다." << endl << endl;
	cout.width(52); cout << "본 검색기능은 날짜와 지출내역만 표시됩니다." << endl << endl;
	cout.width(42); cout << "잔액은 출력파트에서 확인바랍니다." << endl << endl;
	cout.width(33); cout << "1. 달별로 검색하기" << endl << endl;
	cout.width(33); cout << "2. 일자로 검색하기" << endl << endl;
	cout.width(31); cout << "3. 로비로 나가기" << endl << endl;
	cout.width(54); cout << "□□□□□□□□□□□□□□□□□□□□□□□□□" << endl;
	cin >> a;
	switch (a)
	{
	case 1:searchmonth(first); break;
	case 2: searchday(first); break;
	case 3: break;
	default:
		cout << endl << "잘못 입력하셨습니다." << endl; break;
	}

}

//메인
void mainmenu() {

	cout.width(50); cout << "★★★★★★가게부 프로그램★★★★★★" << endl << endl;
	cout.width(37); cout << " 1. 가게부 쓰기 " << endl << endl;
	cout.width(37); cout << " 2. 가게부 보기 " << endl << endl;
	cout.width(38); cout << " 3. 지출내역 검색 " << endl << endl;
	cout.width(37); cout << " 4. 날짜별 정렬 " << endl << endl;
	cout.width(34); cout << " 0. 종료하기" << endl << endl;
	cout.width(50); cout << "★★★★★★★★★★★★★★★★★★★★" << endl << endl;

}


void main() {
	firstmoney *first = new firstmoney[SIZE]; //배열로 잡아주기위해 new
	int num;
	do {
		mainmenu();
		cin >> num;
		switch (num)
		{
		case 1: input(first); break;
		case 2: output(first); break;
		case 3: search(first); break;
		case 4: Sort(first); break;
		case 0: break;
		default:
			cout << "잘못 입력하셨습니다." << endl;
			continue; //잘못입력하면 처음으로 돌아감.
		}
	} while (num != 0);
	cout << "종료합니다" << endl;
}