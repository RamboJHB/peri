#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>	//file structure

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Charlie, 2015");
MODULE_DESCRIPTION("Module, aussitot insere, aussitot efface");

//step 3
static int LED;
static int major;
module_param(LED, int, 0);//--------------------parameter LED sudo insmod ./module.ko LED=2
MODULE_PARM_DESC(LED, "Nombre de led"); 

static int open_ledbp(struct inode *inode, struct file *file) {
    printk(KERN_DEBUG "open()__________________\n");
    return 0;
}

static ssize_t read_ledbp(struct file *file, char *buf, size_t count, loff_t *ppos) {
    printk(KERN_DEBUG "read()__________________\n");
    return count;
}

static ssize_t write_ledbp(struct file *file, const char *buf, size_t count, loff_t *ppos) {
    printk(KERN_DEBUG "write()_____________________\n");
    return count;
}

static int release_ledbp(struct inode *inode, struct file *file) {
    printk(KERN_DEBUG "close()_________________________\n");
    return 0;
}

struct file_operations fops_ledbp =		
{
    .open       = open_ledbp,
    .read       = read_ledbp,
    .write      = write_ledbp,
    .release    = release_ledbp 
};



									//step 2
/*static int LED;
module_param(LED, int, 0);
MODULE_PARM_DESC(LED, "Nombre de led");

static int __init mon_module_init(void)
{
    printk(KERN_DEBUG "Hello World !::::::::::::::::\n");
    printk(KERN_DEBUG "LED=%d !\n", LED);
    return 0;
}

*/


static int __init mon_module_init(void)
{
    printk(KERN_DEBUG "Hello World !________________JL\n");
    
    printk(KERN_DEBUG "LED=%d !\n", LED);
    major = register_chrdev(0, "ledbpJL", &fops_ledbp); // 0 est le numéro majeur qu'on laisse choisir par linux
    return 0;
}

static void __exit mon_module_cleanup(void)
{
   
   unregister_chrdev(major, "ledbpJL");
   
   printk(KERN_DEBUG "Goodbye World! heheheheheheheheh\n");
}

module_init(mon_module_init);
module_exit(mon_module_cleanup);
