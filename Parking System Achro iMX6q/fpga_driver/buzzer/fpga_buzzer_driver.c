#include "/root/work/achroimx6q/fpga_driver/include/fpga_driver.h"

static int buzzer_port_usage=0;
static unsigned char *iom_fpga_buzzer_addr;

static ssize_t iom_buzzer_write(struct file *file, const char *buf, size_t count, loff_t *f_pos);
static ssize_t iom_buzzer_read(struct file *file, char *buf, size_t count, loff_t *f_pos);
static int iom_buzzer_open(struct inode *inode, struct file *file);
static int iom_buzzer_release(struct inode *inode, struct file *file);

struct file_operations iom_buzzer_fops={
	.owner	=	THIS_MODULE,
	.open	=	iom_buzzer_open,
	.write	=	iom_buzzer_write,
	.read	=	iom_buzzer_read,
	.release=	iom_buzzer_release
};
static int iom_buzzer_open(struct inode *inode, struct file *file){
	if(buzzer_port_usage)
		return -EBUSY;
	buzzer_port_usage=1;
	return 0;
}
static int iom_buzzer_release(struct inode *inode, struct file *file){
	buzzer_port_usage=0;
	return 0;
}
static ssize_t iom_buzzer_write(struct file *file, const char *buf, size_t count, loff_t *f_pos){
	unsigned char value;
	unsigned short _s_value;

	if(copy_from_user(&value, buf, 1))
		return -EFAULT;
	_s_value=value & 0xF;
	outw(_s_value, (unsigned int) iom_fpga_buzzer_addr);
	return 1;
}
static ssize_t iom_buzzer_read(struct file *file, char *buf, size_t count, loff_t *f_pos){
	unsigned char value;
	unsigned short _s_value;

	_s_value=inw((unsigned int)iom_fpga_buzzer_addr);
	value=_s_value & 0xF;
	if(copy_to_user(buf, &value,1))
		return -EFAULT;
	return 1;
}

int __init iom_buzzer_init(void){
	int result;
	result=register_chrdev(IOM_BUZZER_MAJOR, IOM_LED_NAME, &iom_buzzer_fops);

	if(result < 0){
		printk(KERN_WARNING "Can't get any major number\n");
		return result;
	}

	iom_fpga_buzzer_addr=ioremap(IOM_BUZZER_ADDRESS, 0x1);
	printk("init module %s, major number: %d\n",IOM_BUZZER_NAME,IOM_LED_MAJOR);
	return 0;
}

void __exit iom_buzzer_exit(void){
	iounmap(iom_fpga_buzzer_addr);
	unregister_chrdev(IOM_BUZZER_MAJOR, IOM_LED_NAME);
}
module_init(iom_buzzer_init);
module_exit(iom_buzzer_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LJW");
MODULE_DESCRIPTION("FPGA BUZZER driver");
