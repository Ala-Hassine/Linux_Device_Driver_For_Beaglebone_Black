#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("MIT");
MODULE_AUTHOR("Ala Hassine");
MODULE_DESCRIPTION("Simple Hello World module for BeagleBone Black");
MODULE_VERSION("1.0");

static int __init bbb_hello_init(void)
{
    printk(KERN_INFO "BBB Hello: Module Loaded Successfully!\n");
    printk(KERN_INFO "BBB Hello: BeagleBone Black Greeting\n");
    return 0;
}

static void __exit bbb_hello_exit(void)
{
    printk(KERN_INFO "BBB Hello: Module Unloaded\n");
}

module_init(bbb_hello_init);
module_exit(bbb_hello_exit);
