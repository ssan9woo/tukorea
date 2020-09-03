#include "../include/fpga_test.h"
#include "../include/fpga_dot_font.h"

int main(int argc, char**argv) {
	int dev, num;
	ssize_t ret;
	int i;

	char usage[50];
	sprintf(usage, "Usage:\n\tfpga_dot_test <%d ~ %d>\n", DOT_MIN, DOT_MAX);
	assert(argc == 2, usage);

	num = atoi(argv[1]);
	assert(DOT_MIN <= num && num <= DOT_MAX, "Invalid number");

	dev = open(DOT_DEVICE, O_WRONLY);
	assert2(dev >= 0, "Device open error", DOT_DEVICE);

	ret = write(dev, fpga_number[num], sizeof(fpga_number[num]));
	assert2(ret >= 0, "Device write error", DOT_DEVICE);

	close(dev);
	return 0;
}
