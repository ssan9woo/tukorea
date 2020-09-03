#include "../include/fpga_test.h"
unsigned char quit = 0;
void user_signal1(int sig){quit = 1;}

int main(int argc, char **argv)
{
	int dev;
	unsigned char dip_sw_buf = 0;

	dev = open(DIP_SWITCH_DEVICE, O_RDONLY);
	assert2(dev >= 0, "Device open error",DIP_SWITCH_DEVICE);

	(void)signal(SIGINT, user_signal1);
	printf("Press <ctrl + c> to quit.\n");

	while(!quit)
	{
		usleep(400000);
		read(dev,&dip_sw_buf,1);
		printf("Read dip switch: 0x%02X\n", dip_sw_buf);
	}
	close(dev);
	return 0;

}

