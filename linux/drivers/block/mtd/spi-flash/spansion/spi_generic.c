/*
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER.
 * 
 * Copyright 2009-2011 Spansion LLC. All rights reserved.
 * 
 * The contents of this file are subject to the terms of the GNU
 * General Public License Version 2 only (the "License"). 
 * You may not use this file except in compliance with the License. 
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * 
 * If applicable, add the following below the License Header, with 
 * the fields enclosed by brackets [] replaced by your own identifying 
 * information: "Portions Copyrighted [year] [name of copyright owner]"
 *
 * <http://www.spansion.com/Support/SES/Pages/ask_spansion.html>
 */

#include <linux/version.h>
#include <generated/autoconf.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/spi.h>
#include <linux/device.h>
#include <linux/spi/spi.h>

//  Generic board driver connecting the SPI chip driver to the Linux SPI
//  framework. Things like requested I/O mode or partition data go in here.
//  Might be customized and integrated into a larger board driver that 
//  supports other devices, too.


static struct spi_interface {
   struct mtd_info *mtd;             //  registered MTD obj handle (front end)
   struct spi_device *spi;           //  registered SPI obj handle (back end)
   unsigned char txbuf[4 + 256];     //  write buffer
} inter;


//  ============
//  generic_read
//  ============

static void generic_read(struct spi_flash* spiflash, unsigned char command,
                         void* to, unsigned long from, size_t len)
{
   unsigned int flags = *(unsigned int*)(spiflash->chip.priv);
   int i = 0;
   
   inter.txbuf[i++] = command;
   if (from != SPI_NO_ADDRESS) {
      if (flags & HAS_ADDR32)
         inter.txbuf[i++] = ((from >> 24) & 0xFF);
      inter.txbuf[i++] = ((from >> 16) & 0xFF);
      inter.txbuf[i++] = ((from >>  8) & 0xFF);
      inter.txbuf[i++] = ((from      ) & 0xFF);
   }
   
   switch (command) {                   //  add dummy/mode bytes
   case SPI_FAST_READ:
   case SPI_DUAL_READ:
   case SPI_QUAD_READ:
   case SPI_OTP_READ:
   case SPI_DUAL_READ_HP:
      inter.txbuf[i++] = 0;
      break;
   case SPI_QUAD_READ_HP:
   case SPI_WAKE_UP:
      inter.txbuf[i++] = 0;
      inter.txbuf[i++] = 0;
      inter.txbuf[i++] = 0;
   }
   
   spi_write_then_read( inter.spi, inter.txbuf, i, to, len);
}


//  =============
//  generic_write
//  =============

static void generic_write(struct spi_flash* spiflash, unsigned char command,
                          const void* from, unsigned long to, size_t len)
{
   unsigned int flags = *(unsigned int*)(spiflash->chip.priv);
   int i = 0;
   
   if (len > sizeof( inter.txbuf))
      { printk( "ERROR: SPI write request too large!\n"); return; }
   
   inter.txbuf[i++] = command;
   if (to != SPI_NO_ADDRESS) {
      if (flags & HAS_ADDR32)
         inter.txbuf[i++] = ((to >> 24) & 0xFF);
      inter.txbuf[i++] = ((to >> 16) & 0xFF);
      inter.txbuf[i++] = ((to >>  8) & 0xFF);
      inter.txbuf[i++] = ((to      ) & 0xFF);
   }
   
   memcpy( &inter.txbuf[i], from, len);
   i += len;
   
   spi_write( inter.spi, inter.txbuf, i);
}


//  =============
//  generic_probe
//  =============

static int generic_probe(struct spi_device *spi)
{
   struct spi_flash *spiflash;
   
   if (inter.mtd) 
      { printk( "ERROR: SPI device already initialized!\n"); return -ENODEV; }

   //  Initialize SPI struct:

   if (!(spiflash = kzalloc( sizeof(struct spi_flash), GFP_KERNEL)))
      { printk( "ERROR: Failed to allocate SPI struct!\n"); return -ENOMEM; }
   
   spiflash->io_mode = SPI_SINGLE_IO;
   spiflash->read_op = generic_read;
   spiflash->write_op = generic_write;
   inter.spi = spi;
   
   //  Probe for a valid SPI device:
   
   if ((inter.mtd = spsn_spi_probe( spiflash))) {
      inter.mtd->owner = THIS_MODULE;
      add_mtd_device( inter.mtd);
      return 0;
   }

   printk( "MTD probing of SPI flash device failed\n");
   kfree( spiflash);
   return -ENODEV;
}


//  ==============
//  generic_remove
//  ==============

static int generic_remove(struct spi_device *spi)
{
   if (inter.mtd) {
      struct spi_flash *spiflash = inter.mtd->priv;
      del_mtd_device( inter.mtd);
      spsn_spi_destroy( inter.mtd);
      if (spiflash)
         kfree( spiflash);
   }
   return 0;
}


static struct spi_driver generic_spi_driver = {
   .driver = {
      .name = "s25fl128s",
      .bus = &spi_bus_type,
      .owner = THIS_MODULE,      
   },
   .probe = generic_probe,
   .remove = generic_remove,
};


//  ===================
//  generic_init_module
//  ===================

static int __init generic_init_module(void)
{
   return spi_register_driver( &generic_spi_driver);
}

   
//  ======================
//  generic_cleanup_module
//  ======================

static void __exit generic_cleanup_module(void)
{
   spi_unregister_driver( &generic_spi_driver);
}


module_init( generic_init_module);
module_exit( generic_cleanup_module);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Spansion Inc.");
MODULE_DESCRIPTION("SPI chip driver interface to the SPI framework");
