#include <linux/module.h>

#include <linux/types.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/err.h>
#include <linux/kernel.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/nand_ecc.h>
#include <linux/mtd/partitions.h>

struct s5pc100_mtd_nand {
	void __iomem *iomem;
	struct mtd_info mtd_info;
	struct nand_chip nand_chip;
};

int s5pc100_nand_probe (struct platform_device *pdev)
{
	struct s5pc100_mtd_nand *data;
	struct resource *res;
	struct resource *area;
	resource_size_t size;
	int err = 0;
	data =  kzalloc(sizeof(*data),GFP_KERNEL);
	if (data == NULL ) {
		pr_err("no memory for nand devices !!!\n");
		return -ENOMEM;
	}
	platform_set_drvdata(pdev,data);

	res  = pdev->resource;
	size = resource_size(res);

	area = request_mem_region(res->start,size );
	pr_err("mask:area:(%d)\n",area);
	if (area == NULL) {
		return -ENOENT;
	}

}

int s5pc100_nand_remove(struct platform_device *dev)
{
}

#ifdef CONFIG_OF
const struct of_device_id s5pc100_nand_id_table[] = {
	{.compatible = "s5pc100-nand-control"},
};
#endif
struct platform_driver s5pc100_nand_driver = {
	.probe = s5pc100_nand_probe ,
	.remove = s5pc100_nand_remove ,
	.driver = {
		.name = "s5pc100-nand",
		.of_match_table = s5pc100_nand_id_table,
	},
};

static __init s5pc100_nand_init(void)
{
	return platform_driver_register(&s5pc100_nand_driver);
}

static __exit s5pc100_nand_exit(void)
{
	return platform_driver_unregister(&s5pc100_nand_driver);
}

module_init(s5pc100_nand_init);
module_exit(s5pc100_nand_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("mask.yang");
