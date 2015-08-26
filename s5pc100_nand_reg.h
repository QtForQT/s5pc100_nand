#ifndef __S5PC100_NAND_REG

#define __S5PC100_NAND_REG


#define S5PC100_NFREG(x) (x)

#define S5PC100_NFCONF S5PC100_NFREG(0x00)
#define S5PC100_NFCONT  S5PC100_NFREG(0x04)
#define S5PC100_NFCMMD S5PC100_NFREG(0x08)
#define S5PC100_NFADDR S5PC100_NFREG(0x0c)
#define S5PC100_NFDATA S5PC100_NFREG(0x10)
#define S5PC100_NFMECCD0 S5PC100_NFREG(0x14)
#define S5PC100_NFMECCD1 S5PC100_NFREG(0x18)
#define S5PC100_NFSECCD S5PC100_NFREG(0x1c)
#define S5PC100_NFSBLK S5PC100_NFREG(0x20)
#define S5PC100_NFEBLK S5PC100_NFREG(0x24)
#define S5PC100_NFSTAT S5PC100_NFREG(0x28)
#define S5PC100_NFECCERR0 S5PC100_NFREG(0x2c)
#define S5PC100_NFECCERR1 S5PC100_NFREG(0x30)
#define S5PC100_NFMECC0 S5PC100_NFREG(0x34)
#define S5PC100_NFMECC1 S5PC100_NFREG(0x38)
#define S5PC100_NFSECC S5PC100_NFREG(0x3c)
#define S5PC100_NFMLCBITPT S5PC100_NFREG(0x40)
#define S5PC100_NF8ECCERR0 S5PC100_NFREG(0x44)
#define S5PC100_NF8ECCERR1 S5PC100_NFREG(0x48)
#define S5PC100_NF8ECCERR2 S5PC100_NFREG(0x4c)
#define S5PC100_NFM8ECC0 S5PC100_NFREG(0x50)
#define S5PC100_NFM8ECC1 S5PC100_NFREG(0x54)
#define S5PC100_NFM8ECC2 S5PC100_NFREG(0x58)
#define S5PC100_NFM8ECC3 S5PC100_NFREG(0x5c)
#define S5PC100_NFMLC8BITPT0 S5PC100_NFREG(0x60)
#define S5PC100_NFMLC8BITPT1 S5PC100_NFREG(0x64)
#define S5PC100_NFACTADJ S5PC100_NFREG(0x68)

#endif


