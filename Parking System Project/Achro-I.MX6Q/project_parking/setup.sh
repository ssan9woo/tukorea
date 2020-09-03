umount /root/nfs

ifconfig eth0 down
iwconfig wlan0 essid "wookkl"

wpa_supplicant -i wlan0 -c /etc/wpa_supplicant.conf &

ifconfig wlan0 172.20.10.5 netmask 255.255.255.0
route add default gw 172.20.10.1


ifconfig wlan0 up 
