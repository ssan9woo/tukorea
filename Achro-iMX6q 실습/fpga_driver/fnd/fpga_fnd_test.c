#include "/root/work/achroimx6q/fpga_driver/include/fpga_test.h"

int main(int argc,char **argv){
	int dev;
	unsigned char data[4];
	ssize_t ret;
	int data_len;
	int i;
	char c;
			
	char usage[50];
	sprintf(usage,"Usage:\n\tfpga_fnd_test <%d digits>\n",FND_MAX_DIGIT);
	assert(argc == 2,usage);
	
	data_len=strlen(argv[1]);	
	assert(data_len <= FND_MAX_DIGIT, "Too many digits");
	memset(data,0, sizeof(data));
	for(i=0; i<data_len;i++){
		c=argv[1][i];
		assert('0'<=c && c<='9',"Invalid digit value");
		data[i] = c-'0';
	}
	
	dev = open(FND_DEVICE,O_RDWR);
	assert2(dev>=0, "Device open error",FND_DEVICE);
	ret=write(dev, data, FND_MAX_DIGIT);
	assert2(ret>=0, "Device write error",FND_DEVICE);

	sleep(1);
	
	memset(data,0,sizeof(data));
	ret=read(dev,data,FND_MAX_DIGIT);
	assert2(ret>=0,"Device read error",FND_DEVICE);

	printf("Current FND Value: ");
	for(i=0; i<data_len;i++){
		printf("%d",data[i]);
	}
	printf("\n");

	close(dev);
	return 0;
}
