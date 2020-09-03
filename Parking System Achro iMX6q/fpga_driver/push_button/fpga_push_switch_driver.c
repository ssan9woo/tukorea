#include "../include/fpga_driver.h"


static unsigned char *iom_fpga_push_switch_addr;

//prototypes
static ssize_t iom_push_switch_read(struct file *file, char *buf, size_t count, loff_t *f_pos);
              
struct file_operations iom_push_switch_fops = {
	.owner = THIS_MODULE,
	.read = iom_push_switch_read
};

static ssize_t iom_push_switch_read(struct file *file, char *buf, size_t count, loff_t *f_pos)
{
	unsigned char value[IOM_PUSH_SWITCH_MAX_BUTTON];
	unsigned short _s_value;
	int i;

	if(count > IOM_PUSH_SWITCH_MAX_BUTTON)
		count = IOM_PUSH_SWITCH_MAX_BUTTON;

	for(i = 0; i < count; i++)
	{
		_s_value = inw((unsigned int)iom_fpga_push_switch_addr+i*2);
		value[i] = _s_value && 0xFF;	
	}

	if(copy_to_user(buf,value,count))
		return -EFAULT;
	return count;
}

int __init iom_push_switch_init(void)
{
	int result;
	result = register_chrdev(IOM_PUSH_SWITCH_MAJOR, IOM_PUSH_SWITCH_NAME, &iom_push_switch_fops);

	if(result < 0)
	{
		printk(KERN_WARNING "Can't get any major number \n");
		return result;
	}

	iom_fpga_push_switch_addr = ioremap(IOM_PUSH_SWITCH_ADDRESS, 0x12);
	printk("init module %s, major number: %d\n", IOM_PUSH_SWITCH_NAME, IOM_PUSH_SWITCH_MAJOR);
	return 0;
}

void __exit iom_push_switch_exit(void)
{
	iounmap(iom_fpga_push_switch_addr);
	unregister_chrdev(IOM_PUSH_SWITCH_MAJOR, IOM_PUSH_SWITCH_NAME);
}

module_init(iom_push_switch_init);
module_exit(iom_push_switch_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SANGWOO");
MODULE_DESCRIPTION("FPGA FND driver");