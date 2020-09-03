#include "../include/fpga_test.h"

unsigned char quit = 0;

void user_signal1(int sig){
	quit = 1;
}

void usage() {
	printf("Usage:\n");
	printf("\tfpga_step_motor_test <Motor Action> <Motor Diretion> <Speed>\n");
	printf("\tMotor Action : %d(Stop) / %d(Start)\n", STEP_MOTOR_OFF, STEP_MOTOR_ON);
	printf("\tMotor Direction : %d(Left) / %d(Right)\n", STEP_MOTOR_DIR_LEFT, STEP_MOTOR_DIR_RIGHT);
	printf("\tMotor Speed : %d(Fast) ~ %d(Slow)\n", STEP_MOTOR_SPDVAL_MIN, STEP_MOTOR_SPDVAL_MAX);
	printf("\tex) fpga_step_motor_test 1 0 10\n");
}







int main(int argc, char **argv)
{
	unsigned char state[3];
	int dev, len;
	int action, dir, speed;

	if (argc != 4) {
		usage();
		exit(1);
	}

	action = atoi(argv[1]);
	assert(action == STEP_MOTOR_OFF || action == STEP_MOTOR_ON, "Invalid motor action");
	dir = atoi(argv[2]);
	assert(dir == STEP_MOTOR_DIR_LEFT || dir == STEP_MOTOR_DIR_RIGHT, "Invalid motor direction");
	speed = atoi(argv[3]);
	assert(STEP_MOTOR_SPDVAL_MIN <= speed && speed <= STEP_MOTOR_SPDVAL_MAX, "Invalid motor speed");

	memset(state, 0, sizeof(state));
	state[0] = (unsigned char)action;
	state[1] = (unsigned char)dir;
	state[2] = (unsigned char)speed;
	
	dev = open(STEP_MOTOR_DEVICE, O_WRONLY);
	assert2(dev >= 0, "Device open error", STEP_MOTOR_DEVICE);

	write(dev, state, 3);

	close(dev);
	return 0;
}
