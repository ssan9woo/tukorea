insmod ./ko/fpga_text_lcd_driver.ko
mknod /dev/lcd c 263 0

insmod ./ko/fpga_fnd_driver.ko
mknod /dev/fnd c 261 0

insmod ./ko/fpga_hc-sr04_driver.ko
mknod /dev/us c 246 0

insmod ./ko/fpga_push_switch_driver.ko
mknod /dev/push_switch c 265 0

insmod ./ko/fpga_buzzer_driver.ko
mknod /dev/buzzer c 264 0 

insmod ./ko/fpga_dot_driver.ko
mknod /dev/dot c 262 0 

insmod ./ko/fpga_led_driver.ko
mknod /dev/led c 260 0 
