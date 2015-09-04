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
#include <asm/io.h>
#include <linux/io.h>
#include <linux/slab.h>

#include "s5pc100_nand_reg.h"

static struct mtd_partition s5pc100_nand_partition[] = {
	[0] = {
		.name = "bootloader",
		.size = SZ_1M,
		.offset = 0,
	},
	[1] = {
		.name = "kernel",
		.offset = MTDPART_OFS_APPEND,
		.size  = SZ_1M * 3,
	},
	[2] = {
		.name = "rootfs",
		.offset = MTDPART_OFS_APPEND,
		.size = MTDPART_SIZ_FULL,
	},
};

struct s5pc100_mtd_nand {
	void __iomem *base;
	struct clk   *clk;
	struct mtd_info mtd_info;
	struct nand_chip chip;
};

static struct nand_chip *get_chip(struct s5pc100_mtd_nand *d)
{
	return &d->chip;
}
static struct mtd_info *get_info(struct s5pc100_mtd_nand *d)
{
	return &d->mtd_info;
}
static void s5pc100_chip_write_buf(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	struct nand_chip *this = mtd->priv;
	writesb(this->IO_ADDR_W, buf, len);
}
static void s5pc100_chip_read_buf(struct mtd_info *mtd, uint8_t *buf, int len)
{
	struct nand_chip *this = mtd->priv;
	readsb(this->IO_ADDR_R, buf, len);
}
static int s5pc100_nand_devready(struct mtd_info *mtd)
{

	struct s5pc100_mtd_nand *d = container_of(mtd,struct s5pc100_mtd_nand,mtd_info);
	return ioread32(d->base + S5PC100_NFSTAT) & S5PC100_NAND_STAT_BUSY;

}

static void s5pc100_nand_hwcontrol(struct mtd_info *mtd, int cmd, 
		                                   unsigned int ctrl)
{
	struct s5pc100_mtd_nand *d = container_of(mtd,struct s5pc100_mtd_nand,mtd_info);

	if (cmd == NAND_CMD_NONE)
		return;

	if (ctrl & NAND_CLE)
		writeb(cmd, d->base + S5PC100_NFCMMD);
	else 
		writeb(cmd, d->base + S5PC100_NFADDR);
}

static int s3c_nand_scan_bbt(struct mtd_info *mtdinfo)
{
	return 0;
}

static void hw_init_nand(struct s5pc100_mtd_nand  *d)
{
	struct nand_chip *chip = get_chip(d);
	struct mtd_info  *info = get_info(d);
	info->priv = chip;
	chip->chip_delay   = 50;
	#if 0
	chip->ecc.mode = NAND_ECC_SOFT;
	#else
	chip->ecc.mode = NAND_ECC_NONE;
	#endif
	chip->IO_ADDR_W = d->base + S5PC100_NFDATA;
	chip->IO_ADDR_R = d->base + S5PC100_NFDATA;
	chip->cmd_ctrl  = s5pc100_nand_hwcontrol;
	chip->dev_ready = s5pc100_nand_devready;
	chip->write_buf = s5pc100_chip_write_buf;
	chip->read_buf  = s5pc100_chip_read_buf;
	chip->scan_bbt		= s3c_nand_scan_bbt;
	chip->options		= 0;
}

static void enable_nand_controler(struct s5pc100_mtd_nand *d)
{
	iowrite32(S5PC100_NFCONT_ENABLE,d->base + S5PC100_NFCONT);
}

int s5pc100_nand_probe (struct platform_device *pdev)
{
//	struct resource *res;
//	struct resource *area;
//	resource_size_t size;
//	res  = pdev->resource;
//	size = resource_size(res);
//	area = request_mem_region(res->start,size );
//	if (area == NULL) {
//		return -ENOENT;
//	}
//
	struct s5pc100_mtd_nand *data;
	int err = 0;
	data =  kzalloc(sizeof(*data),GFP_KERNEL);
	if (data == NULL ) {
		pr_err("no memory for nand devices !!!\n");
		return -ENOMEM;
	}
#define phys_addr 0xE7200000
	data->base = ioremap(phys_addr,S5PC100_NAND_ADDR_LEN);
	if ( !data->base ) {
		pr_err("Io remap error !!!\n");
		goto fail;
	}
	platform_set_drvdata(pdev,data);
	data->clk = clk_get(&pdev->dev,"nfcon");
	if (IS_ERR(data->clk)) {
		pr_err("%s:no clk fond \n",__func__);
		err = -1;
		goto fail;
	}
	clk_enable(data->clk);

	enable_nand_controler(data);
	hw_init_nand(data);
	if (nand_scan(&data->mtd_info, 1)) {
		err = -ENXIO;
		goto no_dev;
	}

	data->mtd_info.name = "s5pc_nand";
	err = mtd_device_parse_register(&data->mtd_info, NULL, NULL,s5pc100_nand_partition,ARRAY_SIZE(s5pc100_nand_partition));
	if (err) {
		nand_release(&data->mtd_info);
		goto no_dev;
	}
	pr_err("------------success---------\n");
	return 0;
fail:
	return -1;
no_dev:
	return err;
}

int s5pc100_nand_remove(struct platform_device *dev)
{
	return 0;
}

#ifdef CONFIG_OF
const struct of_device_id s5pc100_nand_id_table[] = {
	{.compatible = "s5pc100-nand-control"},
};
#else
#define s5pc100_nand_id_table NULL
#endif
struct platform_driver s5pc100_nand_driver = {
	.probe = s5pc100_nand_probe ,
	.remove = s5pc100_nand_remove ,
	.driver = {
		.name = "s5pc100-nand",
		.of_match_table = s5pc100_nand_id_table,
	},
};

static int  __init s5pc100_nand_init(void)
{
	return platform_driver_register(&s5pc100_nand_driver);
}

static void __exit s5pc100_nand_exit(void)
{
	return platform_driver_unregister(&s5pc100_nand_driver);
}

module_init(s5pc100_nand_init);
module_exit(s5pc100_nand_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("mask.yang");
