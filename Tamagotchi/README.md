# Sensor

Esp 8266과 핸드폰을 이용해 나만의 다마고치를 키울 수 있는 프로젝트

몇가지의 센서 / 소자들과 Cloudmqtt 서버를 통해 제작.

필요한 센서 및 소자 
-----------------------------------

1. 조도센서
2. 온습도센서(DHT)
3. 키패드(4 x 4)
4. Timer(내장)
5. [128 x 32] OLED : Bit map이 128 x 32로 되어있기 때문에 64픽셀을 쓰면 오류발생.
6. cloudmqtt (무료 broker)

각 기능설명 
-------------------------------------

1. 다마고치의 기분, 배고픔, 나이, 상태 등을 볼 수 있다.

2. 조도센서, 산책(옵션), 온습도센서의 상태 등으로 기분을 제어.

3. 기본적인 실행 환경은 128 x 32의 oled로 출력한다.

4. 키패드 or 버튼을 사용해 메뉴선택, 밥주기, 산책 등의 인터페이스 사용.

5. MQTT서버를 이용해 위급상황[ex) 배고픔경보] 를 핸드폰/다른 esp8266보드로 받을 수 있다.
   - 배고픔이 2 이하이면 핸드폰/보드 로 위급상황을 알린다.
   
6. 다마고치의 나이에 따라 모습이 변경 될 수 있도록 한다(나이가 5살 이상일 시 변화)

7. Timer로 일정 시간마다 온/습도 값을 받아오고, 나이가 많아지고, 배고픔 이 떨어진다.

Screenshot
----------
<br><br>

Initial screen

<img width = "18%" src = "https://user-images.githubusercontent.com/56511253/71470400-6148c180-280f-11ea-9723-504464169b30.png">

<br>
Main
<img width = "18%" src = "https://user-images.githubusercontent.com/56511253/71470404-6279ee80-280f-11ea-92dd-0aca7910f8f9.png">

<br>
Menu
<img width = "18%" src = "https://user-images.githubusercontent.com/56511253/71470406-6443b200-280f-11ea-9278-1d4a64170e6a.png">

<br>
State
<img width = "18%" src = "https://user-images.githubusercontent.com/56511253/71470408-660d7580-280f-11ea-841c-2b4cc6030f07.png">

<br>
Walk
<img width = "18%" src = "https://user-images.githubusercontent.com/56511253/71470410-673ea280-280f-11ea-8429-c88ede02a6f5.png">

<br>
Feed
<div>
  <img width = "18%" src = "https://user-images.githubusercontent.com/56511253/71470414-6a399300-280f-11ea-9d8d-250eeffd64c7.png">
  &emsp;
  <img width = "18%" src = "https://user-images.githubusercontent.com/56511253/71470416-6b6ac000-280f-11ea-8f33-eb12b0d8fe5c.png">
  &emsp;
  <img width = "18%" src = "https://user-images.githubusercontent.com/56511253/71470420-6c9bed00-280f-11ea-841c-82be52c7056d.png">
</div>
<br>
Temp
<img width = "18%" src = "https://user-images.githubusercontent.com/56511253/71470423-6e65b080-280f-11ea-9b85-617856dfa162.png">

<br>
Lux
<img width = "18%" src = "https://user-images.githubusercontent.com/56511253/71470427-6f96dd80-280f-11ea-801c-b90ccb429f30.png">

<br>
Mqtt
<img width = "18%" src = "https://user-images.githubusercontent.com/56511253/71470429-70c80a80-280f-11ea-9d89-dfb46a198463.png">


