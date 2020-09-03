#include "../include/fpga_driver.h"


static unsigned char *iom_fpga_step_motor_addr;

//prototypes
static ssize_t iom_step_motor_write(struct file *file, const char *buf, size_t count, loff_t *f_pos);


struct file_operations iom_step_motor_fops = {
	.owner = THIS_MODULE,
	.write = iom_step_motor_write
};



static ssize_t iom_step_motor_write(struct file *file, const char *buf, size_t count, loff_t *f_pos)
{
	unsigned char value[3];
	unsigned short _s_value = 0;
	
	if(count != 3) return -EFAULT;
	
	if(copy_from_user(&value,buf,count)) return -EFAULT;
	
	_s_value = value[0] & 0xF;
	outw(_s_value,(unsigned int)iom_fpga_step_motor_addr);
	
	_s_value = value[1] & 0xF;
	outw(_s_value,(unsigned int)iom_fpga_step_motor_addr + 2);
	
	_s_value = value[2] & 0xFF;
	outw(_s_value,(unsigned int)iom_fpga_step_motor_addr + 4);
	
	return count;
}


int __init iom_step_motor_init(void)
{
	int result;
	result = register_chrdev(IOM_STEP_MOTOR_MAJOR, IOM_STEP_MOTOR_NAME, &iom_step_motor_fops);

	if(result < 0)
	{
		printk(KERN_WARNING "Can't get any major number \n");
		return result;
	}

	iom_fpga_step_motor_addr = ioremap(IOM_STEP_MOTOR_ADDRESS, 0x6);
	printk("init module %s, major number: %d\n", IOM_STEP_MOTOR_NAME, IOM_STEP_MOTOR_MAJOR);
	return 0;
}

void __exit iom_step_motor_exit(void)
{
	iounmap(iom_fpga_step_motor_addr);
	unregister_chrdev(IOM_STEP_MOTOR_MAJOR, IOM_STEP_MOTOR_NAME);
}

module_init(iom_step_motor_init);
module_exit(iom_step_motor_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SANGWOO");
MODULE_DESCRIPTION("FPGA FND driver");