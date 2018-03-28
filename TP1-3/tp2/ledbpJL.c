#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>				//from step 3
#include <asm/io.h>
#include <mach/platform.h>                      //from step 4


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charlie, 2015");
MODULE_DESCRIPTION("Module, aussitot insere, aussitot efface");


static int LED; //=17 define at init dev
static int major;
static const int LED0 = 4;
static const int BTN0 = 18;    
            
module_param(LED, int, 0);
MODULE_PARM_DESC(LED, "Nombre de led");



//------------------------------------------------------------------------------
//                                GPIO ACCES
//------------------------------------------------------------------------------

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

static void gpio_fsel(int pin, int fun){
    uint32_t reg = pin / 10;
    uint32_t bit = (pin % 10) * 3;
    uint32_t mask = 0b111 << bit;
    gpio_regs->gpfsel[reg] = (gpio_regs->gpfsel[reg] & ~mask) | ((fun << bit) & mask);
}

static uint32_t gpio_read(int pin){
     return ((gpio_regs->gplev[pin / 32]) >> (pin % 32)) & 0x1;  
}

static void gpio_write(int pin, bool val){
    if (val)
        gpio_regs->gpset[pin / 32] = (1 << (pin % 32));
    else
        gpio_regs->gpclr[pin / 32] = (1 << (pin % 32));
}

//------------------------------------------------------------------------------
//                                Functions
//------------------------------------------------------------------------------
static int open_ledbp(struct inode *inode, struct file *file) {	//open & select pin/gpio function
    printk(KERN_DEBUG "open()__________________JL\n");
    gpio_fsel(BTN0,0);	
    gpio_fsel(LED0,1);
    gpio_fsel(LED,1);
    return 0;
}

static ssize_t read_ledbp(struct file *file, char *buf, size_t count, loff_t *ppos) {//read button
    if (gpio_read(BTN0) == 1) 
	buf[0] = '1';
    else
        buf[0] = '0';
    printk(KERN_DEBUG "read(%c)____JL\n",buf[0]);
    return count;
}

static ssize_t write_ledbp(struct file *file, const char *buf, size_t count, loff_t *ppos) {//write LED
    printk(KERN_DEBUG "write(%c)____JL\n",buf[0]);
    int led[2];  
    led[0] = LED0;
    led[1] = LED;  
    int i;
    for ( i = 0 ; i <count ; i ++ ){
        if (buf[i] == '0')
            gpio_write(led[i],false);       
        else
            gpio_write(led[i],true);
    }

    return count;
}

static int release_ledbp(struct inode *inode, struct file *file) { //called in quitting program
    printk(KERN_DEBUG "close()_________________________JL\n");
    gpio_fsel(LED0, 0);    
    return 0;
}

struct file_operations fops_ledbp =
{
    .open       = open_ledbp,
    .read       = read_ledbp,
    .write      = write_ledbp,
    .release    = release_ledbp 
};





//------------------------------------------------------------------------------
//                              MODULE INIT & EXIT 
//------------------------------------------------------------------------------


static int __init mon_module_init(void)
{
    major = register_chrdev(0, "ledbpJL", &fops_ledbp); // 0 est le numéro majeur qu'on laisse choisir par linux
    printk(KERN_DEBUG "Hello World !________________JL\n");
    printk(KERN_DEBUG "LED=%d !\n", LED);
    printk(KERN_DEBUG "major=%d !\n", major);
    return 0;
}

static void __exit mon_module_cleanup(void)
{
   
   unregister_chrdev(major, "ledbpJH");
   printk(KERN_DEBUG "Goodbye World! heheheheheheheheh\n");
}

module_init(mon_module_init);
module_exit(mon_module_cleanup);
