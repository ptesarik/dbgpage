#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/debugfs.h>
#include <linux/string.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("TERADATA");
MODULE_DESCRIPTION("Peek into struct page data");

static struct dentry *dbgdir;

static struct inode_operations dbg_inode_ops;

static int dbg_mkdir(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	struct inode *inode = new_inode(dir->i_sb);

	if (!inode)
		return -ENOMEM;

	inode->i_ino = get_next_ino();
	inode->i_mode = mode | S_IFDIR;
	inode->i_atime = inode->i_mtime = inode->i_ctime = CURRENT_TIME;
	inode->i_op = &simple_dir_inode_operations;
	inode->i_fop = &simple_dir_operations;

	/* directory inodes start off with i_nlink == 2 (for "." entry) */
	inc_nlink(inode);

	d_instantiate(dentry, inode);
	dget(dentry);   /* Extra count - pin the dentry in core */
	dir->i_mtime = dir->i_ctime = CURRENT_TIME;
	inc_nlink(dir);

	return 0;
}

static int __init dbgpage_init(void)
{
	struct inode *dbgino;

	dbgdir = debugfs_create_dir("page", NULL);
	if (IS_ERR(dbgdir)) {
		printk(KERN_ERR "dbgpage: debugfs not initialized\n");
		return -ENODEV;
	}
	if (!dbgdir) {
		printk(KERN_ERR "dbgpage: cannot create debugfs directory\n");
		return -ENOMEM;
	}

	dbgino = dbgdir->d_inode;
	memcpy(&dbg_inode_ops, dbgino->i_op, sizeof(dbg_inode_ops));
	dbg_inode_ops.mkdir = dbg_mkdir;
	dbgino->i_op = &dbg_inode_ops;

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
