// SPDX-License-Identifier: Dual BSD/GPL
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/errno.h>
#include "hello1.h"

MODULE_AUTHOR("Artem Serdiuk");
MODULE_DESCRIPTION("Lab 5: Module hello2.c");
MODULE_LICENSE("Dual BSD/GPL");

static unsigned int count = 1;
module_param(count, uint, 0644);
MODULE_PARM_DESC(count, "Number of prints");

static int __init hello2_init(void)
{
	unsigned int i;
	int ret;

	if (count > 10) {
		pr_err("hello2: parameter count is too large: %u > 10\n",
		       count);
		return -EINVAL;
	}

	if (count == 0 || (count >= 5 && count <= 10))
		pr_warn("hello2: warning, unusual count value: %u\n", count);

	for (i = 0; i < count; i++) {
		ret = print_hello();
		if (ret)
			return ret;
	}

	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2: module unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
