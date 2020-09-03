#include "../include/fpga_test.h"

int main(int argc, char**argv) {
	unsigned char buf[TEXT_LCD_MAX_BUF];
	int dev, len1, len2;
	int i;

	assert(2 <= argc && argc <= 3, "Usage:\n\tfpga_text_lcd_test <first line> <second line>\n");

	char errmsg[50];
	sprintf(errmsg, "%d alphanumeric characters on a line", TEXT_LCD_LINE_BUF);

	len1 = strlen(argv[1]);
	assert(len1 <= TEXT_LCD_LINE_BUF, errmsg);
	if (argc == 3) {
		len2 = strlen(argv[2]);
		assert(len2 <= TEXT_LCD_LINE_BUF, errmsg);
	}
	dev = open(TEXT_LCD_DEVICE, O_WRONLY);
	assert2(dev >= 0, "Device open error", TEXT_LCD_DEVICE);

	memset(buf, ' ', TEXT_LCD_MAX_BUF);
	memcpy(buf, argv[1], len1);
	if (argc == 3)
		memcpy(buf + TEXT_LCD_LINE_BUF, argv[2], len2);
 
	write(dev, buf, TEXT_LCD_MAX_BUF);

	close(dev);
	return 0;
}

