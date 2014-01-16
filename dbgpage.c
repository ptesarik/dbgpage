#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TERADATA");
MODULE_DESCRIPTION("Peek into struct page data");

static int __init dbgpage_init(void)
{
	printk(KERN_INFO "dbgpage: Module loaded\n");
	return 0;
}

static void __exit dbgpage_cleanup(void)
{
	printk(KERN_INFO "dbgpage: Unloaded\n");
}

module_init(dbgpage_init);
module_exit(dbgpage_cleanup);
