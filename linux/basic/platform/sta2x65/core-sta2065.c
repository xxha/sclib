/*
 * Cartesio (STA206x) Architecture and Board Support Package.
 *
 * Device definitions for Cartesio Plus STA2065 System-on-Chip.
 *
 * Copyright (C) 2009-2010 STMicroelectronics
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/device.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/uio_driver.h>
#include <linux/amba/bus.h>
#include <linux/amba/clcd.h>
#include <linux/amba/pl080_dmac.h>
#include <linux/spi/spi.h>
#include <linux/usb/musb.h>

#include <asm/clkdev.h>
#include <asm/mach-types.h>
#include <asm/mach/arch.h>
#include <asm/mach/irq.h>
#include <asm/mach/map.h>

#include <mach/bridge.h>
#include <mach/clkdev.h>
#include <mach/cpu.h>
#include <mach/eft.h>
#include <mach/fsmc.h>
#include <mach/gps.h>
#include <mach/hardware.h>
#include <mach/memory.h>
#include <mach/i2c.h>
#include <mach/jpeg.h>
#include <mach/mmc.h>
#include <mach/msp.h>
#include <mach/sga.h>
#include <mach/ssp.h>
#include <mach/tsc.h>
#include <mach/vip.h>
#include <mach/sarac.h>
#include <mach/cif.h>
#include <mach/c3.h>
#include <mach/uart.h>

/*
 * =============================================================================
 *   System and Reset Controller (SRC) - Peripheral Clock Gating
 * =============================================================================
 */

/* PLL2 Clock */
unsigned int board_cut_version;

static struct clk pll2_clk = {
	.name		= "PLL2",
	.rate		= 624000000,
};

/* Generic CLK48 AMBA Peripheral Bus (APB) Clock */
static struct clk clk48_clk = {
	.name		= "CLK48",
	.parent		= &pll2_clk,
	.prescaler	= 13,
};

/* ARM PrimeCell PL110 CLCD Controller Clock */
static struct clk clcd_clk = {
	.name		= "CLCDCLK",
	.parent		= &pll2_clk,
	.prescaler	= 8,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(4), .core = SRC_PCLK1_BIT(4) },
	.set_rate	= clcdclk_set_rate,
	.round_rate	= clcdclk_round_rate,
	.flags		= SRC_KEEP_ENABLED | SRC_DONT_OPERATE,
};

/* ARM PrimeCell PL011 UART0 Controller Clock */
static struct clk uart0_clk = {
	.name		= "CLK48_UART0",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(7), .core = SRC_PCLK1_BIT(7) },
};

/* ARM PrimeCell PL011 UART1 Controller Clock */
static struct clk uart1_clk = {
	.name		= "CLK48_UART1",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(11), .core = SRC_PCLK1_BIT(11) },
};

/* ARM PrimeCell PL011 UART2 Controller Clock */
static struct clk uart2_clk = {
	.name		= "CLK48_UART2",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(19), .core = SRC_PCLK1_BIT(19) },
};

/* ARM PrimeCell PL011 UART3 Controller Clock */
static struct clk uart3_clk = {
	.name		= "CLK48_UART3",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK2_BIT(7), .core = SRC_PCLK2_BIT(8) },
};

/* ARM PrimeCell PL180 SDI0 Controller Clock */
static struct clk sdi0_clk = {
	.name		= "SDICLK_SDI0",
	.parent		= &pll2_clk,
	.prescaler	= SDI0_CLK_PRESCALER,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(8), .core = SRC_PCLK1_BIT(8) },
	.set_rate	= sdi0_clk_set_rate,
	.round_rate	= sdi0_clk_round_rate,
};

/* ARM PrimeCell PL180 SDI1 Controller Clock */
static struct clk sdi1_clk = {
	.name		= "SDICLK_SDI1",
	.parent		= &pll2_clk,
	.prescaler	= SDI1_CLK_PRESCALER,
	.pclk_bits	= { .amba = SRC_PCLK2_BIT(1), .core = SRC_PCLK2_BIT(2) },
	.set_rate	= sdi1_clk_set_rate,
	.round_rate	= sdi1_clk_round_rate,
};

/* ARM PrimeCell PL180 SDI2 Controller Clock */
static struct clk sdi2_clk = {
	.name		= "SDICLK_SDI2",
	.parent		= &pll2_clk,
	.prescaler	= SDI2_CLK_PRESCALER,
	.pclk_bits	= { .amba = SRC_PCLK2_BIT(9), .core = SRC_PCLK2_BIT(10) },
	.set_rate	= sdi2_clk_set_rate,
	.round_rate	= sdi2_clk_round_rate,
};

/* ARM PrimeCell PL022 SSP0 Controller Clock */
static struct clk ssp0_clk = {
	.name		= "CLK48_SSP0",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(6), .core = SRC_PCLK1_BIT(6) },
};

/* ARM PrimeCell PL022 SSP1 Controller Clock */
static struct clk ssp1_clk = {
	.name		= "CLK48_SSP1",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK2_BIT(3), .core = SRC_PCLK2_BIT(4) },
};

/* MSP0 Controller Clock */
static struct clk msp0_clk = {
	.name		= "CLK48_MSP0",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(12), .core = SRC_PCLK1_BIT(12) },
};

/* MSP1 Controller Clock */
static struct clk msp1_clk = {
	.name		= "CLK48_MSP1",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(20), .core = SRC_PCLK1_BIT(20) },
};

/* MSP2 Controller Clock */
static struct clk msp2_clk = {
	.name		= "CLK48_MSP2",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(21), .core = SRC_PCLK1_BIT(21) },
};

/* MSP3 Controller Clock */
static struct clk msp3_clk = {
	.name		= "CLK48_MSP3",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK1_BIT(27), .core = SRC_PCLK1_BIT(28) },
};

/* Mentor Inventra USB HS-OTG Controller Clock */
static struct clk musb0_clk = {
	.name		= "CLK48_USB0",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(13), .core = SRC_PCLK1_BIT(13) },
};

/* Mentor Inventra USB FS Controller Clock */
static struct clk musb1_clk = {
	.name		= "CLK48_USB1",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(30), .core = SRC_PCLK1_BIT(30) },
};

/* I2C0 Controller Clock */
static struct clk i2c0_clk = {
	.name		= "CLK48_I2C0",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(9), .core = SRC_PCLK1_BIT(9) },
};

/* I2C1 Controller Clock */
static struct clk i2c1_clk = {
	.name		= "CLK48_I2C1",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(10), .core = SRC_PCLK1_BIT(10) },
};

/* I2C2 Controller Clock */
static struct clk i2c2_clk = {
	.name		= "CLK48_I2C2",
	.parent		= &clk48_clk,
	.pclk_bits	= { .amba = SRC_PCLK2_BIT(5), .core = SRC_PCLK2_BIT(6) },
};

/* ARM PrimeCell PL080 DMA0 Controller Clock */
static struct clk dma0_clk = {
	.name		= "HCLK_DMA0",
	.rate		= 48000000,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(0), .core = SRC_NO_PCLK_BIT },
};

/* ARM PrimeCell PL080 DMA1 Controller Clock */
static struct clk dma1_clk = {
	.name		= "HCLK_DMA1",
	.rate		= 48000000,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(3), .core = SRC_NO_PCLK_BIT },
};

/* Smart Graphics Accelerator Controller Clock */
static struct clk sga_clk = {
	.name		= "HCLK_SGA",
	.rate		= 48000000,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(26), .core = SRC_NO_PCLK_BIT },
};
static struct clk jpeg_clk = {
	.name		= "HCLK_JPEG",
	.rate		= 48000000,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(28), .core = SRC_NO_PCLK_BIT },
};

/* Video input Port Controller Clock */
static struct clk vip_clk = {
	.name		= "HCLK_VIP",
	.rate		= 48000000,
	.pclk_bits	= { .amba = SRC_PCLK0_BIT(27), .core = SRC_NO_PCLK_BIT },
};

/* Pulse Width Light Modulator Controller Clock */
static struct clk pwl_clk = {
	.name		= "CLK32K_PWL",
	.rate		= 32768,
};

/* Pulse Width Light Modulator Controller Clock */
static struct clk pwm_clk = {
	.name		= "CLK48M_PWM",
	.rate		= 48000000,
};

/* Watchdog Timer Clock */
static struct clk wdt_clk = {
	.name		= "CLK32K_WDT",
	.rate		= 32768,
};

static struct clk dummy_apb_pclk;

static struct clk_lookup peripheral_clk_lookups[] = {
	{ .con_id = "apb_pclk", .clk = &dummy_apb_pclk, },
	{ .dev_id = "pll2",  .clk = &pll2_clk,  },
	{ .dev_id = "clk48", .clk = &clk48_clk, },
	{ .dev_id = "clcd",  .clk = &clcd_clk,  },
	{ .dev_id = "uart0", .clk = &uart0_clk, },
	{ .dev_id = "uart1", .clk = &uart1_clk, },
	{ .dev_id = "uart2", .clk = &uart2_clk, },
	{ .dev_id = "uart3", .clk = &uart3_clk, },
	{ .dev_id = "mmc0",  .clk = &sdi0_clk,  },
	{ .dev_id = "mmc1",  .clk = &sdi1_clk,  },
	{ .dev_id = "mmc2",  .clk = &sdi2_clk,  },
	{ .dev_id = "ssp0",  .clk = &ssp0_clk,  },
	{ .dev_id = "ssp1",  .clk = &ssp1_clk,  },
	{ .dev_id = "msp0",  .clk = &msp0_clk,  },
	{ .dev_id = "msp1",  .clk = &msp1_clk,  },
	{ .dev_id = "msp2",  .clk = &msp2_clk,  },
	{ .dev_id = "msp3",  .clk = &msp3_clk,  },
	{ .dev_id = "musb_sta2065_ulpi", .clk = &musb0_clk, },
	{ .dev_id = "musb_sta2065_ephyfs", .clk = &musb0_clk, },
	{ .dev_id = "musb_sta2065_ephy", .clk = &musb1_clk, },
	{ .dev_id = "i2c0",  .clk = &i2c0_clk,  },
	{ .dev_id = "i2c1",  .clk = &i2c1_clk,  },
	{ .dev_id = "i2c2",  .clk = &i2c2_clk,  },
	{ .dev_id = "dma0",  .clk = &dma0_clk,  },
	{ .dev_id = "dma1",  .clk = &dma1_clk,  },
	{ .dev_id = "sga",   .clk = &sga_clk,   },
	{ .dev_id = "vip",   .clk = &vip_clk,   },
	{ .dev_id = "pwl",   .clk = &pwl_clk,   },
	{ .dev_id = "pwm",   .clk = &pwm_clk,   },
	{ .dev_id = "wdt",   .clk = &wdt_clk,   },
	{ .dev_id = "jpeg_decoder",   .clk = &jpeg_clk,   },
};

static struct cartesio_src_platform_data src_platform_data = {
	.clk_table	= peripheral_clk_lookups,
	.clk_table_size	= ARRAY_SIZE(peripheral_clk_lookups),
};

static struct amba_device src_device = {
	.dev = {
		.init_name = "src",
		.platform_data = &src_platform_data,
	},
	.res = {
		.start	= CARTESIO_SRC_BASE,
		.end	= CARTESIO_SRC_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x00041803,
	.irq		= { NO_IRQ, NO_IRQ },
};

/*
 * =============================================================================
 *   Flexible Static Memory Controller (FSMC)
 * =============================================================================
 */

static struct fsmc_platform_data fsmc_platform_data = {
	.bank[0] = {
		.control	= FSMC_BANK0_CONTROL,
		.timing		= FSMC_BANK0_TIMING,
	},
	.bank[1] = {
		.control	= FSMC_BANK1_CONTROL,
		.timing		= FSMC_BANK1_TIMING,
	},
	.bank[2] = {
		.control	= FSMC_BANK2_CONTROL,
		.timing		= FSMC_BANK2_TIMING,
	},
	.device[0] = {
		.control	= FSMC_DEVICE0_CONTROL,
		.cm_timing	= FSMC_DEVICE0_CM_TIMING,
		.am_timing	= FSMC_DEVICE0_AM_TIMING,
		.io_timing	= FSMC_DEVICE0_IO_TIMING,
	},
	.device[1] = {
		.control	= FSMC_DEVICE1_CONTROL,
		.cm_timing	= FSMC_DEVICE1_CM_TIMING,
		.am_timing	= FSMC_DEVICE1_AM_TIMING,
		.io_timing	= FSMC_DEVICE1_IO_TIMING,
	},
};

struct amba_device fsmc_device = {
	.dev = {
		.init_name = "FSMC",
		.platform_data = &fsmc_platform_data,
	},
	.res = {
		.start	= CARTESIO_FSMC_BASE,
		.end	= CARTESIO_FSMC_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x00080090,
	.irq		= { NO_IRQ, NO_IRQ },
};

/*
 * =============================================================================
 *   General Purpose I/O Controllers (GPIO)
 * =============================================================================
 */

static struct gpio_alt_interface gpio_altint_table[] = {
	{ .name = "AC97",		.cfg = "8=C, 17-20=C" },
	{ .name = "CAN_1_A",		.cfg = "23-24=C" },
	{ .name = "CAN_1_B",		.cfg = "76-77=C" },
	{ .name = "CAN_1_C",		.cfg = "80-81=B" },
	{ .name = "CLKOUT0",		.cfg = "25=A" },
	{ .name = "CLKOUT1",		.cfg = "55=A" },
	{ .name = "COMP_FLASH",		.cfg = "27=A, 31=A, 64-67=A" },
	{ .name = "DMA_0",		.cfg = "32-33=A" },
	{ .name = "DMA_1_A",		.cfg = "21-22=B" },
	{ .name = "DMA_1_B",		.cfg = "34-35=A" },
	{ .name = "DMA_1_C",		.cfg = "53-54=B" },
	{ .name = "EFT_ECLK02",		.cfg = "43=B" },
	{ .name = "EFT_ICAP0A",		.cfg = "41=C" },
	{ .name = "EFT_ICAP0B",		.cfg = "40=B" },
	{ .name = "EFT_OCMP0A",		.cfg = "41=B" },
	{ .name = "EFT_OCMP0B",		.cfg = "42=B" },
	{ .name = "EFT_ECLK1",		.cfg = "12=A" },
	{ .name = "EFT_ICAP1B",		.cfg = "11=B" },
	{ .name = "EFT_OCMP1A", 	.cfg = "10=A"  },
	{ .name = "EFT_ICAP2A",		.cfg = "41=C" },
	{ .name = "EFT_OCMP2A",		.cfg = "42=C" },
	{ .name = "EFT_OCMP2B",		.cfg = "43=C" },
	{ .name = "EFT_ECLK3",		.cfg = "12=C" },
	{ .name = "EFT_ICAP3B",		.cfg = "11=B" },
	{ .name = "EFT_OCMP3A",		.cfg = "41=C" },
	{ .name = "FIRDA",		.cfg = "28-29=A" },
	{ .name = "FSMC_A",		.cfg = "9=A, 48-50=C, 83-95=C, 96-123=A" },
	{ .name = "FSMC_B",		.cfg = "9=A, 48-50=C, 83-95=C, 96-123=A, 125=A" },
	{ .name = "FSMC_NAND",		.cfg = "9=A, 117-123=A, 96-105=A" },
	{ .name = "GPS",		.cfg = "5=C, 12=B, 19=B, 48-50=B" },
	{ .name = "I2C_0_A",		.cfg = "62-63=A" },
	{ .name = "I2C_0_B",		.cfg = "2=C, 7=C" },
	{ .name = "I2C_1_A",		.cfg = "0-1=C" },
	{ .name = "I2C_1_B",		.cfg = "3-4=C" },
	{ .name = "I2C_1_C",		.cfg = "53-54=A" },
	{ .name = "I2C_2_A",		.cfg = "28-29=B" },
	{ .name = "I2C_2_B",		.cfg = "36-37=B" },
	{ .name = "I2C_2_C",		.cfg = "76-77=B" },
	{ .name = "MSP_0_A",		.cfg = "8=A, 17-22=A" },
	{ .name = "MSP_0_A_TX",		.cfg = "8=A, 17-20=A" },
	{ .name = "MSP_0_B",		.cfg = "8=A, 21-22=A, 44-47=C" },
	{ .name = "MSP_0_B_TX",		.cfg = "8=A, 44-46=C" },
	{ .name = "MSP_0_C",		.cfg = "8=A, 17-20=A" },
	{ .name = "MSP_0_D",		.cfg = "8=A, 17-19=A, 21-22=A, 47=C" },
	{ .name = "MSP_2",		.cfg = "64-67=C" },
	{ .name = "MSP_2_NOCS",		.cfg = "64-65=C, 67=C"  },
	{ .name = "MSP_3",		.cfg = "51-52=C, 56-57=C" },
	{ .name = "PWLOUT",		.cfg = "13=A" },
	{ .name = "SD_0_8BIT",		.cfg = "32-33=C, 36-37=C" },
	{ .name = "SD_0_8BIT_B",	.cfg = "26-27=B, 30-31=B" },
	{ .name = "SD_1_DIR",		.cfg = "40-47=A, 76-82=A" },
	{ .name = "SD_1",		.cfg = "40-42=A, 44-47=A, 76-79=A" },
	{ .name = "SD_1_4BITS",		.cfg = "40-42=A, 76-79=A" },
	{ .name = "SD_2",		.cfg = "128-131=A, 133=A, 138-142=A" },
	{ .name = "SD_2_8BIT",		.cfg = "128-131=A, 133=A, 134-142=A" },
	{ .name = "SPDIF",		.cfg = "6=C, 23=B, 35=C, 82=B" },
	{ .name = "SRAM",		.cfg = "124=A, 126-127=A" },
	{ .name = "SSP_0_A",		.cfg = "51-52=A, 56-57=A" },
	{ .name = "SSP_0_B",		.cfg = "58-61=A" },
	{ .name = "SSP_0_B_NO_FRM",	.cfg = "58-60=A" },
	{ .name = "SSP_1",		.cfg = "44-47=B" },
	{ .name = "SSP_1_NOFRM",		.cfg = "44-46=B" },
	{ .name = "UART_0",		.cfg = "0-1=A" },
	{ .name = "UART_0_CTS", 	.cfg = "0-2=A, 7=A" },
	{ .name = "UART_0_MDM",		.cfg = "0-7=A" },
	{ .name = "UART_1",		.cfg = "14-15=A" },
	{ .name = "UART_1_CTS_A",	.cfg = "14-15=A, 23=A" },
	{ .name = "UART_1_CTS_B",	.cfg = "14-15=A, 34=C" },
	{ .name = "UART_2_A",		.cfg = "36-39=A" },
	{ .name = "UART_2_B",		.cfg = "53-54=C, 38-39=A" },
	{ .name = "UART_2_C",		.cfg = "38-39=A" },
	{ .name = "UART_3",		.cfg = "3-6=B" },
	{ .name = "USB_DRVVBUS",	.cfg = "25=B" },
	{ .name = "USB_DRVVBUS_82",	.cfg = "82=C" },
	{ .name = "USB_LEGACY",		.cfg = "68-74=B" },
	{ .name = "USB_ULPI",		.cfg = "68-75=C, 26-27=C, 30-31=C" },
	{ .name = "VIP",		.cfg = "143-157=A" },
	{ .name = "GPS_PPS",		.cfg = "25=C" },
};

static struct gpio_platform_data gpio0_platform_data = {
	.block_base		= 0,
	.block_size		= 32,
	.altint_table		= gpio_altint_table,
	.altint_table_size	= ARRAY_SIZE(gpio_altint_table),
};

static struct amba_device gpio0_device = {
	.dev = {
		.init_name = "gpioblock0",
		.platform_data = &gpio0_platform_data,
	},
	.res = {
		.start	= CARTESIO_GPIO0_BASE,
		.end	= CARTESIO_GPIO0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x1F180060,
	.irq		= { IRQ_GPIO0, NO_IRQ },
};

static struct gpio_platform_data gpio1_platform_data = {
	.block_base		= 32,
	.block_size		= 32,
	.altint_table		= gpio_altint_table,
	.altint_table_size	= ARRAY_SIZE(gpio_altint_table),
};

static struct amba_device gpio1_device = {
	.dev = {
		.init_name = "gpioblock1",
		.platform_data = &gpio1_platform_data,
	},
	.res = {
		.start	= CARTESIO_GPIO1_BASE,
		.end	= CARTESIO_GPIO1_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x1F180060,
	.irq		= { IRQ_GPIO1, NO_IRQ },
};

static struct gpio_platform_data gpio2_platform_data = {
	.block_base		= 64,
	.block_size		= 32,
	.altint_table		= gpio_altint_table,
	.altint_table_size	= ARRAY_SIZE(gpio_altint_table),
};

static struct amba_device gpio2_device = {
	.dev = {
		.init_name = "gpioblock2",
		.platform_data = &gpio2_platform_data,
	},
	.res = {
		.start	= CARTESIO_GPIO2_BASE,
		.end	= CARTESIO_GPIO2_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x1F180060,
	.irq		= { IRQ_GPIO2, NO_IRQ },
};

static struct gpio_platform_data gpio3_platform_data = {
	.block_base		= 96,
	.block_size		= 32,
	.altint_table		= gpio_altint_table,
	.altint_table_size	= ARRAY_SIZE(gpio_altint_table),
};

static struct amba_device gpio3_device = {
	.dev = {
		.init_name = "gpioblock3",
		.platform_data = &gpio3_platform_data,
	},
	.res = {
		.start	= CARTESIO_GPIO3_BASE,
		.end	= CARTESIO_GPIO3_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x1F180060,
	.irq		= { IRQ_GPIO3, NO_IRQ },
};

static struct gpio_platform_data gpio4_platform_data = {
	.block_base		= 128,
	.block_size		= 32,
	.altint_table		= gpio_altint_table,
	.altint_table_size	= ARRAY_SIZE(gpio_altint_table),
};

static struct amba_device gpio4_device = {
	.dev = {
		.init_name = "gpioblock4",
		.platform_data = &gpio4_platform_data,
	},
	.res = {
		.start	= CARTESIO_GPIO4_BASE,
		.end	= CARTESIO_GPIO4_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x1F180060,
	.irq		= { IRQ_GPIO4, NO_IRQ },
};

/*
 * =============================================================================
 *   Direct Memory Access Controllers (DMA)
 * =============================================================================
 */

static struct pl080_dma_platform_data dma_platform_data = {
	.nr_channels		= 8,
	.nr_descs_per_channel	= 64,
	.ahb_master_memory	= DMA_AHB_MASTER_1,
};

static struct amba_device dma0_device = {
	.dev = {
		.init_name = "dma0",
		.platform_data = &dma_platform_data,
	},
	.res = {
		.start	= CARTESIO_DMAC0_BASE,
		.end	= CARTESIO_DMAC0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x8A280080,
	.irq		= { IRQ_DMA0, NO_IRQ },
};

struct amba_device dma1_device = {
	.dev = {
		.init_name = "dma1",
		.platform_data = &dma_platform_data,
	},
	.res = {
		.start	= CARTESIO_DMAC1_BASE,
		.end	= CARTESIO_DMAC1_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x8A280080,
	.irq		= { IRQ_DMA1, NO_IRQ },
};

/*
 * =============================================================================
 *   SDIO/SD-Card Host Interface Controllers (SDI)
 * =============================================================================
 */

/* externs defined in board-<machine_name>.c */
extern int cartesio_mmc_platform_init(struct amba_device *dev);
extern void cartesio_mmc_platform_exit(struct amba_device *dev);

#ifdef CONFIG_CARTESIO_MMC0
static struct mmc_platform_data mmc0_platform_data = {
	.id 			= 0,
	.ocr_mask		= MMC_VDD_32_33 | MMC_VDD_33_34,
	.platform_init		= cartesio_mmc_platform_init,
	.platform_exit		= cartesio_mmc_platform_exit,
	.card_detect_gpio	= MMC0_DETECT_GPIO,
	.write_protect_gpio	= MMC0_PROTECT_GPIO,
	.reserved_space		= MMC0_OFFSET,
	.bus_width		= MMC0_BUS_WIDTH,
	.max_frequency_hz	= MMC0_FMAX,
	.cut_version 		= &board_cut_version,
	.dma_slave = {
		.dma_dev	= &dma1_device.dev,	/* bound to DMA1 */
		.tx_req_line	= DMA1_REQ_SDI0,
		.rx_req_line	= DMA1_REQ_SDI0,
		.nr_descs	= 128,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_32BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_PERIPHERAL,
		.burst_size	= DMA_BURST_SIZE_8,	/* data FIFO / 2 */
		.ahb_master	= DMA_AHB_MASTER_0,
	},
};

static struct amba_device mmc0_device = {
	.dev = {
		.init_name = "mmc0",
		.platform_data = &mmc0_platform_data,
	},
	.res = {
		.start	= CARTESIO_SDI0_BASE,
		.end	= CARTESIO_SDI0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x00480180,
	.irq		= { IRQ_SDMMC0, NO_IRQ },
};
#endif

#ifdef CONFIG_CARTESIO_MMC1
static struct mmc_platform_data mmc1_platform_data = {
	.id 			= 1,
	.ocr_mask		= MMC_VDD_32_33 | MMC_VDD_33_34,
	.platform_init		= cartesio_mmc_platform_init,
	.platform_exit		= cartesio_mmc_platform_exit,
	.card_detect_gpio	= MMC1_DETECT_GPIO,
	.write_protect_gpio	= MMC1_PROTECT_GPIO,
	.bus_width		= MMC1_BUS_WIDTH,
	.max_frequency_hz	= MMC1_FMAX,
	.cut_version 		= &board_cut_version,
	.dma_slave = {
		.dma_dev	= &dma0_device.dev,	/* bound to DMA0 */
		.tx_req_line	= DMA0_REQ_SDI1,
		.rx_req_line	= DMA0_REQ_SDI1,
		.nr_descs	= 128,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_32BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_PERIPHERAL,
		.burst_size	= DMA_BURST_SIZE_8,	/* data FIFO / 2 */
		.ahb_master	= DMA_AHB_MASTER_0,
	},
};

static struct amba_device mmc1_device = {
	.dev = {
		.init_name = "mmc1",
		.platform_data = &mmc1_platform_data,
	},
	.res = {
		.start	= CARTESIO_SDI1_BASE,
		.end	= CARTESIO_SDI1_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x00480180,
	.irq		= { IRQ_SDMMC1, NO_IRQ },
};
#endif

#ifdef CONFIG_CARTESIO_MMC2
static struct mmc_platform_data mmc2_platform_data = {
	.id 			= 2,
	.ocr_mask		= MMC_VDD_32_33 | MMC_VDD_33_34,
	.platform_init		= cartesio_mmc_platform_init,
	.platform_exit		= cartesio_mmc_platform_exit,
	.card_detect_gpio	= MMC2_DETECT_GPIO,
	.write_protect_gpio	= MMC2_PROTECT_GPIO,
	.bus_width		= MMC2_BUS_WIDTH,
	.max_frequency_hz	= MMC2_FMAX,
	.cut_version 		= &board_cut_version,
	.dma_slave = {
		.dma_dev	= &dma1_device.dev,	/* bound to DMA0 */
		.tx_req_line	= DMA1_REQ_SDI2,
		.rx_req_line	= DMA1_REQ_SDI2,
		.nr_descs	= 128,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_32BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_PERIPHERAL,
		.burst_size	= DMA_BURST_SIZE_8,	/* data FIFO / 2 */
		.ahb_master	= DMA_AHB_MASTER_0,
	},
};

static struct amba_device mmc2_device = {
	.dev = {
		.init_name = "mmc2",
		.platform_data = &mmc2_platform_data,
	},
	.res = {
		.start	= CARTESIO_SDI2_BASE,
		.end	= CARTESIO_SDI2_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x00480180,
	.irq		= { IRQ_SDMMC2, NO_IRQ },
};
#endif
/*
 * =============================================================================
 *   High-Speed USB On-The-Go Interface Controllers (USB HS-OTG-ULPI)
 * =============================================================================
 */
/*
  USB0 with EPHYFS has some problem with dma in gadget mode.
 */
#ifdef CONFIG_USB_MUSB_HDRC0
static struct musb_hdrc_config musb_hdrc0_hs_otg_ulpi_config = {
	.multipoint	= true,
	.dyn_fifo	= true,
#ifdef CONFIG_USB_MUSB_HDRC0_EPHY
	.dma		= false,
#else
	.dma		= true,
#endif
	.num_eps	= 16,	/* number of endpoints _with_ ep0 */
	.ram_bits	= 16,
};

static struct musb_hdrc_platform_data musb_hdrc0_hs_otg_ulpi_platform_data = {
#if   defined(CONFIG_USB_MUSB_HDRC0_HOST)
	.mode	= MUSB_HOST,
#elif defined(CONFIG_USB_MUSB_HDRC0_PERIPHERAL)
	.mode	= MUSB_PERIPHERAL,
#elif defined(CONFIG_USB_MUSB_HDRC0_OTG)
	.mode	= MUSB_OTG,
#endif
	.clock	= "USBHSCLK",	/* for clk_get() */
	.config = &musb_hdrc0_hs_otg_ulpi_config,
#ifdef CONFIG_USB_CARTESIO_DMA
	.dma_slave = {
		.dma_dev	= &dma1_device.dev,	/* bound to DMA1 */
		.tx_req_line	= DMA1_REQ_USB_OTG_CH0,
		.rx_req_line	= DMA1_REQ_USB_OTG_CH1,
		.nr_descs	= 128,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_32BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_DMA,
		.burst_size	= DMA_BURST_SIZE_128,	/* fifo packet size */
		.ahb_master	= DMA_AHB_MASTER_0,
	},
#endif
};

static struct resource musb_hdrc0_hs_otg_ulpi_resource[] = {
	[0] = {
		.start	= CARTESIO_USB_ULPI_BASE,
		.end	= CARTESIO_USB_ULPI_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_USB_ULPI,
		.end	= IRQ_USB_ULPI,
		.flags	= IORESOURCE_IRQ,
	}
};

u64 musb_hdrc0_hs_otg_ulpi_dma_mask = DMA_BIT_MASK(32);

static struct platform_device musb_hdrc0_hs_otg_ulpi_device = {
	.name		= "musb_hdrc",
	.id		= 0,
	.dev = {
#ifdef CONFIG_USB_MUSB_HDRC0_EPHY
		.init_name = "musb_sta2065_ephyfs",	/* for clk_get() and phy selection */
#else
		.init_name = "musb_sta2065_ulpi",	/* for clk_get() and phy selection */
#endif
		.platform_data = &musb_hdrc0_hs_otg_ulpi_platform_data,
		.dma_mask = &musb_hdrc0_hs_otg_ulpi_dma_mask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.num_resources	= ARRAY_SIZE(musb_hdrc0_hs_otg_ulpi_resource),
	.resource	= musb_hdrc0_hs_otg_ulpi_resource,
};
#endif

/*
 * =============================================================================
 *   High-Speed USB On-The-Go Interface Controllers (USB HS-OTG-EPHY)
 * =============================================================================
 */
#ifdef CONFIG_USB_MUSB_HDRC1
static struct musb_hdrc_config musb_hdrc1_hs_otg_ephy_config = {
	.multipoint	= true,
	.dyn_fifo	= true,
	.dma		= true,
	.num_eps	= 16,	/* number of endpoints _with_ ep0 */
	.ram_bits	= 16,
};

static struct musb_hdrc_platform_data musb_hdrc1_hs_otg_ephy_platform_data = {
#if   defined(CONFIG_USB_MUSB_HDRC1_HOST)
	.mode	= MUSB_HOST,
#elif defined(CONFIG_USB_MUSB_HDRC1_PERIPHERAL)
	.mode	= MUSB_PERIPHERAL,
#elif defined(CONFIG_USB_MUSB_HDRC1_OTG)
	.mode	= MUSB_OTG,
#endif
	.clock	= "USBHSCLK",	/* for clk_get() */
	.config = &musb_hdrc1_hs_otg_ephy_config,
#ifdef CONFIG_USB_CARTESIO_DMA
	.dma_slave = {
		.dma_dev	= &dma0_device.dev,	/* bound to DMA0 */
		.tx_req_line	= DMA0_REQ_USB_FS_CH0,
		.rx_req_line	= DMA0_REQ_USB_FS_CH1,
		.nr_descs	= 128,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_32BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_DMA,
		.burst_size	= DMA_BURST_SIZE_128,	/* fifo packet size / 4 */
		.ahb_master	= DMA_AHB_MASTER_0,
	},
#endif
};

static struct resource musb_hdrc1_hs_otg_ephy_resource[] = {
	[0] = {
		.start	= CARTESIO_USB_EPHY_BASE,
		.end	= CARTESIO_USB_EPHY_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_USB_EPHY,
		.end	= IRQ_USB_EPHY,
		.flags	= IORESOURCE_IRQ,
	}
};

u64 musb_hdrc1_hs_otg_ephy_dma_mask = DMA_BIT_MASK(32);

static struct platform_device musb_hdrc1_hs_otg_ephy_device = {
	.name		= "musb_hdrc",
	.id		= 1,
	.dev = {
		.init_name = "musb_sta2065_ephy",	/* for clk_get() and phy selection */
		.platform_data = &musb_hdrc1_hs_otg_ephy_platform_data,
		.dma_mask = &musb_hdrc1_hs_otg_ephy_dma_mask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.num_resources	= ARRAY_SIZE(musb_hdrc1_hs_otg_ephy_resource),
	.resource	= musb_hdrc1_hs_otg_ephy_resource,
};
#endif
/*
 * =============================================================================
 *   Hardware Accelerated JPEG Decoder
 * =============================================================================
 */

#if defined(CONFIG_CARTESIO_JPEG) || defined(CONFIG_CARTESIO_JPEG_MODULE)

static struct jpeg_platform_data jpeg_platform_data = {
	.mem2jpeg_slave = {
		.dma_dev	= &dma1_device.dev,	/* bound to DMA1 */
		.tx_req_line	= DMA1_REQ_JPEG_TX,
		.rx_req_line	= DMA1_REQ_JPEG_RX,
		.nr_descs	= 2048,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_32BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_DMA,
		.burst_size	= DMA_BURST_SIZE_8,	/* fifo packet size */
		.ahb_master	= DMA_AHB_MASTER_1,
	},
	.jpeg2mem_slave = {
		.dma_dev	= &dma1_device.dev,	/* bound to DMA1 */
		.tx_req_line	= DMA1_REQ_JPEG_TX,
		.rx_req_line	= DMA1_REQ_JPEG_RX,
		.nr_descs	= 4096,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_32BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_PERIPHERAL,
		.burst_size	= DMA_BURST_SIZE_8,	/* fifo packet size */
		.ahb_master	= DMA_AHB_MASTER_1,
	},
};

static struct resource jpeg_decoder_resource[] = {
	[0] = {
		.start	= CARTESIO_JPEG_BASE,
		.end	= CARTESIO_JPEG_BASE + SZ_8K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_JPEG,
		.end	= IRQ_JPEG,
		.flags	= IORESOURCE_IRQ,
	}
};

static struct platform_device jpeg_decoder_device = {
	.name		= "jpeg_decoder",
	.id		= 0,
	.dev = {
		.init_name = "jpeg_decoder",
		.platform_data = &jpeg_platform_data,
	},
	.num_resources	= ARRAY_SIZE(jpeg_decoder_resource),
	.resource	= jpeg_decoder_resource,
};

#endif /* CONFIG_CARTESIO_JPEG || CONFIG_CARTESIO_JPEG_MODULE */

/*
 * =============================================================================
 *   Video Input Port Controller (VIP)
 * =============================================================================
 */

#if defined(CONFIG_VIDEO_CARTESIO_VIP) || defined(CONFIG_VIDEO_CARTESIO_VIP_MODULE)

static struct resource vip_resources[] = {
	[0] = {
		.name	= "vip_regs",
		.start	= (CARTESIO_VIP_BASE),
		.end	= (CARTESIO_VIP_BASE + SZ_128K - 1),
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.name	= "vip_line_irq",
		.start	= IRQ_VIP_LINE,
		.end	= IRQ_VIP_LINE,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		.name	= "vip_vsync_irq",
		.start	= IRQ_VIP_VSYNC,
		.end	= IRQ_VIP_VSYNC,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct vip_platform_data cartesio_vip_data = {
	.dma_slave_odd = {
		.dma_dev	= &dma1_device.dev,	/* bound to DMA1 */
		.tx_req_line	= DMA1_REQ_VIP_ODD,
		.rx_req_line	= DMA1_REQ_VIP_ODD,
		.nr_descs	= 19200,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_32BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_DMA,
		.burst_size	= DMA_BURST_SIZE_4,
		.ahb_master	= DMA_AHB_MASTER_1,
	},
	.dma_slave_even = {
		.dma_dev	= &dma1_device.dev,	/* bound to DMA1 */
		.tx_req_line	= DMA1_REQ_VIP_EVEN,
		.rx_req_line	= DMA1_REQ_VIP_EVEN,
		.nr_descs	= 19200,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_32BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_DMA,
		.burst_size	= DMA_BURST_SIZE_4,
		.ahb_master	= DMA_AHB_MASTER_1,
	},
	.cut_version 		= &board_cut_version,
#ifdef CONFIG_ITU656
	.sync = ITU_R_BT_656,
#else
	.sync = ITU_R_BT_601,
#endif
};

u64 vip_dma_mask = DMA_BIT_MASK(32);

static struct platform_device vip_device = {
	.name		= "vip",
	.id		= 0,
	.dev = {
		.init_name = "vip",	/* for clk_get() */
		.platform_data = &cartesio_vip_data,
		.dma_mask = &vip_dma_mask,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.num_resources	= ARRAY_SIZE(vip_resources),
	.resource	= vip_resources,
};

#endif /* CONFIG_VIDEO_CARTESIO_VIP || CONFIG_VIDEO_CARTESIO_VIP_MODULE */

/*
 * =============================================================================
 *   Smart Graphics Accelerator (SGA)
 * =============================================================================
 */

#if defined(CONFIG_CARTESIO_SGA) || defined(CONFIG_CARTESIO_SGA_MODULE)

static struct cartesio_sga_platform_data sga_platform_data = {
	.video_ram_start	= CONFIG_CARTESIO_SGA_VIDEO_RAM_START,
	.video_ram_size		= CONFIG_CARTESIO_SGA_VIDEO_RAM_SIZE,
};

static struct amba_device sga_device = {
	.dev = {
		.init_name = "sga",
		.platform_data = &sga_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.res = {
		.start	= CARTESIO_SGA_BASE,
		.end	= CARTESIO_SGA_BASE + SZ_2K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.dma_mask	= DMA_BIT_MASK(32),
	.periphid	= 0x002D6202,
	.irq		= { IRQ_SGA, NO_IRQ },
};

#endif /* CONFIG_CARTESIO_SGA || CONFIG_CARTESIO_SGA_MODULE */

/*
 * =============================================================================
 *   GPS Subsystem and GPS Radio Frequency Frontend Receiver
 * =============================================================================
 */

#if defined(CONFIG_UIO_CARTESIO_GPS) || defined(CONFIG_UIO_CARTESIO_GPS_MODULE)

extern int cartesio_gps_board_init(struct platform_device *pdev);

static struct gps_platform_data gps_platform_data = {
	.board_init		= cartesio_gps_board_init,
	.tracker_time_addr	= GPS_TRACKER_TIME,
};

static struct resource gps_resource[] = {
	[0] = {
		.start	= CARTESIO_GALGPS_BASE,
		.end	= CARTESIO_GALGPS_BASE + SZ_1M - 1,
		.name	= "GALGPS Port",
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= CARTESIO_SRC_BASE,
		.end	= CARTESIO_SRC_BASE + SZ_4K - 1,
		.name	= "SRC CartesioPlus",
		.flags	= IORESOURCE_MEM,
	},
	[2] = {
		.start	= CARTESIO_RTC_BASE,
		.end	= CARTESIO_RTC_BASE + SZ_4K - 1,
		.name	= "RTC CartesioPlus",
		.flags	= IORESOURCE_MEM,
	},
	[3] = {
		.start	= IRQ_GPS,
		.end	= IRQ_GPS,
		.name	= "IRQ CartesioPlus",
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device gps_device = {
	.name		= "uio_sta2062_gps",
	.id		= 0,
	.dev = {
		.init_name = "gps",
		.platform_data = &gps_platform_data,
	},
	.num_resources	= ARRAY_SIZE(gps_resource),
	.resource	= gps_resource,
};

#endif /* CONFIG_UIO_CARTESIO_GPS || CONFIG_UIO_CARTESIO_GPS_MODULE */

/* SARAC Device */

#if 0
static struct sarac_platform_data_container sarac_platform_data = {
        .mem2sarac_slave = {
                .dma_dev        = &dma0_device.dev,     /* SARAC bound to DMA0 */
                .tx_req_line    = DMA0_REQ_SRC_DMA_DRE,
                .rx_req_line    = DMA0_REQ_SRC_DMA_DRF,
                .nr_descs       = 2048,
                .channel        = DMA_CHANNEL_AUTO,
                .slave_width    = DMA_TRANSFER_WIDTH_32BIT,
                .flow_ctrlr     = DMA_FLOW_CONTROLLER_DMA,
                .burst_size     = DMA_BURST_SIZE_1,     /* fifo packet size */
                .ahb_master     = DMA_AHB_MASTER_0,
        },
};

static struct resource sarac_resource[] = {
        [0] = {
                .start  = CARTESIO_SARAC_BASE,
                .end    = CARTESIO_SARAC_BASE + SZ_4K - 1,
                .name   = "SaRaC registers",
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_SARAC_UFL,
                .end    = IRQ_SARAC_UFL,
                .name   = "CartesioPlus SaRaC Underflow interrupt",
                .flags  = IORESOURCE_IRQ,
        },
	[2] = {
                .start  = IRQ_SARAC_OFL,
                .end    = IRQ_SARAC_OFL,
                .name   = "CartesioPlus SaRaC Overflow interrupt",
                .flags  = IORESOURCE_IRQ,
        },
};

static struct platform_device sarac_device = {
        .name           = "cartesio-sarac",
        .id             = 1,
        .dev = {
                .init_name = "cartesio-sarac",
                .platform_data = &sarac_platform_data,
        },
        .num_resources  = ARRAY_SIZE(sarac_resource),
        .resource       = sarac_resource,
};

/* SARAC Audio Device */
#endif

/* SARAC Device */

static struct sarac_platform_data_container cartesio_sarac_platform_data = {
        .mem2sarac_slave = {
                .dma_dev        = &dma0_device.dev,     /* SARAC bound to DMA0 */
                .tx_req_line    = DMA0_REQ_SRC_DMA_DRE,
                .rx_req_line    = DMA0_REQ_SRC_DMA_DRF,
                .nr_descs       = 2048,
                .channel        = DMA_CHANNEL_AUTO,
                .slave_width    = DMA_TRANSFER_WIDTH_32BIT,
                .flow_ctrlr     = DMA_FLOW_CONTROLLER_DMA,
                .burst_size     = DMA_BURST_SIZE_1,     /* fifo packet size */
                .ahb_master     = DMA_AHB_MASTER_0,
        },
};

static struct resource cartesio_sarac_resource[] = {
        [0] = {
                .start  = CARTESIO_SARAC_BASE,
                .end    = CARTESIO_SARAC_BASE + SZ_4K - 1,
                .name   = "SaRaC registers",
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .start  = IRQ_SARAC_UFL,
                .end    = IRQ_SARAC_UFL,
                .name   = "CartesioPlus SaRaC Underflow interrupt",
                .flags  = IORESOURCE_IRQ,
        },
        [2] = {
                .start  = IRQ_SARAC_OFL,
                .end    = IRQ_SARAC_OFL,
                .name   = "CartesioPlus SaRaC Overflow interrupt",
                .flags  = IORESOURCE_IRQ,
        },
};

static struct platform_device cartesio_sarac_device = {
        .name           = "cartesio_sarac",
        .id             = 1,
        .dev = {
                .init_name = "cartesio_sarac",
                .platform_data = &cartesio_sarac_platform_data,
        },
        .num_resources  = ARRAY_SIZE(cartesio_sarac_resource),
        .resource       = cartesio_sarac_resource,
};


/*
 * =============================================================================
 *   Controller Area Network Interfaces (CAN)
 * =============================================================================
 */

#if defined(CONFIG_CAN_C_CAN) || defined(CONFIG_CAN_C_CAN_MODULE)

static struct resource can_0_resource[] = {
	[0] = {
		.start  = CARTESIO_CAN0_BASE,
		.end    = CARTESIO_CAN0_BASE + SZ_4K - 1,
		.flags  = IORESOURCE_MEM | IORESOURCE_MEM_32BIT,
	},
	[1] = {
		.start  = IRQ_CAN0,
		.end    = IRQ_CAN0,
		.flags  = IORESOURCE_IRQ,
	}
};

static struct platform_device can0_device = {
	.name		= "c_can_platform",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(can_0_resource),
	.resource	= can_0_resource,
};

static struct resource can_1_resource[] = {
	[0] = {
		.start  = CARTESIO_CAN1_BASE,
		.end    = CARTESIO_CAN1_BASE + SZ_4K - 1,
		.flags  = IORESOURCE_MEM | IORESOURCE_MEM_32BIT,
	},
	[1] = {
		.start  = IRQ_CAN1,
		.end    = IRQ_CAN1,
		.flags  = IORESOURCE_IRQ,
	}
};

static struct platform_device can1_device = {
	.name		= "c_can_platform",
	.id		= 1,
	.num_resources	= ARRAY_SIZE(can_1_resource),
	.resource	= can_1_resource,
};

#endif /* CONFIG_CAN_C_CAN || CONFIG_CAN_C_CAN_MODULE */

/*
 * =============================================================================
 *   Inter-Integrated Circuit Interface Controllers (I2C)
 * =============================================================================
 */

/* externs defined in board-<machine_name>.c */
extern int cartesio_i2c_platform_init(struct amba_device *dev);
extern int cartesio_i2c_platform_exit(struct amba_device *dev);

static struct i2c_platform_data i2c0_platform_data = {
	.slave_address	= I2C0_SLAVE_ADDRESS,
	.busnum		= 0,
	.baud_rate	= I2C0_SPEED,
	.filter		= I2C_FILTER_OFF,
	.platform_init	= cartesio_i2c_platform_init,
	.platform_exit	= cartesio_i2c_platform_exit,
	.dma_threshold	= 12,
};

static struct amba_device i2c0_device = {
	.dev = {
		.init_name = "i2c0",
		.platform_data = &i2c0_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.res = {
		.start	= CARTESIO_I2C0_BASE,
		.end	= CARTESIO_I2C0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.dma_mask	= DMA_BIT_MASK(32),
	.periphid	= 0x00180024,
	.irq		= { IRQ_I2C0, NO_IRQ },
};

static struct i2c_platform_data i2c1_platform_data = {
	.slave_address	= I2C1_SLAVE_ADDRESS,
	.busnum		= 1,
	.baud_rate	= I2C1_SPEED,
	.filter		= I2C_FILTER_OFF,
	.platform_init	= cartesio_i2c_platform_init,
	.platform_exit	= cartesio_i2c_platform_exit,
	.dma_threshold	= 12,
};

static struct amba_device i2c1_device = {
	.dev = {
		.init_name = "i2c1",
		.platform_data = &i2c1_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.res = {
		.start	= CARTESIO_I2C1_BASE,
		.end	= CARTESIO_I2C1_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.dma_mask	= DMA_BIT_MASK(32),
	.periphid	= 0x00180024,
	.irq		= { IRQ_I2C1, NO_IRQ },
};

static struct i2c_platform_data i2c2_platform_data = {
	.slave_address	= I2C2_SLAVE_ADDRESS,
	.busnum		= 2,
	.baud_rate	= I2C2_SPEED,
	.filter		= I2C_FILTER_OFF,
	.platform_init	= cartesio_i2c_platform_init,
	.platform_exit	= cartesio_i2c_platform_exit,
	.dma_threshold	= 12,
};

static struct amba_device i2c2_device = {
	.dev = {
		.init_name = "i2c2",
		.platform_data = &i2c2_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.res = {
		.start	= CARTESIO_I2C2_BASE,
		.end	= CARTESIO_I2C2_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.dma_mask	= DMA_BIT_MASK(32),
	.periphid	= 0x00180024,
	.irq		= { IRQ_I2C2, NO_IRQ },
};

/*
 * =============================================================================
 *   Asynchronous AHB-APB Bridge Controller
 * =============================================================================
 */

static struct resource cartesio_aab_resources[] = {
	[0] = {
		.start	= CARTESIO_AHB_APB_BASE,
		.end	= CARTESIO_AHB_APB_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct platform_device aab_device = {
	.name		= "ahb-apb-bridge",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(cartesio_aab_resources),
	.resource	= cartesio_aab_resources,
};

/*
 * =============================================================================
 *   Pulse Width Light Modulator Controller
 * =============================================================================
 */
#ifdef CONFIG_CARTESIO_PWL
static struct resource cartesio_pwl_resources[] = {
	[0] = {
		.start	= CARTESIO_RTC_BASE,
		.end	= CARTESIO_RTC_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
};

struct platform_device pwm_device = {
	.name		= "pwl",
        .dev = {
                .init_name = "pwl",
        },
	.id		= 0,
	.num_resources	= ARRAY_SIZE(cartesio_pwl_resources),
	.resource	= cartesio_pwl_resources,
};
#endif

#if defined(CONFIG_CARTESIO_PWM)

static struct resource cartesio_pwm1_resources[] = {
	[0] = {
		.start	= CARTESIO_EFT1_BASE,
		.end	= CARTESIO_EFT1_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct cartesio_eft_platform_data pwm1_platform_data = {
	.aab_id		= AAB_EFT1,
	.altf_ocmpa	= "EFT_OCMP1A",
};

struct platform_device pwm_device = {
	.name		= "pwm",
	.dev = {
		.init_name = "pwm",
		.platform_data = &pwm1_platform_data,
	},
	.id		= 1,
	.num_resources	= ARRAY_SIZE(cartesio_pwm1_resources),
	.resource	= cartesio_pwm1_resources,
};
#endif
/*
 * =============================================================================
 *   Extended Function Timer Controllers (EFT)
 * =============================================================================
 */

#if defined(CONFIG_CARTESIO_EFT) || defined(CONFIG_CARTESIO_EFT_MODULE)

static struct resource cartesio_eft0_resources[] = {
	[0] = {
		.start	= CARTESIO_EFT0_BASE,
		.end	= CARTESIO_EFT0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_EFT0,
		.end	= IRQ_EFT0,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct cartesio_eft_platform_data eft0_platform_data = {
	.aab_id		= AAB_EFT0,
	.altf_eclk	= "EFT_ECLK02",
	.altf_icapa	= "EFT_ICAP0A",
	.altf_icapb	= "EFT_ICAP0B",
	.altf_ocmpa	= "EFT_OCMP0A",
	.altf_ocmpb	= "EFT_OCMP0B",
	.init		= EFT0_INIT,
};

static struct platform_device eft0_device = {
	.name		= "eft",
	.id		= 0,
	.dev = {
		.platform_data = &eft0_platform_data,
	},
	.num_resources	= ARRAY_SIZE(cartesio_eft0_resources),
	.resource	= cartesio_eft0_resources,
};

static struct resource cartesio_eft1_resources[] = {
	[0] = {
		.start	= CARTESIO_EFT1_BASE,
		.end	= CARTESIO_EFT1_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_EFT1,
		.end	= IRQ_EFT1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct cartesio_eft_platform_data eft1_platform_data = {
	.aab_id		= AAB_EFT1,
	.altf_eclk	= "EFT_ECLK1",
	.altf_icapb	= "EFT_ICAP1B",
	.init		= EFT1_INIT,
};

static struct platform_device eft1_device = {
	.name		= "eft",
	.id		= 1,
	.dev = {
		.platform_data = &eft1_platform_data,
	},
	.num_resources	= ARRAY_SIZE(cartesio_eft1_resources),
	.resource	= cartesio_eft1_resources,
};

static struct resource cartesio_eft2_resources[] = {
	[0] = {
		.start	= CARTESIO_EFT2_BASE,
		.end	= CARTESIO_EFT2_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_EFT2,
		.end	= IRQ_EFT2,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct cartesio_eft_platform_data eft2_platform_data = {
	.aab_id		= AAB_EFT2,
	.altf_eclk	= "EFT_ECLK02",
	.altf_icapa	= "EFT_ICAP2A",
	.altf_ocmpa	= "EFT_OCMP2A",
	.altf_ocmpb	= "EFT_OCMP2B",
	.init		= EFT2_INIT,
};

static struct platform_device eft2_device = {
	.name		= "eft",
	.id		= 2,
	.dev = {
		.platform_data = &eft2_platform_data,
	},
	.num_resources	= ARRAY_SIZE(cartesio_eft2_resources),
	.resource	= cartesio_eft2_resources,
};

static struct resource cartesio_eft3_resources[] = {
	[0] = {
		.start	= CARTESIO_EFT3_BASE,
		.end	= CARTESIO_EFT3_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_EFT3,
		.end	= IRQ_EFT3,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct cartesio_eft_platform_data eft3_platform_data = {
	.aab_id		= AAB_EFT3,
	.altf_eclk	= "EFT_ECLK3",
	.altf_icapb	= "EFT_ICAP3B",
	.altf_ocmpa	= "EFT_OCMP3A",
	.init		= EFT3_INIT,
};

static struct platform_device eft3_device = {
	.name		= "eft",
	.id		= 3,
	.dev = {
		.platform_data = &eft3_platform_data,
	},
	.num_resources	= ARRAY_SIZE(cartesio_eft3_resources),
	.resource	= cartesio_eft3_resources,
};

#endif /* CONFIG_CARTESIO_EFT || CONFIG_CARTESIO_EFT_MODULE */

/*
 * =============================================================================
 *   Color Liquid Crystal Display Controller (CLCD)
 * =============================================================================
 */

#if defined(CONFIG_FB_ARMCLCD) || defined(CONFIG_FB_ARMCLCD_MODULE)

extern struct clcd_board clcd_data;

static struct amba_device clcd_device = {
	.dev = {
		.init_name = "clcd",
		.platform_data = &clcd_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.res = {
		.start	= CARTESIO_CLCD_BASE,
		.end	= CARTESIO_CLCD_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.dma_mask	= DMA_BIT_MASK(32),
	.periphid	= 0x00180110,
	.irq		= { IRQ_CLCD, NO_IRQ },
};

#endif /* CONFIG_FB_ARMCLCD || CONFIG_FB_ARMCLCD_MODULE */

/*
 * =============================================================================
 *   Real Time Clock Unit (RTC)
 * =============================================================================
 */

static struct amba_device rtc_device = {
	.dev = {
		.init_name = "rtc",
	},
	.res = {
		.start	= CARTESIO_RTC_BASE,
		.end	= CARTESIO_RTC_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x00180031,
	.irq		= { IRQ_RTC_RTT, NO_IRQ },
};

/*
 * =============================================================================
 *   Universal Asynchronous Receiver/Transmitter Controllers (UART)
 * =============================================================================
 */
static struct cartesio_uart_platform_data uart0_platform_data = {
         .dma_slave = {
                .dma_dev        = &dma0_device.dev,     /* bound to DMA0 */
                .tx_req_line    = DMA0_REQ_UART0_TX,
                .rx_req_line    = DMA0_REQ_UART0_RX,
                .nr_descs       = 128,
                .tx_reg         = CARTESIO_UART0_BASE,
                .rx_reg         = CARTESIO_UART0_BASE,
                .channel        = DMA_CHANNEL_AUTO,
                .slave_width    = DMA_TRANSFER_WIDTH_8BIT,
                .flow_ctrlr     = DMA_FLOW_CONTROLLER_DMA,
                .burst_size     = DMA_BURST_SIZE_16,     /* data FIFO / 2 */
                .ahb_master     = DMA_AHB_MASTER_0,
        },
};
static struct amba_device uart0_device = {
	.dev = {
		.init_name = "uart0",
		.platform_data = &uart0_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),

	},
	.res = {
		.start	= CARTESIO_UART0_BASE,
		.end	= CARTESIO_UART0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x00380802,
	.irq		= { IRQ_UART0, NO_IRQ },
};
static struct cartesio_uart_platform_data uart1_platform_data = {
         .dma_slave = {
                .dma_dev        = &dma1_device.dev,     /* bound to DMA1 */
                .tx_req_line    = DMA1_REQ_UART1_TX,
                .rx_req_line    = DMA1_REQ_UART1_RX,
                .nr_descs       = 128,
		.tx_reg         = CARTESIO_UART1_BASE,
                .rx_reg         = CARTESIO_UART1_BASE,
                .channel        = DMA_CHANNEL_AUTO,
                .slave_width    = DMA_TRANSFER_WIDTH_8BIT,
                .flow_ctrlr     = DMA_FLOW_CONTROLLER_DMA,
                .burst_size     = DMA_BURST_SIZE_16,     /* data FIFO / 2 */
                .ahb_master     = DMA_AHB_MASTER_0,
        },
};
static struct amba_device uart1_device = {
	.dev = {
		.init_name = "uart1",
		.platform_data = &uart1_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),

	},
	.res = {
		.start	= CARTESIO_UART1_BASE,
		.end	= CARTESIO_UART1_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x00380802,
	.irq		= { IRQ_UART1, NO_IRQ },
};
static struct cartesio_uart_platform_data uart2_platform_data = {
         .dma_slave = {
                .dma_dev        = &dma1_device.dev,     /* bound to DMA1 */
                .tx_req_line    = DMA1_REQ_UART2_TX,
                .rx_req_line    = DMA1_REQ_UART2_RX,
                .nr_descs       = 128,
		.tx_reg         = CARTESIO_UART2_BASE,
                .rx_reg         = CARTESIO_UART2_BASE,
                .channel        = DMA_CHANNEL_AUTO,
                .slave_width    = DMA_TRANSFER_WIDTH_8BIT,
                .flow_ctrlr     = DMA_FLOW_CONTROLLER_DMA,
                .burst_size     = DMA_BURST_SIZE_16,     /* data FIFO / 2 */
                .ahb_master     = DMA_AHB_MASTER_0,
        },
};
static struct amba_device uart2_device = {
	.dev = {
		.init_name = "uart2",
		.platform_data = &uart2_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),

	},
	.res = {
		.start	= CARTESIO_UART2_BASE,
		.end	= CARTESIO_UART2_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x00380802,
	.irq		= { IRQ_UART2, NO_IRQ },
};
static struct cartesio_uart_platform_data uart3_platform_data = {
         .dma_slave = {
                .dma_dev        = &dma1_device.dev,     /* bound to DMA1 */
                .tx_req_line    = DMA1_REQ_UART3_TX,
                .rx_req_line    = DMA1_REQ_UART3_RX,
                .nr_descs       = 128,
		.tx_reg         = CARTESIO_UART3_BASE,
                .rx_reg         = CARTESIO_UART3_BASE,
                .channel        = DMA_CHANNEL_AUTO,
                .slave_width    = DMA_TRANSFER_WIDTH_8BIT,
                .flow_ctrlr     = DMA_FLOW_CONTROLLER_DMA,
                .burst_size     = DMA_BURST_SIZE_16,     /* data FIFO / 2 */
                .ahb_master     = DMA_AHB_MASTER_0,
        },
};
static struct amba_device uart3_device = {
	.dev = {
		.init_name = "uart3",
		.platform_data = &uart3_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),

	},
	.res = {
		.start	= CARTESIO_UART3_BASE,
		.end	= CARTESIO_UART3_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x00380802,
	.irq		= { IRQ_UART3, NO_IRQ },
};

/*
 * =============================================================================
 *   AC97 Revision 2.3 Compliant Audio Controller
 * =============================================================================
 */

static struct resource cartesio_ac97_resource[] = {
	[0] = {
		.start	= CARTESIO_AC97_BASE,
		.end	= CARTESIO_AC97_BASE + 3 * SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_AC97,
		.end	= IRQ_AC97,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device cartesio_ac97_device = {
	.name		= "cartesio-ac97",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(cartesio_ac97_resource),
	.resource	= cartesio_ac97_resource,
};

/*
 * =============================================================================
 *   Multichannel Serial Port Controllers (MSP)
 * =============================================================================
 */

static struct cartesio_msp_platform_data msp0_platform_data = {
	.bus_num		= MSP0_BUSNUM,
	.dma_slave = {
		.dma_dev	= &dma0_device.dev,	/* bound to DMA0 */
		.tx_req_line	= DMA0_REQ_MSP0_TX,
		.rx_req_line	= DMA0_REQ_MSP0_RX,
		.nr_descs	= 128,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_32BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_DMA,
		.burst_size	= DMA_BURST_SIZE_4,	/* data FIFO / 2 */
		.ahb_master	= DMA_AHB_MASTER_0,
	},
	.iodly = MSP0_IODLY,
};

static struct amba_device msp0_device = {
	.dev = {
		.init_name = "msp0",
		.platform_data = &msp0_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.res = {
		.start	= CARTESIO_MSP0_BASE,
		.end	= CARTESIO_MSP0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.dma_mask	= DMA_BIT_MASK(32),
	.periphid	= 0x00280021,
	.irq		= { IRQ_MSP0, NO_IRQ },
};

static struct cartesio_msp_platform_data msp1_platform_data = {
	.bus_num		= MSP1_BUSNUM,
	.dma_slave = {
		.dma_dev	= &dma0_device.dev,	/* bound to DMA0 */
		.tx_req_line	= DMA0_REQ_MSP1_TX,
		.rx_req_line	= DMA0_REQ_MSP1_RX,
		.nr_descs	= 128,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_32BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_DMA,
		.burst_size	= DMA_BURST_SIZE_4,	/* data FIFO / 2 */
		.ahb_master	= DMA_AHB_MASTER_0,
	},
	.iodly = MSP1_IODLY,
};

static struct amba_device msp1_device = {
	.dev = {
		.init_name = "msp1",
		.platform_data = &msp1_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.res = {
		.start	= CARTESIO_MSP1_BASE,
		.end	= CARTESIO_MSP1_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.dma_mask	= DMA_BIT_MASK(32),
	.periphid	= 0x00280021,
	.irq		= { IRQ_MSP1, NO_IRQ },
};

static struct cartesio_msp_platform_data msp2_platform_data = {
	.bus_num		= MSP2_BUSNUM,
	.dma_slave = {
		.dma_dev	= &dma0_device.dev,	/* bound to DMA0 */
		.tx_req_line	= DMA0_REQ_MSP2_TX,
		.rx_req_line	= DMA0_REQ_MSP2_RX,
		.nr_descs	= 128,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_32BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_DMA,
		.burst_size	= DMA_BURST_SIZE_4,	/* data FIFO / 2 */
		.ahb_master	= DMA_AHB_MASTER_0,
	},
	.iodly = MSP2_IODLY,
};

static struct amba_device msp2_device = {
	.dev = {
		.init_name = "msp2",
		.platform_data = &msp2_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.res = {
		.start	= CARTESIO_MSP2_BASE,
		.end	= CARTESIO_MSP2_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.dma_mask	= DMA_BIT_MASK(32),
	.periphid	= 0x00280021,
	.irq		= { IRQ_MSP2, NO_IRQ },
};

static struct cartesio_msp_platform_data msp3_platform_data = {
	.bus_num		= MSP3_BUSNUM,
	.dma_slave = {
		.dma_dev	= &dma0_device.dev,	/* bound to DMA0 */
		.tx_req_line	= DMA0_REQ_MSP3_TX,
		.rx_req_line	= DMA0_REQ_MSP3_RX,
		.nr_descs	= 128,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_16BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_DMA,
		.burst_size	= DMA_BURST_SIZE_1,	/* data FIFO / 2 */
		.ahb_master	= DMA_AHB_MASTER_0,
	},
	.iodly = MSP3_IODLY,
};

static struct amba_device msp3_device = {
	.dev = {
		.init_name = "msp3",
		.platform_data = &msp3_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.res = {
		.start	= CARTESIO_MSP3_BASE,
		.end	= CARTESIO_MSP3_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.dma_mask	= DMA_BIT_MASK(32),
	.periphid	= 0x00280021,
	.irq		= { IRQ_MSP3, NO_IRQ },
};

/*
 * =============================================================================
 *   Synchronous Serial Port Controllers (SSP)
 * =============================================================================
 */

static struct cartesio_ssp_platform_data ssp0_platform_data = {
	.bus_num		= SSP0_BUSNUM,
	.mode			= SSP0_MODE,
	.dma_slave = {
		.dma_dev	= &dma0_device.dev,	/* bound to DMA0 */
		.tx_req_line	= DMA0_REQ_SSP0_TX,
		.rx_req_line	= DMA0_REQ_SSP0_RX,
		.nr_descs	= 64,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_8BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_DMA,
		.burst_size	= DMA_BURST_SIZE_1,
		.ahb_master	= DMA_AHB_MASTER_0,
	},
};

static struct amba_device ssp0_device = {
	.dev = {
		.init_name = "ssp0",
		.platform_data = &ssp0_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.res = {
		.start	= CARTESIO_SSP0_BASE,
		.end	= CARTESIO_SSP0_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.dma_mask	= DMA_BIT_MASK(32),
	.periphid	= 0x01080022,
	.irq		= { IRQ_SSP0, NO_IRQ },
};

static struct cartesio_ssp_platform_data ssp1_platform_data = {
	.bus_num		= SSP1_BUSNUM,
	.mode			= SSP1_MODE,
	.dma_slave = {
		.dma_dev	= &dma1_device.dev,	/* bound to DMA1 */
		.tx_req_line	= DMA1_REQ_SSP1_TX,
		.rx_req_line	= DMA1_REQ_SSP1_RX,
		.nr_descs	= 64,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_8BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_DMA,
		.burst_size	= DMA_BURST_SIZE_1,
		.ahb_master	= DMA_AHB_MASTER_0,
	},
};

static struct amba_device ssp1_device = {
	.dev = {
		.init_name = "ssp1",
		.platform_data = &ssp1_platform_data,
		.coherent_dma_mask = DMA_BIT_MASK(32),
	},
	.res = {
		.start	= CARTESIO_SSP1_BASE,
		.end	= CARTESIO_SSP1_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.dma_mask	= DMA_BIT_MASK(32),
	.periphid	= 0x01080022,
	.irq		= { IRQ_SSP1, NO_IRQ },
};

/*
 * =============================================================================
 *   Touch Screen Controller (TSC)
 * =============================================================================
 */

#ifdef CONFIG_TOUCHSCREEN_CARTESIO
static struct cartesio_tsc_platform_data tsc_platform_data = {
	.averaging_samples	= TSC_AVG_SAMPLES_8,
	.tracking_index		= TSC_TRK_INDEX_127,
	.volt_settling_time	= TSC_VST_TIME_10US,
	.touch_detect_delay	= TSC_TDD_TIME_100US,
	.samples_per_second	= 0,
	.swap_xy		= TOUCHSCREEN_SWAP_XY,
	.flip_x			= TOUCHSCREEN_FLIP_X,
	.flip_y			= TOUCHSCREEN_FLIP_Y,
};

static struct resource tsc_resource[] = {
	[0] = {
		.start	= CARTESIO_TSC_BASE,
		.end	= CARTESIO_TSC_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TSC_INT,
		.end	= IRQ_TSC_INT,
		.flags	= IORESOURCE_IRQ,
	}
};

static struct platform_device tsc_device = {
	.name		= "cartesio-tsc",
	.id		= 0,
	.dev = {
		.init_name = "tsc",
		.platform_data = &tsc_platform_data,
	},
	.num_resources	= ARRAY_SIZE(tsc_resource),
	.resource	= tsc_resource,
};
#endif

/*
 * =============================================================================
 *   Watchdog Timer (WDT)
 * =============================================================================
 */

static struct amba_device wdt_device = {
	.dev = {
		.init_name = "wdt",
	},
	.res = {
		.start	= CARTESIO_WDT_BASE,
		.end	= CARTESIO_WDT_BASE + SZ_4K - 1,
		.flags	= IORESOURCE_MEM,
	},
	.periphid	= 0x00041805,
	.irq		= { IRQ_WATCHDOG, NO_IRQ },
};

/*
 *==============================================================================
 * CDROM
 *==============================================================================
*/
#if defined(CONFIG_CARTESIO_CDROM_HW)
static struct cartesio_chitf_platform_data cif_platform_data = {
	.dma_slave = {
		.dma_dev	= &dma1_device.dev,	/* bound to DMA1: As per the data sheet */
		.rx_req_line	= DMA1_REQ_CHITF,
		.nr_descs	= 128,
		.channel	= DMA_CHANNEL_AUTO,
		.slave_width	= DMA_TRANSFER_WIDTH_32BIT,
		.flow_ctrlr	= DMA_FLOW_CONTROLLER_DMA,
		.burst_size	= DMA_BURST_SIZE_4,	/* data FIFO / 2 */
		.ahb_master	= DMA_AHB_MASTER_0,
	},

};

static struct cartesio_c3_platform_data  c3_platform_data = {
       .msp = MSP3_BUSNUM,
};


static struct resource c3_resources[] = {
	[0] = {
                .name   = "C3-Regs",
                .start  = (CARTESIO_C3_BASE),
                .end    = (CARTESIO_C3_BASE + SZ_256),
                .flags  = IORESOURCE_MEM,
        },
	[1] = {
		.start  = IRQ_C3_FORMATOR,
		.end    = IRQ_C3_FORMATOR,
		.flags  = IORESOURCE_IRQ,
        },

};


static struct resource bd_resources[] = {
	[0] = {
                .name   = "BD-Regs",
                .start  = (CARTESIO_BD_BASE),
                .end    = (CARTESIO_BD_BASE + 64),
                .flags  = IORESOURCE_MEM,
        },
        [1] = {
                .name   = "DF-Regs",
                .start  = (CARTESIO_DF_BASE),
                .end    = (CARTESIO_DF_BASE + SZ_256),
                .flags  = IORESOURCE_MEM,
        },
	[2] = {
                .start  = IRQ_TATI, /* As per validation code */
                .end    = IRQ_TATI,
                .flags  = IORESOURCE_IRQ,
        },

};

static struct resource cif_resources [] = {
	[0] = {
		.name   = "CIF-Regs",
		.start  = (CARTESIO_CIF_BASE),
		.end    = (CARTESIO_CIF_BASE + SZ_256),
		.flags  = IORESOURCE_MEM,
	},
};

static struct platform_device c3_device = {
        .name           = "cartesio-c3",
        .id             = 0,
	.dev = {
		.init_name = "cartesio-c3",
		.platform_data = &c3_platform_data,
	},
        .num_resources  = ARRAY_SIZE(c3_resources),
        .resource       = c3_resources,
};

static struct platform_device bd_device = {
        .name           = "cartesio-bd",
        .id             = 0,
	.dev = {
		.init_name = "cartesio-bd"
	//	.platform_data = &cartesio_bd_platform_data;
	},
        .num_resources  = ARRAY_SIZE(bd_resources),
        .resource       = bd_resources,
};

static struct platform_device cif_device = {
        .name           = "cartesio-cif",
        .id             = 0,
	.dev = {
		.init_name = "cartesio-cif",
		.platform_data = &cif_platform_data,
	},
        .num_resources  = ARRAY_SIZE(cif_resources),
        .resource       = cif_resources,
};

#endif

/*
 * =============================================================================
 *   Cartesio Product ID and Revision Detection
 * =============================================================================
 */

static void __init cartesio_detect_core(void)
{
	void __iomem *ebootrom;
	enum core_pid product_id;
	u32 ebootrom_ver;
	u8 die_revision;

	/*
	 * Magic offset in Embedded Boot ROM where ROM code version is stored
	 */
	const u32 ebootrom_version_offset = 0x7FFC;


	/*
	 * Product ID is "generic Cartesio Plus" since different
	 * models can share the same Boot ROM code revision
	 */
	product_id = STA20645;

	ebootrom = ioremap(CARTESIO_EBOOTROM_BASE, CARTESIO_EBOOTROM_SIZE);
	ebootrom_ver = readl(ebootrom + ebootrom_version_offset) & 0xFFF;
	iounmap(ebootrom);

	die_revision = ebootrom_ver >> 8;
	board_cut_version = ebootrom_ver;

	cartesio_set_core_identity(product_id, die_revision);
}

/*
 * =============================================================================
 *   Static I/O Memory Mappings
 * =============================================================================
 */

static struct map_desc __initdata cartesio_io_map[] = {
	/* Embedded devices */
	IO_MAP(CARTESIO_SDMC_BASE,      SZ_4K, MT_DEVICE),
	IO_MAP(CARTESIO_CLCD_BASE,      SZ_4K, MT_DEVICE),
	IO_MAP(CARTESIO_VIC_BASE,       SZ_4K, MT_DEVICE),
	IO_MAP(CARTESIO_SRC_BASE,       SZ_4K, MT_DEVICE),
	IO_MAP(CARTESIO_MTU0_BASE,      SZ_4K, MT_DEVICE),
	IO_MAP(CARTESIO_MTU1_BASE,      SZ_4K, MT_DEVICE),
	IO_MAP(CARTESIO_RTC_BASE,       SZ_4K, MT_DEVICE),
	IO_MAP(CARTESIO_PMU_BASE,       SZ_4K, MT_DEVICE),
	IO_MAP(CARTESIO_ASIC_REGS_BASE, SZ_4K, MT_DEVICE),
#ifdef CONFIG_DEBUG_LL
	IO_MAP(DEBUG_LL_UART_BASE,      SZ_4K, MT_DEVICE),
#endif

	/* Embedded memories */
	IO_MAP(CARTESIO_BACKUP_RAM_BASE, CARTESIO_BACKUP_RAM_SIZE, MT_DEVICE),
	IO_MAP(CARTESIO_ESRAM_BASE,      CARTESIO_ESRAM_SIZE,      MT_DEVICE),
	IO_MAP(CARTESIO_AHB_APB_BASE,    SZ_64K,      MT_DEVICE),
//	IO_MAP(CARTESIO_DTCM_BASE,       CARTESIO_DTCM_SIZE,       MT_DEVICE),
//	IO_MAP(CARTESIO_ITCM_BASE,       CARTESIO_ITCM_SIZE,       MT_DEVICE),
};

static void __init cartesio_map_io(void)
{
	iotable_init(cartesio_io_map, ARRAY_SIZE(cartesio_io_map));
}

/*
 * =============================================================================
 *   Cartesio System-on-Chip AMBA Device List
 * =============================================================================
 */

static struct amba_device *amba_devices[] __initdata = {
	&src_device,
	&fsmc_device,
	&gpio0_device,
	&gpio1_device,
	&gpio2_device,
	&gpio3_device,
	&gpio4_device,
	&rtc_device,
	&uart0_device,
	&uart1_device,
	&uart2_device,
	&uart3_device,
	&i2c0_device,
	&i2c1_device,
	&i2c2_device,
	&dma0_device,
	&dma1_device,
	&msp0_device,
	&msp1_device,
	&msp2_device,
	&msp3_device,
	&ssp0_device,
	&ssp1_device,
#ifdef CONFIG_CARTESIO_MMC0
	&mmc0_device,
#endif
#ifdef CONFIG_CARTESIO_MMC1
	&mmc1_device,
#endif
#ifdef CONFIG_CARTESIO_MMC2
	&mmc2_device,
#endif
	&wdt_device,
#if defined(CONFIG_FB_ARMCLCD) || defined(CONFIG_FB_ARMCLCD_MODULE)
	&clcd_device,
#endif
#if defined(CONFIG_CARTESIO_SGA) || defined(CONFIG_CARTESIO_SGA_MODULE)
	&sga_device,
#endif
};

/*
 * =============================================================================
 *   Cartesio System-on-Chip Platform Device List
 * =============================================================================
 */

static struct platform_device *core_devices[] __initdata = {
	&aab_device,
#ifdef CONFIG_CARTESIO_PWL
	&pwm_device,
#endif
#if defined(CONFIG_CARTESIO_EFT) || defined(CONFIG_CARTESIO_EFT_MODULE)
	&eft0_device,
	&eft1_device,
	&eft2_device,
	&eft3_device,
#endif
#ifdef CONFIG_TOUCHSCREEN_CARTESIO
	&tsc_device,
#endif
#if defined(CONFIG_UIO_CARTESIO_GPS) || defined(CONFIG_UIO_CARTESIO_GPS_MODULE)
	&gps_device,
#endif
#if defined(CONFIG_CAN_C_CAN) || defined(CONFIG_CAN_C_CAN_MODULE)
	&can0_device,
	&can1_device,
#endif
	&cartesio_ac97_device,
#ifdef CONFIG_USB_MUSB_HDRC1
	&musb_hdrc1_hs_otg_ephy_device,
#endif
#if defined(CONFIG_VIDEO_CARTESIO_VIP) || defined(CONFIG_VIDEO_CARTESIO_VIP_MODULE)
	&vip_device,
#endif
#if defined(CONFIG_CARTESIO_JPEG) || defined(CONFIG_CARTESIO_JPEG_MODULE)
	&jpeg_decoder_device,
#endif
	// &sarac_device,
	&cartesio_sarac_device,
#if defined(CONFIG_CARTESIO_CDROM_HW)
	&c3_device,
	&bd_device,
	&cif_device,
#endif
};

static struct platform_device *core_late_devices[] __initdata = {
	/*
	 * ULPI-based USB controller is registered late because ULPI
	 * transceiver initialization may eventually depend on board
	 * devices, such as I2C-based GPIO expanders
	 */
#ifdef CONFIG_USB_MUSB_HDRC0
	&musb_hdrc0_hs_otg_ulpi_device,
#endif
};

/*
 * =============================================================================
 *   Cartesio Bootloader Log Access
 * =============================================================================
 */

#ifdef CONFIG_SYSFS
static ssize_t bootlog_show(struct kobject *kobj, struct kobj_attribute *a, char *buf)
{
	const u32 __base = IO_ADDRESS(CARTESIO_ESRAM_BASE) + 0x3800;
	u32 *b32 = (u32 *)buf;
	int i;

	for(i = 0; i < 2048; i += 4)
		*b32++ = readl(__base + i);

	return 2048;
}

static struct kobj_attribute bootlog_attr = __ATTR_RO(bootlog);

static struct attribute *st_attrs[] = {
	&bootlog_attr.attr,
	NULL,
};

static struct attribute_group st_attr_group = {
	.attrs = st_attrs,
	.name = "stmicro",
};
#endif /* CONFIG_SYSFS */

/*
 * =============================================================================
 *   Cartesio Core Initialization
 * =============================================================================
 */

static void __init cartesio_core_init(void)
{
	int i;

	/*
	 * Detect product ID and core revision
	 */
	cartesio_detect_core();

	clkdev_add_table(peripheral_clk_lookups, ARRAY_SIZE(peripheral_clk_lookups));

	/*
	 * Register core-specific AMBA devices
	 */
	for (i = 0; i < ARRAY_SIZE(amba_devices); i++)
		amba_device_register(amba_devices[i], &iomem_resource);

	/*
	 * Register core-specific platform devices
	 */
	platform_add_devices(core_devices, ARRAY_SIZE(core_devices));

	/*
	 * Following devices can wakeup the system
	 */
	device_init_wakeup(&rtc_device.dev, true);
}

static int __init cartesio_core_late_init(void)
{
	/*
	 * Register core-specific platform devices depending on board devices
	 */
	platform_add_devices(core_late_devices, ARRAY_SIZE(core_late_devices));

#ifdef CONFIG_SYSFS
	sysfs_create_group(kernel_kobj, &st_attr_group);
#endif

	return 0;
}

/*
 * This comes after cartesio_board_init(), when all drivers for board
 * devices have already been initialized, and can be used to delay the
 * initialization of core devices when they depend on board devices
 */
fs_initcall(cartesio_core_late_init);

extern void __init cartesio_vic_init(void);
extern struct sys_timer cartesio_sys_timer;

MACHINE_START(CARTESIO_PLUS, CARTESIO_MACHINE_NAME)
	/* Maintainer: Giuseppe Gorgoglione (giuseppe.gorgoglione@st.com) */
	.phys_io	= CARTESIO_FSMC_BASE,
	.io_pg_offst	= (IO_ADDRESS(CARTESIO_FSMC_BASE) >> 18) & 0xFFFC,
	.boot_params	= BOOT_PARAMS_OFFSET,
	.map_io		= cartesio_map_io,
	.init_irq	= cartesio_vic_init,
	.timer		= &cartesio_sys_timer,
	.init_machine	= cartesio_core_init,
MACHINE_END
