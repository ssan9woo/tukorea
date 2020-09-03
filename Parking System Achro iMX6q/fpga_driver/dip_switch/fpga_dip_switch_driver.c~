#include "../include/fpga_driver.h"

static int dip_switch_port_usage = 0;
static unsigned char *iom_fpga_dip_switch_addr;

//prototypes
static ssize_t iom_dip_switch_read(struct file *file, char *buf, size_t count, loff_t *f_pos);
static int iom_dip_switch_open(struct inode *inode, struct file *file);
static int iom_dip_switch_release(struct inode *inode, struct file *file);
              
struct file_operations iom_dip_switch_fops = {
	.owner = THIS_MODULE,
	.open = iom_dip_switch_open,
	.read = iom_dip_switch_read,
	.release = iom_dip_switch_release
};

static int iom_dip_switch_open(struct inode *inode, struct file *file)
{
	if(dip_switch_port_usage)
		return -EBUSY;
	dip_switch_port_usage = 1;
	return 0;
}


static int iom_dip_switch_release(struct inode *inode, struct file *file)
{
	dip_switch_port_usage = 0;
	return 0;
}


static ssize_t iom_dip_switch_read(struct file *file, char *buf, size_t count, loff_t *f_pos)
{
	unsigned char value;
	unsigned short _s_value;

	_s_value = inw((unsigned int)iom_fpga_dip_switch_addr);
	value = _s_value & 0xFF;	
	
	if(copy_to_user(buf,&value,1))
		return -EFAULT;
	return count;
}

int __init iom_dip_switch_init(void)
{
	int result;
	result = register_chrdev(IOM_DIP_SWITCH_MAJOR, IOM_DIP_SWITCH_NAME, &iom_dip_switch_fops);

	if(result < 0)
	{
		printk(KERN_WARNING "Can't get any major number \n");
		return result;
	}

	iom_fpga_dip_switch_addr = ioremap(IOM_DIP_SWITCH_ADDRESS, 0x1);
	printk("init module %s, major number: %d\n", IOM_DIP_SWITCH_NAME, IOM_DIP_SWITCH_MAJOR);
	return 0;
}

void __exit iom_dip_switch_exit(void)
{
	iounmap(iom_fpga_dip_switch_addr);
	unregister_chrdev(IOM_DIP_SWITCH_MAJOR, IOM_DIP_SWITCH_NAME);
}

module_init(iom_dip_switch_init);
module_exit(iom_dip_switch_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LJW");
MODULE_DESCRIPTION("FPGA FND driver");
