#include "/root/work/achroimx6q/fpga_driver/include/fpga_driver.h"

static int dot_port_usage = 0;
static unsigned char *iom_fpga_dot_addr;


static ssize_t iom_dot_write(struct file *file, const char *buf, size_t count, loff_t *f_pos);
static int iom_dot_open(struct inode *inode, struct file *file);
static int iom_dot_release(struct inode *inode, struct file *file);

struct file_operations iom_dot_fops = {
	.owner	= 	THIS_MODULE,
	.open	=	iom_dot_open,
	.write	=	iom_dot_write,
	.release=	iom_dot_release
};

static int iom_dot_open(struct inode *inode, struct file *file) {
	if (dot_port_usage)
		return -EBUSY;
	dot_port_usage = 1;
	return 0;
}

static int iom_dot_release(struct inode *inode, struct file *file) {
	dot_port_usage = 0;
	return 0;
}
static ssize_t iom_dot_write(struct file *file, const char *buf, size_t count, loff_t *f_pos) {
	unsigned char value[IOM_DOT_MAX_ROW]; 
	unsigned short _s_value;
	int i;

	if (count >= IOM_DOT_MAX_ROW)
		count = IOM_DOT_MAX_ROW;

	if (copy_from_user(value, buf, count))
		return -EFAULT;

	for (i = 0; i < count; i++) {
		_s_value = value[i] & 0x7F;
		outw(_s_value, (unsigned int)iom_fpga_dot_addr + i * 2);
	}
	return count;
}


int __init iom_dot_init(void) {
	int result = register_chrdev(IOM_DOT_MAJOR, IOM_DOT_NAME, &iom_dot_fops);
	if (result < 0) {
		printk(KERN_WARNING "Can't get any major number\n");
		return result;
	}
	
	iom_fpga_dot_addr = ioremap(IOM_DOT_ADDRESS, 0x14);  // 사용할 메모리 크기
	printk("init module %s, major number: %d\n", IOM_FND_NAME, IOM_FND_MAJOR);
	return 0;
}

void __exit iom_dot_exit(void) {
	iounmap(iom_fpga_dot_addr);
	unregister_chrdev(IOM_DOT_MAJOR, IOM_DOT_NAME);
}

module_init(iom_dot_init);
module_exit(iom_dot_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LJW");
MODULE_DESCRIPTION("FPGA DOT driver");


