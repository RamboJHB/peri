#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <asm/io.h>
#include <mach/platform.h>

static const int LED0 = 4;
static const int LED1 = 5;
static const int BTN0 = 18;

struct gpio_s
{
    uint32_t gpfsel[7];
    uint32_t gpset[3];
    uint32_t gpclr[3];
    uint32_t gplev[3];
    uint32_t gpeds[3];
    uint32_t gpren[3];
    uint32_t gpfen[3];
    uint32_t gphen[3];
    uint32_t gplen[3];
    uint32_t gparen[3];
    uint32_t gpafen[3];
    uint32_t gppud[1];
    uint32_t gppudclk[3];
    uint32_t test[1];
}
*gpio_regs = (struct gpio_s *)__io_address(GPIO_BASE);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charlie, 2015");
MODULE_DESCRIPTION("Module, aussitot insere, aussitot efface");

static int LED;
static int major;
module_param(LED, int, 0);
MODULE_PARM_DESC(LED, "Nombre de led");



static void gpio_fsel(int pin, int fun)
{
    uint32_t reg = pin / 10;
    uint32_t bit = (pin % 10) * 3;
    uint32_t mask = 0b111 << bit;
    gpio_regs->gpfsel[reg] = (gpio_regs->gpfsel[reg] & ~mask) | ((fun << bit) & mask);
}

//gpio write
static void gpio_write(int pin, bool val)
{
    if (val)
        gpio_regs->gpset[pin / 32] = (1 << (pin % 32));
    else
        gpio_regs->gpclr[pin / 32] = (1 << (pin % 32));
}

//gpio read
static int gpio_read(int pin)
{
    return ((gpio_regs->gplev[pin / 32]) >> (pin % 32)) & 0x1;
}


static int 
open_ledbp(struct inode *inode, struct file *file) {
    printk(KERN_DEBUG "open(gpioJH)\n");
    gpio_fsel(BTN0,0);
    gpio_fsel(LED0,1);
    gpio_fsel(LED1,1);
    return 0;
}

static ssize_t 
read_ledbp(struct file *file, char *buf, size_t count, loff_t *ppos) {
    
    if(gpio_read(BTN0))
	buf[0] = '1';
    else
	buf[0] = '0';
    printk(KERN_DEBUG "read(%c)",*buf);
    return count;
}


//ledbp write
static ssize_t 
write_ledbp(struct file *file, const char *buf, size_t count, loff_t *ppos) {
    printk(KERN_DEBUG "write(%d)",*buf);
    int i;
    for(i = 0; i < count; i ++){
	if((*buf) == '0')
    		gpio_write(LED0 + i,0);
    	else
		gpio_write(LED0 + i,1);
    }
   
    return count;
}

static int 
release_ledbp(struct inode *inode, struct file *file) {
    printk(KERN_DEBUG "close(JH)\n");
    return 0;
}

struct file_operations fops_ledbp =
{
    .open       = open_ledbp,
    .read       = read_ledbp,
    .write      = write_ledbp,
    .release    = release_ledbp 
};

//init & cleaning up

static int __init mon_module_init(void)
{
    major = register_chrdev(0, "ledbp_JH", &fops_ledbp); // 0 est le numéro majeur qu'on laisse choisir par linux
    
    printk(KERN_DEBUG "init______________\n");
    printk(KERN_DEBUG "LED=%d !\n", LED);
    return 0;
}

static void __exit mon_module_cleanup(void)
{
   unregister_chrdev(major, "ledbp_JH");
   printk(KERN_DEBUG "exit_______________\n");
}

module_init(mon_module_init);
module_exit(mon_module_cleanup);
