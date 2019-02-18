#include "linux/kernel.h"
#include "linux/module.h"
#include "linux/fs.h"
#include "linux/init.h"
#include "linux/types.h"
#include "linux/errno.h"
#include "linux/uaccess.h"
#include "linux/kdev_t.h"
#define MAX_SIZE 1024
#define MAJOR_DEVNUM 240

static int dev_open(struct inode *inode, struct file *file);
static int dev_release(struct inode *inode, struct file *file);
static ssize_t dev_read(struct file *file, char __user *user, size_t t, loff_t *f);
static ssize_t dev_write(struct file *file, const char __user *user, size_t t, loff_t *f);

static char message[MAX_SIZE] = "This is xyx's character device";
static int dev_mutex = 0;
static char* devName = "xyxdev"; 

struct file_operations pStruct = { open:dev_open, release:dev_release, read:dev_read, write:dev_write, };

int init_module()
{
	int ret;
	ret = register_chrdev(MAJOR_DEVNUM, devName, &pStruct);
	if (ret < 0)
	{
		printk("Register dev failed!\n");
		return -1;
	}
	else
	{
		printk("Register dev successfully!\n");
		return 0;
	}
}

void cleanup_module()
{
	unregister_chrdev(MAJOR_DEVNUM, devName);
	printk("Unregister dev successfully!\n");
}

static int dev_open(struct inode *inode, struct file *file)
{
    if(dev_mutex)
        return -EBUSY;
    dev_mutex = 1;
	printk("main  device : %d\n", MAJOR(inode->i_rdev));
	printk("slave device : %d\n", MINOR(inode->i_rdev));
	try_module_get(THIS_MODULE);
	return 0;
}

static int dev_release(struct inode *inode, struct file *file)
{
	printk("Device released!\n");
	module_put(THIS_MODULE);
    dev_mutex = 0;
	return 0;
}

static ssize_t dev_read(struct file *file, char __user *user, size_t t, loff_t *f)
{
	if(copy_to_user(user,message,sizeof(message)))
	{
		return -EFAULT;
	}
	return sizeof(message);
}

static ssize_t dev_write(struct file *file, const char __user *user, size_t t, loff_t *f)
{
	if(copy_from_user(message,user,sizeof(message)))
	{
		return -EFAULT;
	}
	return sizeof(message);
}

