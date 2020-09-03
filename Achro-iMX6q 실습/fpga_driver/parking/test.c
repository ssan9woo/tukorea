#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#define LED_DEVICE "/dev/fpga_led"
#define FND_DEVICE "/dev/fpga_fnd"
#define FND_MAX_DIGIT 4

int main(int argc, char**argv){
	int dev_led,dev_fnd;
	unsigned char data_fnd[4];
	ssize_t ret_fnd,ret_led;
	int data_len;
	int number,i;
	char c;
	number=atoi(argv[1]);
	data_len=strlen(argv[1]);
	
	memset(data_fnd,0,sizeof(data_fnd));
	for(i=0; i<data_len;i++){
		c=argv[1][i];
		data_fnd[i]=c-'0';
	}
	dev_fnd=open(FND_DEVICE,O_RDWR);
	ret_fnd=write(dev_fnd,data_fnd,FND_MAX_DIGIT);
	
	dev_led=open(LED_DEVICE,O_RDWR);
	ret_led=write(dev_led,&number,1);
	sleep(1);
	
	close(dev_led);
	close(dev_fnd);
}
