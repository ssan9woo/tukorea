#include "/root/work/achroimx6q/fpga_driver/include/fpga_test.h"

int main(int argc,char **argv){
	int dev;
	unsigned char data;
	ssize_t ret;
	
	char usage[50];
	sprintf(usage,"Usage:\n\tfpga_led_test <%d ~ %d>\n",LED_MIN,LED_MAX);
	assert(argc == 2,usage);
	data=atoi(argv[1]);
	
	dev=open(LED_DEVICE,O_RDWR);
	assert2(dev >= 0, "Device open error", LED_DEVICE);

	ret=write(dev, &data,1);
	assert2(ret >= 0, "Device read error", LED_DEVICE);
	
	sleep(1);
	
	ret=read(dev,&data,1);
	assert2(ret >= 0, "Device read error", LED_DEVICE);

	printf("Current LED value: %d\n", data);
	printf("\n");
	
	close(dev);
	return 0;
}
