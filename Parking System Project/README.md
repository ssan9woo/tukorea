# 장애인 주차 관리 시스템
## 개발 환경: Crosscompile, NFS
### HOST PC
* Ubuntu 12.04 LTS
* C++(OpenCV)
* C (Module Programming)
### Target Board
* 4x ARM Cortex-A9
* Wifi 802.11n Module
* Linux Kernel Version: 3.0.35   
### Raspberry Pi 3B+
* Python3(Opencv)
* Camera Module
## IP
- WIFI : wookkl
- Raspberry pi(Server) : 172.20.10.2
- Achro-I.MX6Q(Client_Target) : 172.20.10.5
- Android Studio(Client_Phone)
##Routing Table
- Default Gateway : 172.20.10.1
- Server와 Client_Target은 같은 Network에서 같은 Default Gateway를 설정하여 통신.
