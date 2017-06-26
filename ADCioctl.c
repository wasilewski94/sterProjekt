/*
 * linux/adc/oss/adccard.c
 *
 * adc card driver for Linux
 *
 *
 * Copyright (C) by Hannu Savolainen 1993-1997
 *
 * OSS/Free for Linux is distributed under the GNU GENERAL PUBLIC LICENSE (GPL)
 * Version 2 (June 1991). See the "COPYING" file distributed with this software
 * for more info.
 *
 *
 * Thomas Sailer     : ioctl code reworked (vmalloc/vfree removed)
 *                   integrated adc_switch.c
 * Stefan Reinauer   : integrated /proc/adc (equals to /dev/sndstat,
 *                   which should disappear in the near future)
 * Eric Dumas	     : devfs support (22-Jan-98) <dumas@linux.eu.org> with
 *                   fixups by C. Scott Ananian <cananian@alumni.princeton.edu>
 * Richard Gooch     : moved common (non OSS-specific) devices to adc_core.c
 * Rob Riggs	     : Added persistent DMA buffers support (1998/10/17)
 * Christoph Hellwig : Some cleanup work (2000/03/01)
 */

#include <linux/kernel.h>   
#include <linux/module.h>
#include <linux/spinlock.h>
#include <linux/sched.h>

/* Zbiory nagłówkowe dla urządzeń znakowych */
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/cdev.h>
//#include <linux/mm.h>
//#include <linux/config.h>
#include <linux/ioport.h>
#include <linux/interrupt.h>
#include <linux/poll.h>
#include <asm/io.h>
#include <asm/bitops.h>

/* Tu dołączamy nasze kody IOCTL */
#include <linux/ioctl.h>

MODULE_LICENSE("GPL v2");
#define DEVICE_NAME "adc_ster"

dev_t my_dev=0;
struct cdev * my_cdev = NULL;
#define PROC_BLOCK_SIZE (3*1024)
static struct class *adc_class = NULL;

static int adc_open(struct inode *inode, struct file *file)
{
  printk(KERN_ALERT "wywolano funkcje adc_open");return 0;
}
static int adc_release(struct inode *inode, struct file *file)
{
  printk(KERN_ALERT "wywolano funkcje release()");
	return 0;
}

static long adc_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
  printk(KERN_ALERT "wywolano funkcje adc_ioctl()");
	
  return 0;
}

const struct file_operations adc_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= adc_ioctl,
	.open		= adc_open,
	.release	= adc_release,
};

static void adc_cleanup(void)
{
  /* Usuwamy urządzenie z klasy */
  if(my_dev && adc_class) {
    device_destroy(adc_class,my_dev);
  }
  /* Usuwamy strukturę urządzenia z systemu*/
  if(my_cdev) { 
    cdev_del(my_cdev);
    my_cdev=NULL;
  }
  /* Zwalniamy numer urządzenia */
  if(my_dev) {
    unregister_chrdev_region(my_dev, 1);
  }
  /* Wyrejestrowujemy klasę */
  if(adc_class) {
    class_destroy(adc_class);
    adc_class=NULL;
  }
  printk(KERN_ALERT "do widzenia");

}

static int __init adc_init(void)
{
   int res=0;
  /* Alokujemy numer urządzenia */
  res=alloc_chrdev_region(&my_dev, 0, 1, DEVICE_NAME);
  if(res) {
    printk (KERN_ALERT "Alocation of the device number for %s failed\n",
            DEVICE_NAME);
    return res; 
  };
  /* Tworzymy klasę opisującą nasze urządzenie - aby móc współpracować z systemem udev */
  adc_class = class_create(THIS_MODULE, DEVICE_NAME);
  if (IS_ERR(adc_class)) {
    printk(KERN_ERR "Error creating adc_class.\n");
 
    res=PTR_ERR(adc_class);
    goto err1;
  }

  /* Alokujemy strukturę opisującą urządzenie znakowe */
  my_cdev = cdev_alloc( );
  my_cdev->ops = &adc_fops;
  my_cdev->owner = THIS_MODULE;
  /* Dodajemy urządzenie znakowe do systemu */
  res=cdev_add(my_cdev, my_dev, 1);
  if(res) {
    printk (KERN_ALERT "Registration of the device number for %s failed\n",
            DEVICE_NAME);
    res=-EFAULT;
    goto err1;
  };
  
  device_create(adc_class,NULL,my_dev,NULL,"%s%d", DEVICE_NAME, MINOR(my_dev));
  printk (KERN_ALERT "%s The major device number is %d.\n",
	  "Registeration is a success.",
	  MAJOR(my_dev));
  return res;
 err1:
  adc_cleanup();
  return res;
  printk(KERN_ALERT "Zaladowano moj modul");
	return 0;
}


module_init(adc_init);
module_exit(adc_cleanup);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("OSS adc subsystem");
MODULE_AUTHOR("Krzysztof Wasilewski");
