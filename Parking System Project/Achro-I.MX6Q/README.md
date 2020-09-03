## Network Setting
  1. mount
  2. ifconfig eth0 down, 
  3. ifconfig wlan0 up
  4. iwconfig wlan0 essid "wookkl"
  5. wpa_supplicant -i wlan0 -c /etc/wpa_supplicant.conf &
  6. iwconfig wlan0 172.20.10.5 netmask 255.255.255.0
  7. route add default gw 172.20.10.1
  - 외부 Network와 접속 시 default gw필요, 단순 소켓통신만 할 때는 필요없음.






## 오류 및 해결과정

- nfs오류 : Host pc에서 Achro로 Mount로 파일을 옮기고 LAN포트 뽑을 시 minicom 멈춤.
   - Ethernet을 down시키기 전에 umount를 해줘야 멈추지 않는다. 

- Gateway 설정오류 : Server와 Client가 같은 네트워크에 있음에도 ping으로 패킷이 보내지지 않음.
   - Server와 Client를 Routing Table을 통해 같은 게이트웨이에 할당 시켜준다.
   - 외부 Network와 연결할 시 Default Gateway 설정 필요.




