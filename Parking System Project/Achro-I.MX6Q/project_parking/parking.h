#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>   
#include <sys/ioctl.h>
#include <unistd.h>  
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/mman.h>

#define LED_DEVICE	"/dev/led"
#define LED__MIN		0
#define LED__MAX		255

#define FND_DEVICE	"/dev/fnd"
#define FND_MAX_DIGIT	4

#define DOT_DEVICE 	"/dev/dot"
#define DOT_MIN		0
#define DOT_MAX		9

#define TEXT_LCD_DEVICE 	"/dev/lcd"
#define TEXT_LCD_MAX_BUF 	32
#define TEXT_LCD_LINE_BUF	16

#define BUZZER_DEVICE		"/dev/buzzer"
#define BUZZER_ON		0
#define BUZZER_OFF		1
#define BUZZER_TOGGLE(x)	(1-(x))

#define PUSH_SWITCH_DEVICE		"/dev/push_switch"
#define IOM_PUSH_SWITCH_MAX_BUTTON	9

#define BUFF_SIZE 1024             
#define SERV_PORT 9999
#define CAR_NUMBER  5
#define TRUE 1
#define FALSE 0
#define DEV_SCREEN_PATH "/dev/input/by-path/platform-imx-i2c.2-event"
#define AREA_NUM 6
typedef struct strct_parking_user{
	char area;
	char car_number[CAR_NUMBER];
}Parking_user;





