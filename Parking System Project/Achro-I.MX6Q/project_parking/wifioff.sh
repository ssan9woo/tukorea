ifconfig wlan0 down
ifconfig eth0 up
ifconfig eth0 192.168.1.222
mount -t nfs 192.168.1.70:/root/work /root/nfs -o tcp,nolock
