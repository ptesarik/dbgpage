#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TERADATA");
MODULE_DESCRIPTION("Peek into struct page data");

static struct dentry *dbgdir;

static int __init dbgpage_init(void)
{
	dbgdir = debugfs_create_dir("page", NULL);
	if (IS_ERR(dbgdir)) {
		printk(KERN_ERR "dbgpage: debugfs not initialized\n");
		return -ENODEV;
	}
	if (!dbgdir) {
		printk(KERN_ERR "dbgpage: cannot create debugfs directory\n");
		return -ENOMEM;
	}

	printk(KERN_INFO "dbgpage: Module loaded\n");
	return 0;
}

static void __exit dbgpage_cleanup(void)
{
	debugfs_remove_recursive(dbgdir);
	printk(KERN_INFO "dbgpage: Unloaded\n");
}

module_init(dbgpage_init);
module_exit(dbgpage_cleanup);
