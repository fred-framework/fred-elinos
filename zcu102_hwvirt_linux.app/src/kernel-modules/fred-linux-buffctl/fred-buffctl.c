// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Fred for Linux. Experimental support.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/types.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

#include <linux/dmaengine.h>
#include <linux/dma-mapping.h>
#include <linux/of_device.h>

#include "fred-buffctl-shared.h"

/*-------------------------------------------------------------------------------------*/

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marco Pagani");
MODULE_DESCRIPTION("Fred buffers control module");

/*-------------------------------------------------------------------------------------*/

#define DRIVER_NAME	"fred_buffctl"

#define STATE_BUSY	1

/*-------------------------------------------------------------------------------------*/

/* Buffer char device */
struct fred_buff {
	unsigned long state;
	struct list_head node;

	/* User/Kernel interface struct */
	struct fred_buff_if buff_if;

	/* Kernel space buffer pointer */
	void *buff_kvp;

	/* Char device */
	struct miscdevice m_dev;
};

/* Buffer Control char device */
struct fred_buffctl {
	unsigned long state;

	/* Buffers list head */
	struct list_head buffs_head;
	/* Used to set buffers ids */
	uint32_t buffs_count;

	/* Char device */
	struct miscdevice m_dev;
};

static struct fred_buffctl *fred_buffctl_lp;

/*-------------------------------------------------------------------------------------*/

static int buffs_open(struct inode *inode_p, struct file *file_p)
{
	struct fred_buff *fred_buff_p;
	struct miscdevice *misc_dev;

	misc_dev = file_p->private_data;
	fred_buff_p = container_of(misc_dev, struct fred_buff, m_dev);

	/* Only one user at a time (stick to the FRED model) */
	if (test_and_set_bit_lock(STATE_BUSY, &fred_buff_p->state)) {
		pr_info("Fred_Buff: device %u already open!\n",
			fred_buff_p->buff_if.id);
		return -EBUSY;
	}

	pr_info("Fred_Buff: buffer %u opened\n", fred_buff_p->buff_if.id);

	return 0;
}

static int buffs_release(struct inode *inode_p, struct file *file_p)
{
	struct fred_buff *fred_buff_p;
	struct miscdevice *misc_dev;

	misc_dev = file_p->private_data;
	fred_buff_p = container_of(misc_dev, struct fred_buff, m_dev);

	/* Release the device */
	clear_bit_unlock(STATE_BUSY, &fred_buff_p->state);

	pr_info("Fred_Buff: buffer %u released\n", fred_buff_p->buff_if.id);

	return 0;
}

static int buffs_mmap(struct file *file_p, struct vm_area_struct *vma)
{
	int retval;
	struct fred_buff *fred_buff_p;
	struct miscdevice *misc_dev;

	misc_dev = file_p->private_data;
	fred_buff_p = container_of(misc_dev, struct fred_buff, m_dev);

	pr_debug("Fred_Buff: buffer %u request map\n", fred_buff_p->buff_if.id);

	/* Create userspace mapping for the DMA-coherent memory */
	retval = dma_mmap_coherent(fred_buff_p->m_dev.this_device, vma,
					fred_buff_p->buff_kvp,
					fred_buff_p->buff_if.phy_addr,
					vma->vm_end - vma->vm_start);
	if (retval) {
		pr_err("Fred_BuffCtl: cannot mmap buffer %u\n",
			fred_buff_p->buff_if.id);
		return retval;
	}

	pr_info("Fred_BuffCtl: buffer %u mapped\n", fred_buff_p->buff_if.id);

	return retval;
}

/*-------------------------------------------------------------------------------------*/

static const char dev_name_base[] = "fred!buff";

static const struct file_operations buffs_file_ops = {
	.owner		= THIS_MODULE,
	.open		= buffs_open,
	.release	= buffs_release,
	.mmap		= buffs_mmap
};

/*-------------------------------------------------------------------------------------*/

static int alloc_dma_membuff_(struct fred_buff *fred_buff_p)
{
	/* Allocate a non cached contiguous buffer using the CMA */
	fred_buff_p->buff_kvp = dmam_alloc_coherent(
					fred_buff_p->m_dev.this_device,
					fred_buff_p->buff_if.length,
					(dma_addr_t *)&fred_buff_p->buff_if.phy_addr,
					GFP_KERNEL);

	if (!fred_buff_p->buff_kvp) {
		pr_err("Fred_BuffCtl: contiguos buffer allocation error\n");
		return -ENOMEM;
	}

	pr_info("Fred_BuffCtl: Contiguos mem buffer allocated at: phy:0x%llx, virt:%p\n",
		(unsigned long long)fred_buff_p->buff_if.phy_addr,
		fred_buff_p->buff_kvp);

	return 0;
}

static int reg_new_buff_(struct fred_buffctl *fred_buffctl_p,
				struct fred_buff *fred_buff_p)
{
	int retval;

	/* Set buffer id */
	fred_buff_p->buff_if.id = fred_buffctl_p->buffs_count++;

	/* Encode id in the dev name */
	snprintf(fred_buff_p->buff_if.dev_name, sizeof(fred_buff_p->buff_if.dev_name),
		"%s_%u", dev_name_base, fred_buff_p->buff_if.id);

	/* Init buffer control character device */
	fred_buff_p->m_dev.minor = MISC_DYNAMIC_MINOR;
	fred_buff_p->m_dev.fops = &buffs_file_ops;
	fred_buff_p->m_dev.name = fred_buff_p->buff_if.dev_name;

	retval = misc_register(&fred_buff_p->m_dev);
	if (retval) {
		pr_err("Fred_BuffCtl: failed to register buffctl misc dev\n");
		goto out;
	}

	retval = dma_set_coherent_mask(fred_buff_p->m_dev.this_device, DMA_BIT_MASK(32));
	if (retval) {
		pr_err("Fred_BuffCtl: Cannot set 32-bit DMA mask: %d\n",retval);
		goto err_misc;
	}

	/* Allocate a contiguous memory buffer */
	retval = alloc_dma_membuff_(fred_buff_p);
	if (retval)
		goto err_misc;

	/* Add to the buffers list */
	list_add(&fred_buff_p->node, &fred_buffctl_p->buffs_head);

	pr_info("Fred_BuffCtl: new buffer allocated: id:%u, size:%zu\n",
		fred_buff_p->buff_if.id,
		fred_buff_p->buff_if.length);

	retval = 0;
	goto out;

err_misc:
	misc_deregister(&fred_buff_p->m_dev);
out:
	return retval;
}

static int dereg_buff_dev_(struct fred_buff *fred_buff_p)
{
	BUG_ON(!fred_buff_p);

	if (test_bit(STATE_BUSY, &fred_buff_p->state)) {
		pr_info("Fred_BuffCtl: buffer id:%u, size:%zu is busy. Cannot remove\n",
			fred_buff_p->buff_if.id,
			fred_buff_p->buff_if.length);
		return -1;
	}

	misc_deregister(&fred_buff_p->m_dev);

	pr_info("Fred_BuffCtl: buffer id:%u, size:%zu removed\n",
		fred_buff_p->buff_if.id,
		fred_buff_p->buff_if.length);

	return 0;
}

static int free_buff_(struct fred_buffctl *fred_buffctl_p, uint32_t buff_usr_id)
{
	struct list_head *cursor, *tmp;
	struct fred_buff *fred_buff_p;

	list_for_each_safe(cursor, tmp, &fred_buffctl_p->buffs_head) {

		fred_buff_p = list_entry(cursor, struct fred_buff, node);
		if (fred_buff_p->buff_if.id == buff_usr_id) {
			if (dereg_buff_dev_(fred_buff_p))
				return -1;

			list_del(cursor);
			kfree(fred_buff_p);

			return 0;
		}
	}

	pr_info("Fred_BuffCtl: Warn! buffer id:%u, size:%zu is missing!\n",
		fred_buff_p->buff_if.id,
		fred_buff_p->buff_if.length);

	return -1;
}

static int free_all_buffs_(struct fred_buffctl *fred_buffctl_p)
{
	struct list_head *cursor, *tmp;
	struct fred_buff *fred_buff_p;

	list_for_each_safe(cursor, tmp, &fred_buffctl_p->buffs_head) {

		fred_buff_p = list_entry(cursor, struct fred_buff, node);
		if (dereg_buff_dev_(fred_buff_p))
			return -1;

		list_del(cursor);
		kfree(fred_buff_p);
	}

	return 0;
}

static int buffctl_open(struct inode *inode_p, struct file *file_p)
{
	struct miscdevice *misc_dev;
	struct fred_buffctl *fred_buffctl_p;

	misc_dev = file_p->private_data;
	fred_buffctl_p = container_of(misc_dev, struct fred_buffctl, m_dev);

	/* Only one user at a time */
	if (test_and_set_bit_lock(STATE_BUSY, &fred_buffctl_p->state)) {
		pr_info("Fred_BuffCtl: device already open!\n");
		return -EBUSY;
	}

	pr_info("Fred_BuffCtl: buffctl opened\n");

	return 0;
}

static int buffctl_release(struct inode *ino, struct file *file_p)
{
	struct miscdevice *misc_dev;
	struct fred_buffctl *fred_buffctl_p;

	misc_dev = file_p->private_data;
	fred_buffctl_p = container_of(misc_dev, struct fred_buffctl, m_dev);

	/* Release the device */
	clear_bit_unlock(STATE_BUSY, &fred_buffctl_p->state);

	pr_info("Fred_BuffCtl: buffctl released\n");

	return 0;
}

static long buffctl_ioctl(struct file *file_p, unsigned int cmd, unsigned long arg)
{
	int retval;
	struct miscdevice *misc_dev;
	struct fred_buffctl *fred_buffctl_p;
	struct fred_buff *fred_buff_p;
	uint32_t buff_usr_id;

	misc_dev = file_p->private_data;
	fred_buffctl_p = container_of(misc_dev, struct fred_buffctl, m_dev);

	/* Commands case */
	switch (cmd) {

	/* Buffer allocation request */
	case FRED_BUFFCTL_ALLOC:

		/* Allocate a new fred buffer */
		fred_buff_p = kzalloc(sizeof(*fred_buff_p), GFP_KERNEL);
		if (!fred_buff_p) {
			retval = -ENOMEM;
			goto out;
		}

		/* Copy from user to get the size, TODO: improve... */
		retval = copy_from_user(&fred_buff_p->buff_if, (const void *)arg,
					sizeof(fred_buff_p->buff_if));
		if (retval) {
			pr_err("Fred_BuffCtl: copy from user failed\n");
			kfree(fred_buff_p);
			goto out;
		}

		pr_info("Fred_BuffCtl: allocation request, size:%zu\n",
			fred_buff_p->buff_if.length);

		/* Allocate the memory buffer using the DMA framework */
		retval = reg_new_buff_(fred_buffctl_p, fred_buff_p);
		if (retval) {
			pr_err("Fred_BuffCtl: cannot allocate a new buffer\n");
			goto out;
		}

		/* Copy back to the user */
		retval = copy_to_user((void *)arg, &fred_buff_p->buff_if,
					sizeof(fred_buff_p->buff_if));
		if (retval) {
			free_buff_(fred_buffctl_p, fred_buff_p->buff_if.id);
			goto out;
		}

		retval = 0;
		break;

	/* Buffer free request */
	case FRED_BUFFCTL_FREE:

		/* Get buffer number */
		retval = get_user(buff_usr_id, (uint32_t __user *)arg);
		if (retval)
			goto out;

		pr_info("Fred_BuffCtl: free request, id:%u\n", buff_usr_id);

		retval = free_buff_(fred_buffctl_p, buff_usr_id);
		if (retval) {
			retval = -EINVAL;
			goto out;
		}

		retval = 0;
		break;

	default:
		retval = -EINVAL;
		break;
	}

 out:
	return retval;
}

/*-------------------------------------------------------------------------------------*/

static const char buffctl_dev_name[] = "fred!buffctl";

static const struct file_operations buffctl_file_ops = {
	.owner		= THIS_MODULE,
	.open		= buffctl_open,
	.release	= buffctl_release,
	.unlocked_ioctl	= buffctl_ioctl
};

/*-------------------------------------------------------------------------------------*/

static int __init fred_buffctl_init(void)
{
	int retval;

	fred_buffctl_lp = kzalloc(sizeof(*fred_buffctl_lp), GFP_KERNEL);
	if (!fred_buffctl_lp) {
		retval = -ENOMEM;
		goto out;
	}

	INIT_LIST_HEAD(&fred_buffctl_lp->buffs_head);

	/* Init buffer control character device */
	fred_buffctl_lp->m_dev.minor = MISC_DYNAMIC_MINOR;
	fred_buffctl_lp->m_dev.fops = &buffctl_file_ops;
	fred_buffctl_lp->m_dev.name = buffctl_dev_name;

	retval = misc_register(&fred_buffctl_lp->m_dev);
	if (retval) {
		pr_err("Fred_BuffCtl: cannot register buffctl misc dev\n");
		goto err_misc_reg;
	}

	pr_info("Fred_BuffCtl: buffctl device initialized\n");
	return 0;

err_misc_reg:
	kfree(fred_buffctl_lp);
out:
	return retval;
}

static void __exit fred_buffctl_exit(void)
{
	free_all_buffs_(fred_buffctl_lp);

	misc_deregister(&fred_buffctl_lp->m_dev);

	kfree(fred_buffctl_lp);

	pr_info("Fred_BuffCtl: buffers control module removed\n");
}

module_init(fred_buffctl_init);
module_exit(fred_buffctl_exit);
