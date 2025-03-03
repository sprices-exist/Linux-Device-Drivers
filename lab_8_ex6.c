#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include<linux/uaccess.h> 
#include<linux/sysfs.h> 
#include<linux/kobject.h> 
 
 
volatile int test_value = 0;
 
 
dev_t dev = 0;
static struct class *dev_class;
static struct cdev test_cdev;
struct kobject *kobj_ref;


static ssize_t sysfs_show(struct kobject *kobj, 
                struct kobj_attribute *attr, char *buf)
{
        pr_info("Read\n");
        return sprintf(buf, "%d", test_value);
}


static ssize_t sysfs_store(struct kobject *kobj, 
                struct kobj_attribute *attr,const char *buf, size_t count)
{
        pr_info("Write\n");
        sscanf(buf,"%d",&test_value);
        return count;
}

static int file_open(struct inode *inode, struct file *file)
{
        pr_info("File Opened\n");
        return 0;
}

static int file_release(struct inode *inode, struct file *file)
{
        pr_info("File Closed\n");
        return 0;
}
 

static ssize_t file_read(struct file *filp, 
                char __user *buf, size_t len, loff_t *off)
{
        pr_info("Read\n");
        return 0;
}

static ssize_t file_write(struct file *filp, 
                const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Write\n");
        return len;
}

struct kobj_attribute test_attr = __ATTR(test_value, 0660, sysfs_show, sysfs_store);


static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = file_read,
        .write          = file_write,
        .open           = file_open,
        .release        = file_release,
};

 



static int __init test_driver_init(void)
{
        if((alloc_chrdev_region(&dev, 0, 1, "test_Dev")) <0){
                pr_info("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(dev), MINOR(dev));
 
        cdev_init(&test_cdev,&fops);
 
        if((cdev_add(&test_cdev,dev,1)) < 0){
            pr_info("Cannot add the device to the system\n");
            goto r_class;
        }
 
        if((dev_class = class_create(THIS_MODULE,"test_class")) == NULL){
            pr_info("Cannot create the struct class\n");
            goto r_class;
        }
 
        if((device_create(dev_class,NULL,dev,NULL,"test_device")) == NULL){
            pr_info("Cannot create the Device\n");
            goto r_device;
        }
 

        kobj_ref = kobject_create_and_add("test_sysfs",kernel_kobj);
 
        if(sysfs_create_file(kobj_ref,&test_attr.attr)){
                pr_err("Cannot create sysfs file\n");
                goto r_sysfs;
    }
        return 0;
 
r_sysfs:
        kobject_put(kobj_ref); 
        sysfs_remove_file(kernel_kobj, &test_attr.attr);
 
r_device:
        class_destroy(dev_class);
r_class:
        unregister_chrdev_region(dev,1);
        cdev_del(&test_cdev);
        return -1;
}



static void __exit test_driver_exit(void)
{
        kobject_put(kobj_ref); 
        sysfs_remove_file(kernel_kobj, &test_attr.attr);
        device_destroy(dev_class,dev);
        class_destroy(dev_class);
        cdev_del(&test_cdev);
        unregister_chrdev_region(dev, 1);
        pr_info("Device Driver Removed\n");
}

module_init(test_driver_init);
module_exit(test_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sprices");
MODULE_DESCRIPTION("sysfs synchronized rw");
MODULE_VERSION("1.0");
