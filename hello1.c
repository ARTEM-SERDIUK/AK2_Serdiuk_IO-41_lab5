// SPDX-License-Identifier: Dual BSD/GPL
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/errno.h>
#include "hello1.h"

MODULE_AUTHOR("Artem Serdiuk");
MODULE_DESCRIPTION("Lab 5: Module hello1.c");
MODULE_LICENSE("Dual BSD/GPL");

struct event_entry {
	struct list_head list;
	ktime_t time_before;
	ktime_t time_after;
};

static LIST_HEAD(event_list);

int print_hello(void)
{
	struct event_entry *entry;

	entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return -ENOMEM;

	entry->time_before = ktime_get();
	pr_info("Hello, world!\n");
	entry->time_after = ktime_get();

	list_add_tail(&entry->list, &event_list);

	return 0;
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1: module loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct event_entry *entry, *tmp;
	s64 duration;

	list_for_each_entry_safe(entry, tmp, &event_list, list) {
		duration = ktime_to_ns(ktime_sub(entry->time_after,
						 entry->time_before));
		pr_info("hello1: print duration = %lld ns\n",
			(long long)duration);
		list_del(&entry->list);
		kfree(entry);
	}
	pr_info("hello1: module unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
