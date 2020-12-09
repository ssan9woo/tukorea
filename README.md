# Embedded-Board

교과 과정으로 ATmega128, ESP8266, Achro-I.MX6Q의 임베디드 보드를 제어.

ATmega128 : 201806 ~ 201812
ESP8266 : 201906 ~ 202007
Achro-I.MX6Q : 202004 ~ 202007


# 프로젝트 설명

## 1. Toycom
- 2학년 1학기 컴퓨터 구조론 수업의 과제 Project(1인 1조)
- 어셈블리어의 학습을 Toycom Program사용.
- Branch와 XOR의 Processing을 구현.
<br><br>
<br><br>

## 2. Account book
- 2학년 1학기 객체지향언어 수업의 기말 Term Project(1인 1조)
- C++을 이용해 개인이 사용할 수 있는 간단한 가계부 프로그램 개발.
<br><br>
<br><br>

## 3. Alram Clock, Safe
- 3학년 1학기 임베디드 프로세서의 기말 Trem Project(1인 1조)
- 수업중에 배운 8Bit MCU ATmega128의 내부 기능들을 사용해 Project 구성.
- ATmega128은 내, 외부 인터럽트와 SPI, IC2, Serial, ADC, 타이머 등이 존재해 간단하게 학습과 프로젝트 구성으로 사용하기 적합.
- AVR studio를 이용한 C언어로 개발.

- 자세한 프로젝트 내용
    - [Alram Clock](https://github.com/sangwoo24/Main-Project-in-KPU/tree/master/Alram%20Clock) : ATmega128로 알람시계 구현
    - [Safe](https://github.com/sangwoo24/Main-Project-in-KPU/tree/master/Safe) : ATmega128로 금고 프로그램 구현
<br><br>
<br><br>

## 4. Login Program, Group Chatting Program, P2P File Transfer System

- 3학년 2학기 임베디드 운영체제의 기말 Term Project(2인 1조)
- Linux 환경에서 C언어를 사용하여 프로젝트 진행.
- Flag를 이용한 접근 제어 방식과 공유 메모리 사용 방식 학습.
- 원격 접속 터미널인 Putty사용.

    - 자세한 프로젝트 내용
    - [Linux Login Project](https://github.com/sangwoo24/Main-Project-in-KPU/tree/master/Linux_Login%20Program) : ID와 PW를 입력하여 일치할 시 로그인 메세지가 나오는 프로그램.
    - [Group Chatting Program](https://github.com/sangwoo24/Main-Project-in-KPU/tree/master/GroupChatting%20in%20Linux%2CC%2CSocket) : Server를 통해 Client들이 그룹 채팅을 할 수 있는 프로그램.
    - [P2P File Transfer System](https://github.com/sangwoo24/Main-Project-in-KPU/tree/master/P2P_File_Transfer%20in%20Linux%2CC%2CSocket) : 그룹 채팅 내에서 P2P로 File을 보내기 위한 [그룹채팅 + 파일전송]
    프로그램.
<br><br>
<br><br>

## 5. Bus Route
- 3학년 2학기 프로그래밍 응용의 기말 Term Project(4인 1조)
- PBL수업이기에 주어진 문제를 자유로운 언어로 구현 가능했음.
1. 학생들의 Data N개를 엑셀 형태로 받아옴.
2. N명의 학생이 모두 탈 수 있고, 학교를 포함한 노선 3개를 생성.
3. 임의의 학생 A를 입력할 시, A의 집으로부터 학교까지 최적의 [교통수단]과 [시간] 및 [거리]를 출력.
- 주어진 문제를 Python으로 구현.
- K 평균, 다익스트라 등의 알고리즘과 주소를 기반으로 실제 위도와 경도의 정보를 얻을 수 있는 Google Api인 Geocoder 사용.
    
    - 자세한 프로젝트 내용
    - [Bus Route](https://github.com/sangwoo24/Main-Project-in-KPU/tree/master/Bus%20route) : 최종 코드는 2번째 코드.txt 입니다.
<br><br>
<br><br>

## 6. Face Swapping
- 3학년 2학기 임베디드 비전 시스템의 기말 Term Project(3인 1조)
- Opencv 를 이용해 자율 프로젝트 진행.
- Python을 기반으로 진행, Opencv Face Landmark Detection과 Tkinter를 이용해 실시간 영상으로 비교 대상과 얼굴을 바꾸는 프로그램 개발.
    
    - 자세한 프로젝트 내용
    - [Face Swapping](https://github.com/sangwoo24/Main-Project-in-KPU/tree/master/Embedded%20Vision%20System%20-%20Face%20swapping) : 최종 코드는 realtime_face_swapping.py 입니다.
<br><br>
<br><br>

## 7. Tamagochi
- 3학년 2학기 센서응용 시스템의 기말 Term Project(1인 1조)
- ESP-8266을 기반으로 자율 프로젝트 진행.
- ESP와 MQTT를 이용해 자신의 애완용 펫을 키울 수 있게 제작.
- 간단하게 Server와 Client로 나누어 핸드폰, 타 보드 등에서 원격 제어가 가능.

    - 자세한 프로젝트 내용
    - [Tamagochi](https://github.com/sangwoo24/Main-Project-in-KPU/tree/master/Tamagotchi)
<br><br>
<br><br>

## 8. Smart Home[Domitory]
- 4학년 1학기 사물인터넷의 기말 Term Project(4인 1조)
- ESP-8266과 AWS 의 Node-RED를 이용해 자율 프로젝트 진행.
- ESP와 Raspberry pi, Arduino, Node-RED를 이용해 가상의 환경에서 집안의 센서들을 휴대폰으로 제어할 수 있게 하는 프로그램 개발.
    - 자세한 프로젝트 내용
    - [Smart Home[Domitory]](https://github.com/sangwoo24/Main-Project-in-KPU/tree/master/Smart%20Home(Domitory))
<br><br><br><br>
## 9. Parking System
- 4학년 1학기 고급 임베디드 시스템의 기말 Term Project(2인 1조)
- Achro.imx6q 보드를 이용해 Linux 환경에서 자율 프로젝트 진행.
1. Server인 Raspberry pi는 Achro.imx6q가 초음파 센서로 차량을 
인식할 시 Opencv Tesseract로 차량의 번호판을 추출하고, Client인 Android와 Achro.imx6q에게 차량 정보를 전송.

2. Android는 정보를 받은 뒤 DB에 저장을 하여 차량조회, 요금조회, 주차 정산의 기능을 수행.
3. Achro.imx6q는 차량을 초음파 센서로 감지한 뒤, 정보를 받고 차량정산, 요금조회, 주차정산의 기능을 수행.
4. Frmae Buffer를 이용해 Achro.imx6q의 터치스크린LCD에 가상의 주차장 시뮬레이터 생성.  

    - 자세한 프로젝트 내용
    - [Parking System](https://github.com/sangwoo24/Main-Project-in-KPU/tree/master/Parking%20System%20Project)

<br><br><br><br>

## 10. Achro-iMX6q 실습
- Achro imx6q에 내장된 8가지 모듈을 사용하기 위함.
<br><br><br><br>

## 11. IOT실습
- ESP-8266 제어에 필요한 코드들과 Test코드 존재.

## 12. TodoList(임베디드 응용SW)
