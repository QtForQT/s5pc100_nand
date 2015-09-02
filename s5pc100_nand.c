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
#if 0
/**
 * struct nand_chip - NAND Private Flash Chip Data
 * @IO_ADDR_R:		[BOARDSPECIFIC] address to read the 8 I/O lines of the
 *			flash device
 * @IO_ADDR_W:		[BOARDSPECIFIC] address to write the 8 I/O lines of the
 *			flash device.
 * @read_byte:		[REPLACEABLE] read one byte from the chip
 * @read_word:		[REPLACEABLE] read one word from the chip
 * @write_buf:		[REPLACEABLE] write data from the buffer to the chip
 * @read_buf:		[REPLACEABLE] read data from the chip into the buffer
 * @verify_buf:		[REPLACEABLE] verify buffer contents against the chip
 *			data.
 * @select_chip:	[REPLACEABLE] select chip nr
 * @block_bad:		[REPLACEABLE] check, if the block is bad
 * @block_markbad:	[REPLACEABLE] mark the block bad
 * @cmd_ctrl:		[BOARDSPECIFIC] hardwarespecific function for controlling
 *			ALE/CLE/nCE. Also used to write command and address
 * @init_size:		[BOARDSPECIFIC] hardwarespecific function for setting
 *			mtd->oobsize, mtd->writesize and so on.
 *			@id_data contains the 8 bytes values of NAND_CMD_READID.
 *			Return with the bus width.
 * @dev_ready:		[BOARDSPECIFIC] hardwarespecific function for accessing
 *			device ready/busy line. If set to NULL no access to
 *			ready/busy is available and the ready/busy information
 *			is read from the chip status register.
 * @cmdfunc:		[REPLACEABLE] hardwarespecific function for writing
 *			commands to the chip.
 * @waitfunc:		[REPLACEABLE] hardwarespecific function for wait on
 *			ready.
 * @ecc:		[BOARDSPECIFIC] ECC control structure
 * @buffers:		buffer structure for read/write
 * @hwcontrol:		platform-specific hardware control structure
 * @erase_cmd:		[INTERN] erase command write function, selectable due
 *			to AND support.
 * @scan_bbt:		[REPLACEABLE] function to scan bad block table
 * @chip_delay:		[BOARDSPECIFIC] chip dependent delay for transferring
 *			data from array to read regs (tR).
 * @state:		[INTERN] the current state of the NAND device
 * @oob_poi:		"poison value buffer," used for laying out OOB data
 *			before writing
 * @page_shift:		[INTERN] number of address bits in a page (column
 *			address bits).
 * @phys_erase_shift:	[INTERN] number of address bits in a physical eraseblock
 * @bbt_erase_shift:	[INTERN] number of address bits in a bbt entry
 * @chip_shift:		[INTERN] number of address bits in one chip
 * @options:		[BOARDSPECIFIC] various chip options. They can partly
 *			be set to inform nand_scan about special functionality.
 *			See the defines for further explanation.
 * @bbt_options:	[INTERN] bad block specific options. All options used
 *			here must come from bbm.h. By default, these options
 *			will be copied to the appropriate nand_bbt_descr's.
 * @badblockpos:	[INTERN] position of the bad block marker in the oob
 *			area.
 * @badblockbits:	[INTERN] minimum number of set bits in a good block's
 *			bad block marker position; i.e., BBM == 11110111b is
 *			not bad when badblockbits == 7
 * @cellinfo:		[INTERN] MLC/multichip data from chip ident
 * @numchips:		[INTERN] number of physical chips
 * @chipsize:		[INTERN] the size of one chip for multichip arrays
 * @pagemask:		[INTERN] page number mask = number of (pages / chip) - 1
 * @pagebuf:		[INTERN] holds the pagenumber which is currently in
 *			data_buf.
 * @subpagesize:	[INTERN] holds the subpagesize
 * @onfi_version:	[INTERN] holds the chip ONFI version (BCD encoded),
 *			non 0 if ONFI supported.
 * @onfi_params:	[INTERN] holds the ONFI page parameter when ONFI is
 *			supported, 0 otherwise.
 * @ecclayout:		[REPLACEABLE] the default ECC placement scheme
 * @bbt:		[INTERN] bad block table pointer
 * @bbt_td:		[REPLACEABLE] bad block table descriptor for flash
 *			lookup.
 * @bbt_md:		[REPLACEABLE] bad block table mirror descriptor
 * @badblock_pattern:	[REPLACEABLE] bad block scan pattern used for initial
 *			bad block scan.
 * @controller:		[REPLACEABLE] a pointer to a hardware controller
 *			structure which is shared among multiple independent
 *			devices.
 * @priv:		[OPTIONAL] pointer to private chip data
 * @errstat:		[OPTIONAL] hardware specific function to perform
 *			additional error status checks (determine if errors are
 *			correctable).
 * @write_page:		[REPLACEABLE] High-level page write function
 */
struct nand_chip {
	void __iomem *IO_ADDR_R;
	void __iomem *IO_ADDR_W;

	uint8_t (*read_byte)(struct mtd_info *mtd);
	u16 (*read_word)(struct mtd_info *mtd);
	void (*write_buf)(struct mtd_info *mtd, const uint8_t *buf, int len);
	void (*read_buf)(struct mtd_info *mtd, uint8_t *buf, int len);
	int (*verify_buf)(struct mtd_info *mtd, const uint8_t *buf, int len);
	void (*select_chip)(struct mtd_info *mtd, int chip);
	int (*block_bad)(struct mtd_info *mtd, loff_t ofs, int getchip);
	int (*block_markbad)(struct mtd_info *mtd, loff_t ofs);
	void (*cmd_ctrl)(struct mtd_info *mtd, int dat, unsigned int ctrl);
	int (*init_size)(struct mtd_info *mtd, struct nand_chip *this,
			u8 *id_data);
	int (*dev_ready)(struct mtd_info *mtd);
	void (*cmdfunc)(struct mtd_info *mtd, unsigned command, int column,
			int page_addr);
	int(*waitfunc)(struct mtd_info *mtd, struct nand_chip *this);
	void (*erase_cmd)(struct mtd_info *mtd, int page);
	int (*scan_bbt)(struct mtd_info *mtd);
	int (*errstat)(struct mtd_info *mtd, struct nand_chip *this, int state,
			int status, int page);
	int (*write_page)(struct mtd_info *mtd, struct nand_chip *chip,
			const uint8_t *buf, int page, int cached, int raw);

	int chip_delay;
	unsigned int options;
	unsigned int bbt_options;

	int page_shift;
	int phys_erase_shift;
	int bbt_erase_shift;
	int chip_shift;
	int numchips;
	uint64_t chipsize;
	int pagemask;
	int pagebuf;
	int subpagesize;
	uint8_t cellinfo;
	int badblockpos;
	int badblockbits;

	int onfi_version;
	struct nand_onfi_params	onfi_params;

	flstate_t state;

	uint8_t *oob_poi;
	struct nand_hw_control *controller;
	struct nand_ecclayout *ecclayout;

	struct nand_ecc_ctrl ecc;
	struct nand_buffers *buffers;
	struct nand_hw_control hwcontrol;

	uint8_t *bbt;
	struct nand_bbt_descr *bbt_td;
	struct nand_bbt_descr *bbt_md;

	struct nand_bbt_descr *badblock_pattern;

	void *priv;
};
#endif
#if 1
//uint8_t s5pc100_chip_read_byte(struct mtd_info *mtd);
//u16 s5pc100_chip_read_word(struct mtd_info *mtd);

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
//int vs5pc100_chip_erify_buf(struct mtd_info *mtd, const uint8_t *buf, int len);
//void s5pc100_chip_select_chip(struct mtd_info *mtd, int chip);
//int s5pc100_chip_block_bad(struct mtd_info *mtd, loff_t ofs, int getchip);
//int s5pc100_chip_block_markbad(struct mtd_info *mtd, loff_t ofs);
//void s5pc100_chip_cmd_ctrl(struct mtd_info *mtd, int dat, unsigned int ctrl);
//int s5pc100_chip_init_size(struct mtd_info *mtd, struct nand_chip *this,
//		u8 *id_data);
//int s5pc100_chip_dev_ready(struct mtd_info *mtd);
//void s5pc100_chip_cmdfunc(struct mtd_info *mtd, unsigned command, int column,
//		int page_addr);
//int s5pc100_chip_waitfunc(struct mtd_info *mtd, struct nand_chip *this);
//void s5pc100_chip_erase_cmd(struct mtd_info *mtd, int page);
//int s5pc100_chip_scan_bbt(struct mtd_info *mtd);
//int s5pc100_chip_errstat(struct mtd_info *mtd, struct nand_chip *this, int state,
//		int status, int page);
//int s5pc100_chip_write_page(struct mtd_info *mtd, struct nand_chip *chip,
//			const uint8_t *buf, int page, int cached, int raw);
#endif
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



static void hw_init_nand(struct s5pc100_mtd_nand  *d)
{
	struct nand_chip *chip = get_chip(d);
	struct mtd_info  *info = get_info(d);
	info->priv = chip;
	chip->chip_delay   = 50;
	chip->ecc.mode = NAND_ECC_SOFT;
	chip->IO_ADDR_W = d->base + S5PC100_NFDATA;
	chip->IO_ADDR_R = d->base + S5PC100_NFDATA;
	chip->cmd_ctrl  = s5pc100_nand_hwcontrol;
	chip->dev_ready = s5pc100_nand_devready;
	chip->write_buf = s5pc100_chip_write_buf;
	chip->read_buf  = s5pc100_chip_read_buf;
}

static void enable_nand_controler(struct s5pc100_mtd_nand *d)
{
	iowrite32(S5PC100_NFCONT_ENABLE,d->base + S5PC100_NFCONT);
}

int s5pc100_nand_probe (struct platform_device *pdev)
{
#if 0
	return -1;
}
int s5pc100_nand_start(void)
{
#endif
//	struct resource *res;
//	struct resource *area;
//	resource_size_t size;
//	res  = pdev->resource;
//	size = resource_size(res);
//	area = request_mem_region(res->start,size );
//	pr_err("mask:area:(%d)\n",area);
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
	pr_err("----start--s5pc100 nand init \n");
//	s5pc100_nand_start();
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
