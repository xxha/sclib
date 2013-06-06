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

//  MTD chip driver for the Spansion S25FL-S, S25FL-P and S25FL-K
//  SPI flash memory families

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/types.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/cfi.h>
#include <linux/mtd/spi.h>
#include <linux/slab.h>

#define ERASE_TIMEOUT            5  // s
#define PROGRAM_TIMEOUT          1  // s
#define ERASE_SUSPEND_LATENCY   40  // us
#define ERASE_RESUME_LATENCY   100  // us

//  Device database:
//  ================
//
//  Note that not all devices have CFI and some parameters cannot be retrieved
//  via CFI. Therefore, we use a hardcoded device database for now.

struct device_info {
   char *name;                 //  device OPN
   int id[5];                  //  ID (bytes 4/5 are don't care if set to -1)
   unsigned int numblocks[2];  //  number of erase blocks (per region)
   unsigned int erasesize[2];  //  sector size (per region)
   unsigned int pagesize;      //  write page size
   unsigned int flags;         //  device flags (see spi.h)
};


static struct device_info spsn_devinfo[] = {
   { "S25FL032P", 
     { 0x01, 0x02, 0x15, 0x4d, -1 }, { 32, 62 }, { 0x1000, 0x10000 }, 256,
     HAS_MULTI_IO | HAS_STATBIT5_ERROR | HAS_STATBIT6_ERROR | HAS_OTP |
     HAS_CONFREG | HAS_TBPARM },
   { "S25FL064P", 
     { 0x01, 0x02, 0x16, 0x4d, -1 }, { 32, 126 }, { 0x1000, 0x10000 }, 256,
     HAS_MULTI_IO | HAS_STATBIT5_ERROR | HAS_STATBIT6_ERROR | HAS_OTP |
     HAS_CONFREG | HAS_TBPARM },
   { "S25FL129P-00", 
     { 0x01, 0x20, 0x18, 0x4d, 0x01 }, { 32, 254 }, { 0x1000, 0x10000 }, 256,
     HAS_MULTI_IO | HAS_STATBIT5_ERROR | HAS_STATBIT6_ERROR | HAS_OTP |
     HAS_CONFREG | HAS_TBPARM},
   { "S25FL129P-01", 
     { 0x01, 0x20, 0x18, 0x4d, 0x00 }, { 64, 0 }, { 0x40000, 0 },  256,
     HAS_MULTI_IO | HAS_STATBIT5_ERROR | HAS_STATBIT6_ERROR | HAS_OTP |
     HAS_CONFREG },
   { "S25FL128P-00", 
     { 0x01, 0x20, 0x18, 0x03, 0x01 }, { 256, 0 }, { 0x10000, 0 }, 256, 0 },
   { "S25FL128P-01", 
     { 0x01, 0x20, 0x18, 0x03, 0x00 }, {  64, 0 }, { 0x40000, 0 }, 256, 0 },
   { "S25FL256S-00", 
     { 0x01, 0x02, 0x19, 0x4d, 0x01 }, { 32, 510 }, { 0x1000, 0x10000 }, 256,
     HAS_MULTI_IO | HAS_STATBIT5_ERROR | HAS_STATBIT6_ERROR | HAS_CONFREG |
     HAS_TBPARM | HAS_ADDR32 | HAS_ERASESUSPEND},
   { "S25FL256S-01", 
     { 0x01, 0x02, 0x19, 0x4d, 0x00 }, { 128, 0 }, { 0x40000, 0 }, 512,
     HAS_MULTI_IO | HAS_STATBIT5_ERROR | HAS_STATBIT6_ERROR | HAS_CONFREG |
     HAS_ADDR32 | HAS_ERASESUSPEND},
   { "S25FL016K", 
     { 0xef, 0x40, 0x15, -1, -1 }, {  512, 0 }, { 0x1000, 0 }, 256,
     HAS_MULTI_IO | HAS_CONFREG },
   { "S25FL064K", 
     { 0xef, 0x40, 0x17, -1, -1 }, { 2048, 0 }, { 0x1000, 0 }, 256,
     HAS_MULTI_IO | HAS_CONFREG }
};



//  ============
//  device_ready
//  ============

static int device_ready(struct spi_flash *spiflash, unsigned char* status)
{
   spiflash->read_op( spiflash, SPI_READ_STATUS, status, SPI_NO_ADDRESS, 1);
   return !(*status & 0x01);
}


//  ===========
//  device_good
//  ===========

static int device_good(struct spi_flash *spiflash, unsigned char status)
{
   if (((*(int*)(spiflash->chip.priv) & HAS_STATBIT5_ERROR) && 
        (status & 0x20)) ||
       ((*(int*)(spiflash->chip.priv) & HAS_STATBIT6_ERROR) && 
        (status & 0x40))) {
      spiflash->write_op( spiflash, SPI_CLEAR_STATUS, NULL, SPI_NO_ADDRESS, 0);
      return 0;
   }
   return !(status & 0x01);
}


//  ========
//  get_chip
//  ========

static int get_chip(struct spi_flash *spiflash, int mode)
{
   struct flchip *chip = &spiflash->chip;
   unsigned int flags = *(unsigned int*)(chip->priv);
   DECLARE_WAITQUEUE( wait, current);
   
 retry:
   switch (chip->state) {
   case FL_READY:
      return 0;
   case FL_ERASING:       //  suspend the erase to service read requests
      if ((flags & HAS_ERASESUSPEND) && (mode == FL_READING)) {
         spiflash->write_op( spiflash, SPI_ERASE_SUSPEND, NULL, SPI_NO_ADDRESS,
                             0);
         chip->oldstate = FL_ERASING;
         chip->state = FL_ERASE_SUSPENDING;
         chip->erase_suspended = 1;
         mutex_unlock(&chip->mutex);
         cfi_udelay( ERASE_SUSPEND_LATENCY);
         mutex_lock( &chip->mutex);
         chip->state = FL_READY;
         return 0;
      }
   default:               //  wait until we get notified
      set_current_state( TASK_UNINTERRUPTIBLE);
      add_wait_queue( &chip->wq, &wait);
      mutex_unlock( &chip->mutex);
      schedule();
      remove_wait_queue( &chip->wq, &wait);
      mutex_lock( &chip->mutex);
      goto retry;
   }
   return 0;
}


//  ========
//  put_chip
//  ========

static void put_chip(struct spi_flash *spiflash)
{
   struct flchip *chip = &spiflash->chip;
   unsigned int flags = *(unsigned int*)(chip->priv);
   
   if ((flags & HAS_ERASESUSPEND) && (chip->oldstate == FL_ERASING)) {
      spiflash->write_op( spiflash, SPI_ERASE_RESUME, NULL, SPI_NO_ADDRESS, 0);
      chip->state = FL_ERASING;
      chip->oldstate = FL_READY;
      mutex_unlock( &chip->mutex);
      cfi_udelay( ERASE_RESUME_LATENCY);
      mutex_lock( &chip->mutex);
   }
   wake_up( &chip->wq);
}


//  =================
//  do_erase_oneblock
//  =================

static int do_erase_oneblock(struct spi_flash *spiflash, unsigned long adr,
                             int param4k)
{
   struct flchip *chip = &spiflash->chip;
   unsigned long timeo;
   unsigned char status;
   int ret = 0;
   DECLARE_WAITQUEUE( wait, current);
   
   mutex_lock( &chip->mutex);
   ret = get_chip( spiflash, FL_ERASING);
   if (ret) {
      mutex_unlock( &chip->mutex);
      return ret;
   }
   chip->state = FL_ERASING;
   chip->erase_suspended = 0;
   chip->in_progress_block_addr = adr;
   mutex_unlock( &chip->mutex);

   spiflash->write_op( spiflash, SPI_WRITE_ENABLE, NULL, SPI_NO_ADDRESS, 0);
   spiflash->write_op( spiflash, (param4k ? SPI_ERASE_4K : SPI_ERASE), NULL,
                       adr, 0);
   
   timeo = jiffies + ERASE_TIMEOUT * HZ;
   
   for (;;) {
      if (chip->state != FL_ERASING) {
         
         //  Someone has suspended the erase, sleep until resumed:
         
         set_current_state( TASK_UNINTERRUPTIBLE);
         add_wait_queue( &chip->wq, &wait);
         mutex_unlock( &chip->mutex);
         schedule();
         remove_wait_queue( &chip->wq, &wait);
         mutex_lock( &chip->mutex);
         continue;
      }
      if (chip->erase_suspended) {
         
         //  The erase was suspended and resumed, reset the timeout:
         
         timeo = jiffies + ERASE_TIMEOUT * HZ;
         chip->erase_suspended = 0;
      }
      
      if (device_ready( spiflash, &status)) {
         break;
      }
      if (time_after( jiffies, timeo)) {
         printk( KERN_WARNING "MTD %s(): software timeout\n", __func__ );
         break;
      }
      cfi_udelay( 1000);  //  drop the lock, wait 1 ms and try again
   }
   if (!device_good( spiflash, status))
      ret = -EIO;
   
   mutex_lock( &chip->mutex);
   chip->state = FL_READY;
   put_chip( spiflash);
   mutex_unlock( &chip->mutex);
   return ret;
}


//  ==============
//  spsn_spi_erase
//  ==============

static int spsn_spi_erase(struct mtd_info *mtd, struct erase_info *instr)
{
   struct spi_flash *spiflash = (struct spi_flash *)(mtd->priv);
   struct mtd_erase_region_info *regions = mtd->eraseregions;
   unsigned long ofs, len;
   int i = 0, first, ret;

   ofs = instr->addr;
   len = instr->len;
   
   if (ofs > mtd->size || (len + ofs) > mtd->size)
      return -EINVAL;
   
   //  Check that both start and end of the requested erase are aligned
   //  with the erasesize of the appropriate regions:
   
   while (i < mtd->numeraseregions && ofs >= regions[i].offset)
      i++;
   i--;
   if (ofs & (regions[i].erasesize - 1))
      return -EINVAL;
   first = i;
   
   while (i < mtd->numeraseregions && (ofs + len) >= regions[i].offset)
      i++;
   i--;
   if ((ofs + len) & (regions[i].erasesize-1))
      return -EINVAL;
   i=first;
   
   //  Loop over all sectors that need to be erased:
   
   while (len) {
      int size = regions[i].erasesize;

      ret = do_erase_oneblock( spiflash, ofs, (size == 4096));
      if (ret)
         return ret;

      ofs += size;
      len -= size;
      
      if (ofs == regions[i].offset + size * regions[i].numblocks)
         i++;
   }
   
   instr->state = MTD_ERASE_DONE;
   mtd_erase_callback(instr);
   
   return 0;
}


//  ===============
//  do_write_buffer
//  ===============

static int do_write_buffer(struct spi_flash *spiflash, unsigned long adr, 
                           const u_char *buf, int len)
{
   struct flchip *chip = &spiflash->chip;
   unsigned long timeo;
   unsigned char status, cmd;
   int ret;
   
   mutex_lock( &chip->mutex);
   ret = get_chip( spiflash, FL_WRITING);
   if (ret) {
      mutex_unlock( &chip->mutex);
      return ret;
   }
   chip->state = FL_WRITING;
   mutex_unlock( &chip->mutex);
   
   switch (spiflash->io_mode) {
   case SPI_QUAD_IO_HP:
   case SPI_QUAD_IO:
      cmd = SPI_QUAD_PROGRAM;
      break;
   case SPI_QUAD_I:
   case SPI_DUAL_IO_HP:
   case SPI_DUAL_IO:
   case SPI_DUAL_I:
   case SPI_SINGLE_IO_FASTREAD:
   case SPI_SINGLE_IO:
      cmd = SPI_PROGRAM;
      break;
   default:
      printk( "ERROR: unsupported SPI I/O mode!\n");
      ret = -EIO;
      goto out;
   }

   spiflash->write_op( spiflash, SPI_WRITE_ENABLE, NULL, SPI_NO_ADDRESS, 0);
   spiflash->write_op( spiflash, cmd, buf, adr, len);

   timeo = jiffies + PROGRAM_TIMEOUT * HZ;

   for (;;) {
      if (device_ready( spiflash, &status)) {
         break;
      }
      if (time_after( jiffies, timeo)) {
         printk( KERN_WARNING "MTD %s(): software timeout\n", __func__ );
         break;
      }
      cfi_udelay( 10);  //  drop the lock, wait 10 us and try again
   }
   if (!device_good( spiflash, status))
      ret = -EIO;

 out:
   mutex_lock( &chip->mutex);
   chip->state = FL_READY;
   put_chip( spiflash);
   mutex_unlock( &chip->mutex);
   
   return ret;
}


//  ==============
//  spsn_spi_write
//  ==============

static int spsn_spi_write(struct mtd_info *mtd, loff_t to, size_t len, 
                          size_t *retlen, const u_char *buf)
{
   struct spi_flash *spiflash = (struct spi_flash *)(mtd->priv);
   int ret = 0;

   *retlen = 0;
   
   while (len) {          //  we must not cross write page boundaries
      int size = spiflash->pagesize - (to & (spiflash->pagesize - 1));
      
      if (spiflash->max_data_len && size > spiflash->max_data_len)
         size = spiflash->max_data_len;
   
      if (size > len)
         size = len;
      
      ret = do_write_buffer( spiflash, to, buf, size);
      if (ret)
         return ret;
      
      to += size;
      buf += size;
      (*retlen) += size;
      len -= size;
   }
   return ret;
}


//  ==============
//  spsn_spi_read
//  ==============

static int spsn_spi_read(struct mtd_info *mtd, loff_t from, size_t len, 
                         size_t *retlen, u_char *buf)
{
   struct spi_flash *spiflash = (struct spi_flash *)(mtd->priv);
   struct flchip *chip = &spiflash->chip;
   unsigned char cmd;
   int ret = 0;
   
   mutex_lock( &chip->mutex);
   ret = get_chip( spiflash, FL_READING);
   if (ret) {
      mutex_unlock( &chip->mutex);
      return ret;
   }
   chip->state = FL_READING;
   mutex_unlock( &chip->mutex);

   switch (spiflash->io_mode) {
   case SPI_QUAD_IO_HP:
      cmd = SPI_QUAD_READ_HP;
      break;
   case SPI_QUAD_I:
   case SPI_QUAD_IO:
      cmd = SPI_QUAD_READ;
      break;
   case SPI_DUAL_IO_HP:
      cmd = SPI_DUAL_READ_HP;
      break;
   case SPI_DUAL_I:
   case SPI_DUAL_IO:
      cmd = SPI_DUAL_READ;
      break;
   case SPI_SINGLE_IO_FASTREAD:
      cmd = SPI_FAST_READ;
      break;
   case SPI_SINGLE_IO:
      cmd = SPI_READ;
      break;
   default:
      printk( "ERROR: unsupported SPI I/O mode!\n");
      ret = -EIO;
      goto out;
   }

   *retlen = 0;
   
   while (len) {
      int size = len;
      if (spiflash->max_data_len && size > spiflash->max_data_len)
         size = spiflash->max_data_len;
      
      spiflash->read_op( spiflash, cmd, buf, from, size);
      
      from += size;
      buf += size;
      (*retlen) += size;
      len -= size;
   }

 out:
   mutex_lock( &chip->mutex);
   chip->state = FL_READY;
   put_chip( spiflash);
   mutex_unlock( &chip->mutex);
      
   return ret;
}


//  =============
//  spsn_spi_sync
//  =============

static void spsn_spi_sync(struct mtd_info *mtd)
{
   struct spi_flash *spiflash = (struct spi_flash *)(mtd->priv);
   struct flchip *chip = &spiflash->chip;
   DECLARE_WAITQUEUE( wait, current);

 retry:
   mutex_lock(&chip->mutex);
   
   if (chip->state != FL_READY) {
      add_wait_queue( &chip->wq, &wait);        //  wait until op done
      mutex_unlock( &chip->mutex);
      schedule();
      remove_wait_queue( &chip->wq, &wait);
      goto retry;
   }

   mutex_unlock( &chip->mutex);
}


//  ================
//  spsn_spi_suspend
//  ================

static int spsn_spi_suspend(struct mtd_info *mtd)
{
   struct spi_flash *spiflash = (struct spi_flash *)(mtd->priv);
   struct flchip *chip = &spiflash->chip;
   int ret = 0;
   
   mutex_lock( &chip->mutex);
   
   switch (chip->state) {
   case FL_READY:
      chip->state = FL_PM_SUSPENDED;
   case FL_PM_SUSPENDED:
      break;
   default:
      ret = -EAGAIN;
   }
   
   mutex_unlock( &chip->mutex);
   
   return ret;
}


//  ===============
//  spsn_spi_resume
//  ===============

static void spsn_spi_resume(struct mtd_info *mtd)
{
   struct spi_flash *spiflash = (struct spi_flash *)(mtd->priv);
   struct flchip *chip = &spiflash->chip;

   mutex_lock( &chip->mutex);
   
   if (chip->state == FL_PM_SUSPENDED) {
      chip->state = FL_READY;
      wake_up( &chip->wq);
   }
   else
      printk( "ERROR: Chip not in FL_PM_SUSPENDED state upon resume!\n");
   
   mutex_unlock( &chip->mutex);
}


//  ===========
//  set_confreg
//  ===========

static int set_confreg(struct spi_flash *spiflash, unsigned char conf)
{
   unsigned long timeo;
   unsigned char status, regs[2];
   int ret = 0;

   //  no locking needed since this is called by the probing function only
   
   spiflash->read_op( spiflash, SPI_READ_STATUS, &regs[0], SPI_NO_ADDRESS, 1);
   regs[1] = conf;
   spiflash->write_op( spiflash, SPI_WRITE_ENABLE, NULL, SPI_NO_ADDRESS, 0);
   spiflash->write_op( spiflash, SPI_WRITE_REG, regs, SPI_NO_ADDRESS, 
                       sizeof (regs));
   
   timeo = jiffies + PROGRAM_TIMEOUT * HZ;

   for (;;) {
      if (device_ready( spiflash, &status)) {
         break;
      }
      if (time_after( jiffies, timeo)) {
         printk( KERN_WARNING "MTD %s(): software timeout\n", __func__ );
         break;
      }
      cfi_udelay( 10);  //  drop the lock, wait 10 us and try again
   }
   if (!device_good( spiflash, status))
      ret = -EIO;

   return ret;
}


//  =================
//  spsn_spi_otp_read
//  =================


static int spsn_spi_otp_read (struct mtd_info *mtd, loff_t from, size_t len,
                              size_t *retlen, u_char *buf)
{
   struct spi_flash *spiflash = (struct spi_flash *)(mtd->priv);
   struct flchip *chip = &spiflash->chip;
   int ret = 0;
   
   mutex_lock( &chip->mutex);
   ret = get_chip( spiflash, FL_OTP_READ);
   if (ret) {
      mutex_unlock( &chip->mutex);
      return ret;
   }
   chip->state = FL_OTP_READ;
   mutex_unlock( &chip->mutex);

   while (len) {
      int size = len;
      if (spiflash->max_data_len && size > spiflash->max_data_len)
         size = spiflash->max_data_len;
      
      spiflash->read_op( spiflash, SPI_OTP_READ, buf, from, size);
      
      from += size;
      buf += size;
      (*retlen) += size;
      len -= size;
   }
   
   mutex_lock( &chip->mutex);
   chip->state = FL_READY;
   put_chip( spiflash);
   mutex_unlock( &chip->mutex);
      
   return ret;
}


//  ==================
//  spsn_spi_otp_write
//  ==================

static int spsn_spi_otp_write(struct mtd_info *mtd, loff_t from, size_t len, 
                              size_t *retlen, u_char *buf)
{
   struct spi_flash *spiflash = (struct spi_flash *)(mtd->priv);
   struct flchip *chip = &spiflash->chip;
   unsigned long timeo;
   unsigned char status;
   int ret;
   
   mutex_lock( &chip->mutex);
   ret = get_chip( spiflash, FL_OTP_WRITE);
   if (ret) {
      mutex_unlock( &chip->mutex);
      return ret;
   }
   chip->state = FL_OTP_WRITE;
   mutex_unlock( &chip->mutex);
   
   *retlen = 0;
   
   while (len && !ret) {
      spiflash->write_op( spiflash, SPI_WRITE_ENABLE, NULL, SPI_NO_ADDRESS, 0);
      spiflash->write_op( spiflash, SPI_OTP_PROGRAM, buf, from, 1);

      chip->state = FL_WRITING;
      
      timeo = jiffies + PROGRAM_TIMEOUT * HZ;
      
      for (;;) {
         if (device_ready( spiflash, &status)) {
            break;
         }
         if (time_after( jiffies, timeo)) {
            printk( KERN_WARNING "MTD %s(): software timeout\n", __func__ );
            break;
         }
         cfi_udelay( 10);  //  drop the lock, wait 10 us and try again
      }
      if (!device_good( spiflash, status))
         ret = -EIO;
      
      buf++;
      from++;
      len--;
      (*retlen)++;
   }

   mutex_lock( &chip->mutex);
   chip->state = FL_READY;
   put_chip( spiflash);
   mutex_unlock( &chip->mutex);
   
   return ret;
}


//  =================
//  spsn_spi_otp_info
//  =================

static int spsn_spi_otp_info(struct mtd_info *mtd, struct otp_info *buf, 
                             size_t len)
{
   struct spi_flash *spiflash = (struct spi_flash *)(mtd->priv);
   struct flchip *chip = &spiflash->chip;
   unsigned char lockbits[2];
   int i, ret;
   
   if (len < 33 * sizeof(struct otp_info))
      return -EIO;
   
   mutex_lock( &chip->mutex);
   ret = get_chip( spiflash, FL_OTP_READ);
   if (ret) {
      mutex_unlock( &chip->mutex);
      return ret;
   }
   chip->state = FL_OTP_READ;
   mutex_unlock( &chip->mutex);

   spiflash->read_op( spiflash, SPI_OTP_READ, lockbits, 0x100, 1);
   for (i = 0; i < 2; i++) {
      buf[i].start  = 0x102 + i * 8;
      buf[i].length = 8;
      buf[i].locked = !((lockbits[0] >> i) & 0x01);
   }
   
   spiflash->read_op( spiflash, SPI_OTP_READ, lockbits, 0x112, 2);
   for (i = 0; i < 8; i++) {
      buf[2 + i].start  = 0x114 + i * 16;
      buf[2 + i].length = 16;
      buf[2 + i].locked = !((lockbits[0] >> i) & 0x01);
   }
   for (i = 0; i < 8; i++) {
      buf[10 + i].start  = 0x194 + i * 16;
      buf[10 + i].length = 16;
      buf[10 + i].locked = !((lockbits[1] >> i) & 0x01);
   }
   
   spiflash->read_op( spiflash, SPI_OTP_READ, lockbits, 0x214, 2);
   for (i = 0; i < 8; i++) {
      buf[18 + i].start  = 0x216 + i * 16;
      buf[18 + i].length = 16;
      buf[18 + i].locked = !((lockbits[0] >> i) & 0x01);
   }
   for (i = 0; i < 6; i++) {
      buf[26 + i].start  = 0x296 + i * 16;
      buf[26 + i].length = 16;
      buf[26 + i].locked = !((lockbits[1] >> i) & 0x01);
   }
   buf[32].start  = 0x2f6;
   buf[32].length = 10;
   buf[32].locked = !((lockbits[1] >> 6) & 0x01);
   
   mutex_lock( &chip->mutex);
   chip->state = FL_READY;
   put_chip( spiflash);
   mutex_unlock( &chip->mutex);
   
   return 33 * sizeof(struct otp_info);
}


//  =================
//  spsn_spi_otp_lock
//  =================

static int spsn_spi_otp_lock(struct mtd_info *mtd, loff_t from, size_t len)
{
   int i, pos, ret;
   unsigned char lockbits[5] = { 0xff, 0xff, 0xff, 0xff, 0xff };
   const int lockaddr[5] = { 0x100, 0x112, 0x113, 0x214, 0x215 };
   size_t retlen;
   
   for (pos = from; pos < from + len; pos++) {
      for (i = 0; i < 2; i++)
         if (pos >= 0x102 + i * 8 && pos < 0x102 + (i + 1) * 8)
            lockbits[0] &= ~(1 << i);
      for (i = 0; i < 8; i++)
         if (pos >= 0x114 + i * 16 && pos < 0x114 + (i + 1) * 16)
            lockbits[1] &= ~(1 << i);
      for (i = 0; i < 8; i++)
         if (pos >= 0x194 + i * 16 && pos < 0x194 + (i + 1) * 16)
            lockbits[2] &= ~(1 << i);
      for (i = 0; i < 8; i++)
         if (pos >= 0x216 + i * 16 && pos < 0x216 + (i + 1) * 16)
            lockbits[3] &= ~(1 << i);
      for (i = 0; i < 6; i++)
         if (pos >= 0x296 + i * 16 && pos < 0x296 + (i + 1) * 16)
            lockbits[4] &= ~(1 << i);
      if (pos >= 0x2f6 && pos < 0x2f6 + 10)
         lockbits[4] &= ~0x40;
   }
   
   for (i = 0; i < 5; i++)
      if (lockbits[i] != 0xff) {
         ret = spsn_spi_otp_write( mtd, lockaddr[i], 1, &retlen, &lockbits[i]);
         if (ret || retlen != 1)
            return ret;
      }
   
   return 0;
}


//  ==============
//  spsn_spi_probe
//  ==============

struct mtd_info* spsn_spi_probe(struct spi_flash *spiflash)
{
   struct mtd_info *mtd;
   unsigned char id_buf[5], conf, top_boot = 0;
   int i;

   //  Initialize spiflash->chip:
   
   spiflash->chip.start = 0;
   spiflash->chip.state = FL_READY;
   init_waitqueue_head( &(spiflash->chip.wq));
   mutex_init( &spiflash->chip.mutex);
   
   if (!(spiflash->chip.priv = kzalloc( sizeof(int), GFP_KERNEL))) { 
      printk( "Failed to allocate chip private data!\n"); 
      return NULL; 
   }
        
   //  Try to reset the device and read its ID/CFI table:
   //  Note: CFI not supported by engineering samples, use ID for now
   
   spiflash->write_op( spiflash, SPI_CLEAR_STATUS, NULL, SPI_NO_ADDRESS, 0);
   spiflash->read_op( spiflash, SPI_READ_ID, id_buf, SPI_NO_ADDRESS, 
                      sizeof( id_buf));
   
   for (i = 0; i < ARRAY_SIZE(spsn_devinfo); i++) {
      if (id_buf[0]  == spsn_devinfo[i].id[0] &&
          id_buf[1]  == spsn_devinfo[i].id[1] && 
          id_buf[2]  == spsn_devinfo[i].id[2] && 
          (-1        == spsn_devinfo[i].id[3] || 
           id_buf[3] == spsn_devinfo[i].id[3]   ) && 
          (-1        == spsn_devinfo[i].id[4] || 
           id_buf[4] == spsn_devinfo[i].id[4]   )) {

         //  Save the device flags and page size:
         
         *(unsigned int*)(spiflash->chip.priv) = spsn_devinfo[i].flags;
         spiflash->pagesize = spsn_devinfo[i].pagesize;
         
         // Check config register including TBPARM config bit (top boot):
         
         if (spsn_devinfo[i].flags & HAS_CONFREG) {
            spiflash->read_op( spiflash, SPI_READ_CONFIG, &conf, 
                               SPI_NO_ADDRESS, 1);
            if (spsn_devinfo[i].flags & HAS_TBPARM)
               top_boot = (conf >> 2) & 0x01;
         }
         
         printk( "Found Spansion %s flash memory device (%s)\n", 
                 spsn_devinfo[i].name, (spsn_devinfo[i].flags & HAS_TBPARM) ? 
                 (top_boot ? "top boot" : "bottom boot") : "uniform sectors");

         //  Check if requested I/O mode is supported:
         
         if (spiflash->io_mode != SPI_SINGLE_IO && 
             spiflash->io_mode != SPI_SINGLE_IO_FASTREAD &&
             !(spsn_devinfo[i].flags & HAS_MULTI_IO)) {
            printk( "ERROR: device does not support the requested "
                    "multi I/O mode!\n");
            kfree( spiflash->chip.priv);
            return NULL;
         }
         
         //  Adjust config register to requested I/O mode:
         
         if (spsn_devinfo[i].flags & HAS_MULTI_IO) {
            switch (spiflash->io_mode) {
            case SPI_QUAD_IO_HP:
            case SPI_QUAD_IO:
            case SPI_QUAD_I:
               if (!(conf & 0x02) && set_confreg( spiflash, conf | 0x02)) {
                  printk( "ERROR: cannot configure device for quad I/O "
                          "mode!\n");
                  kfree( spiflash->chip.priv);
                  return NULL;
               }
               break;
            case SPI_DUAL_IO_HP:
            case SPI_DUAL_IO:
            case SPI_DUAL_I:
            case SPI_SINGLE_IO_FASTREAD:
            case SPI_SINGLE_IO:
               if ((conf & 0x02) && set_confreg( spiflash, conf & ~0x02)) {
                  printk( "ERROR: cannot configure device for single or "
                          "dual I/O mode!\n");
                  kfree( spiflash->chip.priv);
                  return NULL;
               }
               break;
            default:
               printk( "ERROR: unsupported I/O mode!\n");
               kfree( spiflash->chip.priv);
               return NULL;
            }
         }

         //  Enable 4 byte addressing if needed:
         
         if (spsn_devinfo[i].flags & HAS_ADDR32) {
            unsigned char bar = 0x80;
            spiflash->write_op( spiflash, SPI_BAR_WRITE, &bar, 
                                SPI_NO_ADDRESS, 1);
         }
         
         //  Allocate and initialize mtd struct:
         
         if (!(mtd = kzalloc( sizeof(struct mtd_info), GFP_KERNEL))) {
            printk( "ERROR: Failed to allocate memory for MTD device!\n");
            kfree( spiflash->chip.priv);
            return NULL;
         }
         mtd->priv            = spiflash;
         mtd->type            = MTD_NORFLASH;
         mtd->flags           = MTD_CAP_NORFLASH;
         mtd->name            = spsn_devinfo[i].name;
         mtd->erase           = spsn_spi_erase;
         mtd->write           = spsn_spi_write;
         mtd->read            = spsn_spi_read;
         mtd->sync            = spsn_spi_sync;
         mtd->suspend         = spsn_spi_suspend;
         mtd->resume          = spsn_spi_resume;
         mtd->writesize       = 1;
         mtd->numeraseregions = (spsn_devinfo[i].numblocks[1] ? 2 : 1);
         mtd->eraseregions    = kmalloc( sizeof(struct mtd_erase_region_info) * 
                                         mtd->numeraseregions, GFP_KERNEL);
         if (!mtd->eraseregions) {
            printk( "ERROR: Failed to allocate memory for MTD erase region "
                    "info!\n");
            kfree( mtd);
            kfree( spiflash->chip.priv);
            return NULL;
         }
         mtd->size = 
          spsn_devinfo[i].numblocks[0] * spsn_devinfo[i].erasesize[0];
         mtd->erasesize = spsn_devinfo[i].erasesize[0];
         mtd->eraseregions[0 + top_boot].erasesize = 
          spsn_devinfo[i].erasesize[0];
         mtd->eraseregions[0 + top_boot].numblocks = 
          spsn_devinfo[i].numblocks[0];
         mtd->eraseregions[0].offset = 0;
         
         if (spsn_devinfo[i].numblocks[1]) {
            mtd->size += 
             spsn_devinfo[i].numblocks[1] * spsn_devinfo[i].erasesize[1];
            if (spsn_devinfo[i].erasesize[1] > mtd->erasesize)
               mtd->erasesize = spsn_devinfo[i].erasesize[1];
            mtd->eraseregions[1 - top_boot].erasesize = 
             spsn_devinfo[i].erasesize[1];
            mtd->eraseregions[1 - top_boot].numblocks = 
             spsn_devinfo[i].numblocks[1];
            mtd->eraseregions[1].offset = mtd->eraseregions[0].erasesize * 
                                          mtd->eraseregions[0].numblocks;
         }

         //  Install OTP functions (we treat all regions as user blocks):
         
         if (spsn_devinfo[i].flags & HAS_OTP) {
            mtd->read_user_prot_reg  = spsn_spi_otp_read;
            mtd->write_user_prot_reg = spsn_spi_otp_write;
            mtd->get_user_prot_info  = spsn_spi_otp_info;
            mtd->lock_user_prot_reg  = spsn_spi_otp_lock;
         }
         
         return mtd;
      }
   }
   kfree( spiflash->chip.priv);
   return NULL;
}


//  ================
//  spsn_spi_destroy
//  ================

void spsn_spi_destroy(struct mtd_info *mtd)
{
   struct spi_flash *spiflash = (struct spi_flash *)(mtd->priv);
   
   if (mtd->eraseregions)
      kfree( mtd->eraseregions);
   kfree( mtd);
   if (spiflash->chip.priv)
      kfree( spiflash->chip.priv);
}


EXPORT_SYMBOL_GPL(spsn_spi_probe);
EXPORT_SYMBOL_GPL(spsn_spi_destroy);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Spansion Inc.");
MODULE_DESCRIPTION("MTD chip driver for Spansion SPI flash");
