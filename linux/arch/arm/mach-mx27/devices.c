/*
 * Author: MontaVista Software, Inc.
 *       <source@mvista.com>
 *
 * Based on the OMAP devices.c
 *
 * 2005 (c) MontaVista Software, Inc. This file is licensed under the
 * terms of the GNU General Public License version 2. This program is
 * licensed "as is" without any warranty of any kind, whether express
 * or implied.
 *
 * Copyright 2006-2007 Freescale Semiconductor, Inc. All Rights Reserved.
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fsl_devices.h>
#include <linux/clk.h>

#include <linux/spi/spi.h>

#include <linux/temac.h>

#include <asm/hardware.h>
#include <asm/arch/pmic_external.h>
#include <asm/arch/pmic_power.h>
#include <asm/arch/mmc.h>

#include <asm/arch-mxc/mx27_pins.h>
#include <linux/i2c-gpio.h>

 /*!
  * @file mach-mx27/devices.c
  * @brief device configurations including nor/nand/watchdog for mx27.
  *
  * @ingroup MSL_MX27
  */

#ifndef CONFIG_MX27_DPTC
extern struct dptc_wp dptc_wp_allfreq[DPTC_WP_SUPPORTED];
#endif

static void mxc_nop_release(struct device *dev)
{
	/* Nothing */
}

#if defined(CONFIG_W1_MASTER_MXC) || defined(CONFIG_W1_MASTER_MXC_MODULE)
static struct mxc_w1_config mxc_w1_data = {
	.search_rom_accelerator = 0,
};

static struct platform_device mxc_w1_devices = {
	.name = "mxc_w1",
	.dev = {
		.release = mxc_nop_release,
		.platform_data = &mxc_w1_data,
		},
	.id = 0
};

static void mxc_init_owire(void)
{
	(void)platform_device_register(&mxc_w1_devices);
}
#else
static inline void mxc_init_owire(void)
{
}
#endif

#if defined(CONFIG_RTC_MXC) || defined(CONFIG_RTC_MXC_MODULE)
static struct resource rtc_resources[] = {
	{
	 .start = RTC_BASE_ADDR,
	 .end = RTC_BASE_ADDR + 0x30,
	 .flags = IORESOURCE_MEM,
	 },
	{
	 .start = INT_RTC,
	 .flags = IORESOURCE_IRQ,
	 },
};
static struct platform_device mxc_rtc_device = {
	.name = "mxc_rtc",
	.id = 0,
	.dev = {
		.release = mxc_nop_release,
		},
	.num_resources = ARRAY_SIZE(rtc_resources),
	.resource = rtc_resources,
};
static void mxc_init_rtc(void)
{
	(void)platform_device_register(&mxc_rtc_device);
}
#else
static inline void mxc_init_rtc(void)
{
}
#endif
#if defined(CONFIG_MXC_WATCHDOG) || defined(CONFIG_MXC_WATCHDOG_MODULE)

static struct resource wdt_resources[] = {
	{
	 .start = WDOG1_BASE_ADDR,
	 .end = WDOG1_BASE_ADDR + 0x30,
	 .flags = IORESOURCE_MEM,
	 },
};

static struct platform_device mxc_wdt_device = {
	.name = "mxc_wdt",
	.id = 0,
	.dev = {
		.release = mxc_nop_release,
		},
	.num_resources = ARRAY_SIZE(wdt_resources),
	.resource = wdt_resources,
};

static void mxc_init_wdt(void)
{
	(void)platform_device_register(&mxc_wdt_device);
}
#else
static inline void mxc_init_wdt(void)
{
}
#endif
/*!
 * This is platform device structure for adding SCC
 */
#if defined(CONFIG_MXC_SECURITY_SCC) || defined(CONFIG_MXC_SECURITY_SCC_MODULE)
static struct platform_device mxc_scc_device = {
	.name = "mxc_scc",
	.id = 0,
};

static void mxc_init_scc(void)
{
	platform_device_register(&mxc_scc_device);
}
#else
static inline void mxc_init_scc(void)
{
}
#endif
/* MMC device data */

#if defined(CONFIG_MMC_MXC) || defined(CONFIG_MMC_MXC_MODULE)

extern unsigned int sdhc_get_card_det_status(struct device *dev);
extern int sdhc_init_card_det(int id);

static struct mxc_mmc_platform_data mmc_data = {
	.ocr_mask = MMC_VDD_27_28 | MMC_VDD_28_29 | MMC_VDD_29_30,
	.min_clk = 150000,
	.max_clk = 25000000,
	.card_inserted_state = 0,
	.status = sdhc_get_card_det_status,
};

/*!
 * Resource definition for the SDHC1
 */
static struct resource mxcsdhc1_resources[] = {
	[0] = {
	       .start = SDHC1_BASE_ADDR,
	       .end = SDHC1_BASE_ADDR + SZ_4K - 1,
	       .flags = IORESOURCE_MEM,
	       },
	[1] = {
	       .start = INT_SDHC1,
	       .end = INT_SDHC1,
	       .flags = IORESOURCE_IRQ,
	       },
	[2] = {
	       .start = 0,
	       .end = 0,
	       .flags = IORESOURCE_IRQ,
	       },
	[3] = {
	       .start = MXC_SDIO1_CARD_IRQ,
	       .end = MXC_SDIO1_CARD_IRQ,
	       .flags = IORESOURCE_IRQ,
	       },
};

/*!
 * Resource definition for the SDHC2
 */
static struct resource mxcsdhc2_resources[] = {
	[0] = {
	       .start = SDHC2_BASE_ADDR,
	       .end = SDHC2_BASE_ADDR + SZ_4K - 1,
	       .flags = IORESOURCE_MEM,
	       },
	[1] = {
	       .start = INT_SDHC2,
	       .end = INT_SDHC2,
	       .flags = IORESOURCE_IRQ,
	       },
	[2] = {
	       .start = 0,
	       .end = 0,
	       .flags = IORESOURCE_IRQ,
	       },
	[3] = {
	       .start = MXC_SDIO2_CARD_IRQ,
	       .end = MXC_SDIO2_CARD_IRQ,
	       .flags = IORESOURCE_IRQ,
	       },
};

/*! Device Definition for MXC SDHC1 */
static struct platform_device mxcsdhc1_device = {
	.name = "mxcmci",
	.id = 0,
	.dev = {
		.release = mxc_nop_release,
		.platform_data = &mmc_data,
		},
	.num_resources = ARRAY_SIZE(mxcsdhc1_resources),
	.resource = mxcsdhc1_resources,
};

/*! Device Definition for MXC SDHC2 */
static struct platform_device mxcsdhc2_device = {
	.name = "mxcmci",
	.id = 1,
	.dev = {
		.release = mxc_nop_release,
		.platform_data = &mmc_data,
		},
	.num_resources = ARRAY_SIZE(mxcsdhc2_resources),
	.resource = mxcsdhc2_resources,
};

#ifdef CONFIG_MXC_SDHC3
/*!
 * Resource definition for the SDHC3
 */
static struct resource mxcsdhc3_resources[] = {
	[0] = {
	       .start = SDHC3_BASE_ADDR,
	       .end = SDHC3_BASE_ADDR + SZ_4K - 1,
	       .flags = IORESOURCE_MEM,
	       },
	[1] = {
	       .start = INT_SDHC3,
	       .end = INT_SDHC3,
	       .flags = IORESOURCE_IRQ,
	       },
	[2] = {
	       .start = 0,
	       .end = 0,
	       .flags = IORESOURCE_IRQ,
	       },
	[3] = {
	       .start = MXC_SDIO3_CARD_IRQ,
	       .end = MXC_SDIO3_CARD_IRQ,
	       .flags = IORESOURCE_IRQ,
	       },
};

/*! Device Definition for MXC SDHC3 */
static struct platform_device mxcsdhc3_device = {
	.name = "mxcmci",
	.id = 2,
	.dev = {
		.release = mxc_nop_release,
		.platform_data = &mmc_data,
		},
	.num_resources = ARRAY_SIZE(mxcsdhc3_resources),
	.resource = mxcsdhc3_resources,
};
#endif

static inline void mxc_init_mmc(void)
{
	int cd_irq;

	cd_irq = sdhc_init_card_det(0);
	if (cd_irq) {
		mxcsdhc1_device.resource[2].start = cd_irq;
		mxcsdhc1_device.resource[2].end = cd_irq;
	}
	cd_irq = sdhc_init_card_det(1);
#if 0
	if (cd_irq) {
		mxcsdhc2_device.resource[2].start = cd_irq;
		mxcsdhc2_device.resource[2].end = cd_irq;
	}
#endif
	(void)platform_device_register(&mxcsdhc1_device);
#if 0
	(void)platform_device_register(&mxcsdhc2_device);
#ifdef CONFIG_MXC_SDHC3
	(void)platform_device_register(&mxcsdhc3_device);
#endif
#endif
}
#else
static inline void mxc_init_mmc(void)
{
}
#endif

/* SPI controller and device data */
#if defined(CONFIG_SPI_MXC) || defined(CONFIG_SPI_MXC_MODULE)

#ifdef CONFIG_SPI_MXC_SELECT1
/*!
 * Resource definition for the CSPI1
 */
static struct resource mxcspi1_resources[] = {
	[0] = {
	       .start = CSPI1_BASE_ADDR,
	       .end = CSPI1_BASE_ADDR + SZ_4K - 1,
	       .flags = IORESOURCE_MEM,
	       },
	[1] = {
	       .start = INT_CSPI1,
	       .end = INT_CSPI1,
	       .flags = IORESOURCE_IRQ,
	       },
};

/*! Platform Data for MXC CSPI1 */
static struct mxc_spi_master mxcspi1_data = {
	.maxchipselect = 4,
	.spi_version = 0,
};

/*! Device Definition for MXC CSPI1 */
static struct platform_device mxcspi1_device = {
	.name = "mxc_spi",
	.id = 0,
	.dev = {
		.release = mxc_nop_release,
		.platform_data = &mxcspi1_data,
		},
	.num_resources = ARRAY_SIZE(mxcspi1_resources),
	.resource = mxcspi1_resources,
};

#endif				/* CONFIG_SPI_MXC_SELECT1 */

#ifdef CONFIG_SPI_MXC_SELECT2
/*!
 * Resource definition for the CSPI2
 */
static struct resource mxcspi2_resources[] = {
	[0] = {
	       .start = CSPI2_BASE_ADDR,
	       .end = CSPI2_BASE_ADDR + SZ_4K - 1,
	       .flags = IORESOURCE_MEM,
	       },
	[1] = {
	       .start = INT_CSPI2,
	       .end = INT_CSPI2,
	       .flags = IORESOURCE_IRQ,
	       },
};

/*! Platform Data for MXC CSPI2 */
static struct mxc_spi_master mxcspi2_data = {
	.maxchipselect = 4,
	.spi_version = 0,
};

/*! Device Definition for MXC CSPI2 */
static struct platform_device mxcspi2_device = {
	.name = "mxc_spi",
	.id = 1,
	.dev = {
		.release = mxc_nop_release,
		.platform_data = &mxcspi2_data,
		},
	.num_resources = ARRAY_SIZE(mxcspi2_resources),
	.resource = mxcspi2_resources,
};
#endif				/* CONFIG_SPI_MXC_SELECT2 */

#ifdef CONFIG_SPI_MXC_SELECT3
/*!
 * Resource definition for the CSPI3
 */
static struct resource mxcspi3_resources[] = {
	[0] = {
	       .start = CSPI3_BASE_ADDR,
	       .end = CSPI3_BASE_ADDR + SZ_4K - 1,
	       .flags = IORESOURCE_MEM,
	       },
	[1] = {
	       .start = INT_CSPI3,
	       .end = INT_CSPI3,
	       .flags = IORESOURCE_IRQ,
	       },
};

/*! Platform Data for MXC CSPI3 */
static struct mxc_spi_master mxcspi3_data = {
	.maxchipselect = 4,
	.spi_version = 0,
};

/*! Device Definition for MXC CSPI3 */
static struct platform_device mxcspi3_device = {
	.name = "mxc_spi",
	.id = 2,
	.dev = {
		.release = mxc_nop_release,
		.platform_data = &mxcspi3_data,
		},
	.num_resources = ARRAY_SIZE(mxcspi3_resources),
	.resource = mxcspi3_resources,
};
#endif				/* CONFIG_SPI_MXC_SELECT3 */

static inline void mxc_init_spi(void)
{
#ifdef CONFIG_SPI_MXC_SELECT1
	if (platform_device_register(&mxcspi1_device) < 0)
		printk(KERN_ERR "Registering the SPI Controller_1\n");
#endif				/* CONFIG_SPI_MXC_SELECT1 */
#ifdef CONFIG_SPI_MXC_SELECT2
	if (platform_device_register(&mxcspi2_device) < 0)
		printk(KERN_ERR "Registering the SPI Controller_2\n");
#endif				/* CONFIG_SPI_MXC_SELECT2 */
#ifdef CONFIG_SPI_MXC_SELECT3
	if (platform_device_register(&mxcspi3_device) < 0)
		printk(KERN_ERR "Registering the SPI Controller_3\n");
#endif				/* CONFIG_SPI_MXC_SELECT3 */
}
#else
static inline void mxc_init_spi(void)
{
}
#endif

/* I2C controller and device data */
#if defined(CONFIG_I2C_MXC) || defined(CONFIG_I2C_MXC_MODULE)

#ifdef CONFIG_I2C_MXC_SELECT1
/*!
 * Resource definition for the I2C1
 */
static struct resource mxci2c1_resources[] = {
	[0] = {
	       .start = I2C_BASE_ADDR,
	       .end = I2C_BASE_ADDR + SZ_4K - 1,
	       .flags = IORESOURCE_MEM,
	       },
	[1] = {
	       .start = INT_I2C,
	       .end = INT_I2C,
	       .flags = IORESOURCE_IRQ,
	       },
};

/*! Platform Data for MXC I2C */
static struct mxc_i2c_platform_data mxci2c1_data = {
	.i2c_clk = 60000,	//100000,
};
#endif

#ifdef CONFIG_I2C_MXC_SELECT2
/*!
 * Resource definition for the I2C2
 */
static struct resource mxci2c2_resources[] = {
	[0] = {
	       .start = I2C2_BASE_ADDR,
	       .end = I2C2_BASE_ADDR + SZ_4K - 1,
	       .flags = IORESOURCE_MEM,
	       },
	[1] = {
	       .start = INT_I2C2,
	       .end = INT_I2C2,
	       .flags = IORESOURCE_IRQ,
	       },
};

/*! Platform Data for MXC I2C */
static struct mxc_i2c_platform_data mxci2c2_data = {
	.i2c_clk = 60000,	//100000,
};
#endif

/*! Device Definition for MXC I2C */
static struct platform_device mxci2c_devices[] = {
#ifdef CONFIG_I2C_MXC_SELECT1
	{
	 .name = "mxc_i2c",
	 .id = 0,
	 .dev = {
		 .release = mxc_nop_release,
		 .platform_data = &mxci2c1_data,
		 },
	 .num_resources = ARRAY_SIZE(mxci2c1_resources),
	 .resource = mxci2c1_resources,},
#endif
#ifdef CONFIG_I2C_MXC_SELECT2
	{
	 .name = "mxc_i2c",
	 .id = 1,
	 .dev = {
		 .release = mxc_nop_release,
		 .platform_data = &mxci2c2_data,
		 },
	 .num_resources = ARRAY_SIZE(mxci2c2_resources),
	 .resource = mxci2c2_resources,},
#endif
};

static inline void mxc_init_i2c(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(mxci2c_devices); i++) {
		if (platform_device_register(&mxci2c_devices[i]) < 0)
			dev_err(&mxci2c_devices[i].dev,
				"Unable to register I2C device\n");
	}
}
#else
static inline void mxc_init_i2c(void)
{
}
#endif

#ifdef	CONFIG_MXC_VPU
/*! Platform Data for MXC VPU */
static struct platform_device mxcvpu_device = {
	.name = "mxc_vpu",
	.dev = {
		.release = mxc_nop_release,
		},
	.id = 0,
};

static inline void mxc_init_vpu(void)
{
	if (platform_device_register(&mxcvpu_device) < 0)
		printk(KERN_ERR "Error: Registering the VPU.\n");
}
#else
static inline void mxc_init_vpu(void)
{
}
#endif

struct mxc_gpio_port mxc_gpio_ports[GPIO_PORT_NUM] = {
	{
	 .num = 0,
	 .base = IO_ADDRESS(GPIO_BASE_ADDR),
	 .irq = INT_GPIO,
	 .virtual_irq_start = MXC_GPIO_BASE,
	 },
	{
	 .num = 1,
	 .base = IO_ADDRESS(GPIO_BASE_ADDR) + 0x100,
	 .irq = INT_GPIO,
	 .virtual_irq_start = MXC_GPIO_BASE + GPIO_NUM_PIN,
	 },
	{
	 .num = 2,
	 .base = IO_ADDRESS(GPIO_BASE_ADDR) + 0x200,
	 .irq = INT_GPIO,
	 .virtual_irq_start = MXC_GPIO_BASE + GPIO_NUM_PIN * 2,
	 },
	{
	 .num = 3,
	 .base = IO_ADDRESS(GPIO_BASE_ADDR) + 0x300,
	 .irq = INT_GPIO,
	 .virtual_irq_start = MXC_GPIO_BASE + GPIO_NUM_PIN * 3,
	 },
	{
	 .num = 4,
	 .base = IO_ADDRESS(GPIO_BASE_ADDR) + 0x400,
	 .irq = INT_GPIO,
	 .virtual_irq_start = MXC_GPIO_BASE + GPIO_NUM_PIN * 4,
	 },
	{
	 .num = 5,
	 .base = IO_ADDRESS(GPIO_BASE_ADDR) + 0x500,
	 .irq = INT_GPIO,
	 .virtual_irq_start = MXC_GPIO_BASE + GPIO_NUM_PIN * 5,
	 },
};

static struct i2c_gpio_platform_data i2c_gpio_data = {
	.sda_pin		= MX27_PIN_I2C_DATA,
	.scl_pin		= MX27_PIN_I2C_CLK,
	.sda_is_open_drain	= 0,
	.scl_is_open_drain	= 0,
	.udelay			= 8,
};

static struct platform_device i2c_gpio_device = {
	.name		= "i2c-gpio",
	.id		= 0,
	.dev		= {
		.platform_data	= &i2c_gpio_data,
	},
};

static struct i2c_gpio_platform_data i2c2_gpio_data = {
	.sda_pin                = MX27_PIN_I2C2_SDA,
	.scl_pin                = MX27_PIN_I2C2_SCL,
	.sda_is_open_drain      = 0,
	.scl_is_open_drain      = 0,
	.udelay                 = 8,
};

static struct platform_device i2c2_gpio_device = {
	.name           = "i2c-gpio",
	.id             = 1,
	.dev            = {
		.platform_data  = &i2c2_gpio_data,
	},
};

static inline void mxc_init_i2c_gpio(void)
{
	if (platform_device_register( &i2c_gpio_device ) < 0){
		dev_err( &i2c_gpio_device.dev, "Unable to register I2C-1 GPIO device\n");
	}

        if (platform_device_register( &i2c2_gpio_device ) < 0){
		dev_err( &i2c2_gpio_device.dev, "Unable to register I2C-2 GPIO device\n");
	}
}

#ifndef CONFIG_MX27_DPTC
/*! Device Definition for DPTC */
static struct platform_device mxc_dptc_device = {
	.name = "mxc_dptc",
	.dev = {
		.release = mxc_nop_release,
		.platform_data = &dptc_wp_allfreq,
		},
};

static inline void mxc_init_dptc(void)
{
	(void)platform_device_register(&mxc_dptc_device);
}
#endif

#if defined(CONFIG_PATA_FSL) || defined(CONFIG_PATA_FSL_MODULE)
static struct clk *ata_clk;
extern void gpio_ata_active(void);
extern void gpio_ata_inactive(void);

static int ata_init(struct platform_device *pdev)
{
	/* Configure the pins */

	gpio_ata_active();

	/* Enable the clock */

	ata_clk = clk_get(&pdev->dev, "ata_clk.0");
	clk_enable(ata_clk);

	return 0;
}

static void ata_exit(void)
{
	/* Disable the clock */

	clk_disable(ata_clk);
	clk_put(ata_clk);
	ata_clk = NULL;

	/* Free the pins */

	gpio_ata_inactive();
}

static int ata_get_clk_rate(void)
{
	return clk_get_rate(ata_clk);
}

static struct fsl_ata_platform_data ata_data = {
	.udma_mask        = 0x0F, /* the board handles up to UDMA3 */
	.fifo_alarm       = MXC_IDE_DMA_WATERMARK / 2,
	.max_sg           = MXC_IDE_DMA_BD_NR,
	.init             = ata_init,
	.exit             = ata_exit,
	.get_clk_rate     = ata_get_clk_rate,
};

static struct resource pata_fsl_resources[] = {
	[0] = {		/* I/O */
		.start		= IO_ADDRESS(ATA_BASE_ADDR + 0x00),
		.end		= IO_ADDRESS(ATA_BASE_ADDR + 0xD8),
		.flags		= IORESOURCE_MEM,
	},
	[2] = {		/* IRQ */
		.start		= INT_ATA,
		.end		= INT_ATA,
		.flags		= IORESOURCE_IRQ,
	},
};

static struct platform_device pata_fsl_device = {
	.name			= "pata_fsl",
	.id			= -1,
	.num_resources		= ARRAY_SIZE(pata_fsl_resources),
	.resource		= pata_fsl_resources,
	.dev			= {
		.platform_data	= &ata_data,
		.coherent_dma_mask = ~0,	/* $$$ REVISIT */
	},
};

static inline void mxc_init_pata(void)
{
	(void)platform_device_register(&pata_fsl_device);
}
#else /* CONFIG_PATA_FSL */
static inline void mxc_init_pata(void)
{
}
#endif /* CONFIG_PATA_FSL */

#if defined(CONFIG_TEMAC) || defined(CONFIG_TEMAC_MODULE)

#define	V100P_TEMAC_REG_BASE					0xD4202000
#define	V100P_TEMAC_REG_ZISE					0x2000

#define	TEMAC_INT		IOMUX_TO_IRQ(MX27_PIN_UART3_RTS)

/* We use 16 bit bus only now.
*/
static struct temac_plat_data __initdata mx27_temac_platdata = {
	.flags = TEMAC_PLATF_16BITONLY,
};


static struct temac_plat_data temac_data;

static struct resource mx27_temac_resources[] = {
	[0] = {
		.start = V100P_TEMAC_REG_BASE,
		.end = V100P_TEMAC_REG_BASE+V100P_TEMAC_REG_ZISE,
		.flags = IORESOURCE_MEM
	},
	[1] = {
		.start = TEMAC_INT,
		.end = TEMAC_INT,
		.flags = IORESOURCE_IRQ
	}
};

static struct platform_device mx27_temac_device = {
	.name = "temac",
	.id = 0,
	.num_resources = ARRAY_SIZE(mx27_temac_resources),
	.resource = mx27_temac_resources,
	.dev = {
		.platform_data = & temac_data,
	}
};

void __init mx27_add_device_temac(struct temac_plat_data *data)
{
	if (!data)
		return;

	temac_data = *data;
	platform_device_register(&mx27_temac_device);
}
#else
void __init mx27_add_device_temac (struct temac_plat_data *data) {}
#endif

/*******************************************************************/
#if defined(CONFIG_TX130M_TEMAC) || defined(CONFIG_TX130M_TEMAC_MODULE)

#define V100P_TX130M_TEMAC_REG_BASE		0xD4232000	//0xD4262000
#define V100P_TX130M_TEMAC_REG_ZISE		0x4000		//0x20000

#define TX130M_TEMAC_INT               IOMUX_TO_IRQ(MX27_PIN_UART3_RTS)

/* We use 16 bit bus only now.
 * */
static struct temac_plat_data __initdata mx27_tx130m_temac_platdata = {
	        .flags = TEMAC_PLATF_16BITONLY,
};


static struct temac_plat_data tx130m_temac_data;

static struct resource mx27_tx130m_temac_resources[] = {
	        [0] = {
			.start = V100P_TX130M_TEMAC_REG_BASE,
			.end = V100P_TX130M_TEMAC_REG_BASE+V100P_TX130M_TEMAC_REG_ZISE,
			.flags = IORESOURCE_MEM
		},
		[1] = {
			.start = TX130M_TEMAC_INT,
			.end = TX130M_TEMAC_INT,
			.flags = IORESOURCE_IRQ
		}
};

static struct platform_device mx27_tx130m_temac_device = {
	        .name = "tx130m_temac",
		.id = 0,
		.num_resources = ARRAY_SIZE(mx27_tx130m_temac_resources),
		.resource = mx27_tx130m_temac_resources,
		.dev = {
			.platform_data = & tx130m_temac_data,
		}
};

void __init mx27_add_device_tx130m_temac(struct temac_plat_data *data)
{
	        if (!data)
			return;

	        temac_data = *data;
		platform_device_register(&mx27_tx130m_temac_device);
}
#else
void __init mx27_add_device_tx130m_temac (struct temac_plat_data *data) {}
#endif




static int __init mxc_init_devices(void)
{
	mxc_init_i2c_gpio();
	mxc_init_wdt();
	mxc_init_mmc();
	mxc_init_spi();
	mxc_init_i2c();
	mxc_init_rtc();
	mxc_init_scc();
	mxc_init_owire();
	mxc_init_vpu();
#ifndef CONFIG_MX27_DPTC
	mxc_init_dptc();
#endif
	mxc_init_pata();
	mx27_add_device_temac(&mx27_temac_platdata);
	mx27_add_device_tx130m_temac(&mx27_tx130m_temac_platdata);

	return 0;
}

arch_initcall(mxc_init_devices);
