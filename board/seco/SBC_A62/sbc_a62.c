// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright (C) 2012-2016 Freescale Semiconductor, Inc.
 * Copyright 2017-2018 NXP
 *
 * Author: Fabio Estevam <fabio.estevam@freescale.com>
 */

#include <asm/arch/clock.h>
#include <asm/arch/imx-regs.h>
#include <asm/arch/iomux.h>
#include <asm/arch/mx6-pins.h>
#include <asm/mach-imx/spi.h>
#include <linux/errno.h>
#include <asm/gpio.h>
#include <asm/mach-imx/mxc_i2c.h>
#include <asm/mach-imx/iomux-v3.h>
#include <asm/mach-imx/boot_mode.h>
#include <asm/mach-imx/video.h>
#include <mmc.h>
#include <fsl_esdhc.h>
#include <miiphy.h>
#include <netdev.h>
#include <asm/arch/mxc_hdmi.h>
#include <asm/arch/crm_regs.h>
#include <asm/io.h>
#include <asm/arch/sys_proto.h>
#include <i2c.h>
#include <input.h>
#include <usb.h>
#include <usb/ehci-ci.h>
#include <power/regulator.h>
#include <micrel.h>


#include "pin_muxing_SBC_A62.h"
#include "../common/proto_seco.h"

DECLARE_GLOBAL_DATA_PTR;




int dram_init( void ) {
	gd->ram_size = imx_ddr_size();
	return 0;
}

void ldo_mode_set(int ldo_bypass) {}


#if defined(CONFIG_PCIE_IMX) && !defined(CONFIG_DM_PCI)


static void setup_pcie(void)
{

}
#endif


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   UART                                   |
 * |__________________________________________________________________________|
 */
static void setup_iomux_uart( void ) {
	SETUP_IOMUX_PADS( uart_debug_pads );
}
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */

/*  __________________________________________________________________________
 * |                                                                          |
 * |                               WATCHDOG APX                               |
 * |__________________________________________________________________________|
 */
static inline void setup_iomux_apx_watchdog( void ) {
	 SETUP_IOMUX_PADS( wdt_trigger_pads );
}


#ifdef CONFIG_APX_WATCHDOG
static void trigget_apx_watchdog( void ) {
	hw_watchdog_reset( );
}
#else
static inline void disable_apx_watchdog( void ) {
	setup_iomux_apx_watchdog( );
	gpio_direction_output( IMX_GPIO_NR(4, 11), 0 );
}
#endif
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   USDHC                                  |
 * |__________________________________________________________________________|
 */
#ifdef CONFIG_FSL_ESDHC

/* map the usdhc controller id to the devno given to the board device */
int usdhc_devno[4] = { -1, -1, 1, 0};

boot_mem_dev_t boot_mem_dev_list[SECO_NUM_BOOT_DEV] = {
	{ 0x3, 0x3, -1,  -1,  0, "SPI Nor" },
	{ 0x4, 0x5, -1,   0, -1, "External SD" },
	{ 0x6, 0x7, -1,   0, -1, "eMMC" },
};

#ifdef CONFIG_SPL_BUILD

#define USDHC3_CD_GPIO	                          IMX_GPIO_NR(7, 0)

/* USDHC map:
 * 	USDHC4  -->  eMMC  -->  FSL_SDHC: 0
 * 	USDHC3  -->  uSD   -->  FSL_SDHC: 1
 */

struct fsl_esdhc_cfg usdhc_cfg[CONFIG_SYS_FSL_USDHC_NUM] = {
	{ USDHC4_BASE_ADDR, 0, 8 },
	{ USDHC3_BASE_ADDR, 0, 4 },
};


struct usdhc_l usdhc_list_spl[CONFIG_SYS_FSL_USDHC_NUM] = {
	{usdhc4_pads, ARRAY_SIZE(usdhc4_pads), -1, IMX_GPIO_NR(7, 0)},
	{usdhc3_pads, ARRAY_SIZE(usdhc3_pads), USDHC3_CD_GPIO, -1},
};

enum mxc_clock usdhc_clk[CONFIG_SYS_FSL_USDHC_NUM] = {
	MXC_ESDHC4_CLK,
	MXC_ESDHC3_CLK,
};




int board_mmc_getcd (struct mmc *mmc) {
	struct fsl_esdhc_cfg *cfg = (struct fsl_esdhc_cfg *)mmc->priv;
	int ret = 0;
	switch (cfg->esdhc_base) {
		case USDHC3_BASE_ADDR:
			ret = !gpio_get_value( USDHC3_CD_GPIO );
			break;
		case USDHC4_BASE_ADDR:
			ret = 1; /* eMMC/uSDHC4 is always present */
			break;
	}
	return ret;
}


#if defined(CONFIG_MX6QDL)

int board_mmc_init( bd_t *bis ) {
	struct src *psrc = (struct src *)SRC_BASE_ADDR;
	unsigned reg = readl(&psrc->sbmr1) >> 11;
	/*
	 * Upon reading BOOT_CFG register the following map is done:
	 * Bit 11 and 12 of BOOT_CFG register can determine the current
	 * mmc port
	 * 0x2                  SD3
	 * 0x3                  SD4
	 */
	int ret = 0;
	int index = 0;
	
	switch (reg & 0x3) {
		case 2:
			index = 1;
			break;
		case 3:
			index = 0;
			break;
	}


	print_boot_device( );
	
	SETUP_IOMUX_PADS( usdhc_list_spl[index].pad );	
	usdhc_cfg[index].sdhc_clk = mxc_get_clock( usdhc_clk[index] );
	gd->arch.sdhc_clk = usdhc_cfg[index].sdhc_clk;
	ret = fsl_esdhc_initialize( bis, &usdhc_cfg[index] );

	if ( ret ) 
		printf( "Warning: failed to initialize mmc dev %d\n", index );

	return ret;
}

#endif
#endif	/*  CONFIG_SPL_BUILD  */
#endif  /*  CONFIG_FSL_ESDHC  */
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                   ETHERNET                               |
 * |__________________________________________________________________________|
 */
int board_phy_config (struct phy_device *phydev) {
	phy_write (phydev, MDIO_DEVAD_NONE, MII_BMCR, 0x2100);
	phydev->autoneg = 0;

/* Skew setting */
	ksz9031_phy_extended_write(phydev, 0x02,
			MII_KSZ9031_EXT_RGMII_CTRL_SIG_SKEW,
			MII_KSZ9031_MOD_DATA_NO_POST_INC, 0x0000);
	/* rx data pad skew - devaddr = 0x02, register = 0x05 */
	ksz9031_phy_extended_write(phydev, 0x02,
			MII_KSZ9031_EXT_RGMII_RX_DATA_SKEW,
			MII_KSZ9031_MOD_DATA_NO_POST_INC, 0x0000);
	/* tx data pad skew - devaddr = 0x02, register = 0x05 */
	ksz9031_phy_extended_write(phydev, 0x02,
			MII_KSZ9031_EXT_RGMII_TX_DATA_SKEW,
			MII_KSZ9031_MOD_DATA_NO_POST_INC, 0x0000);
	/* gtx and rx clock pad skew - devaddr = 0x02, register = 0x08 */
	ksz9031_phy_extended_write(phydev, 0x02,
			MII_KSZ9031_EXT_RGMII_CLOCK_SKEW,
			MII_KSZ9031_MOD_DATA_NO_POST_INC, 0x03FF);

	if (phydev->drv->config)
		phydev->drv->config(phydev);

	return 0;
}
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */


/*  __________________________________________________________________________
 * |                                                                          |
 * |                                    USB                                   |
 * |__________________________________________________________________________|
 */
#ifdef CONFIG_USB_EHCI_MX6

#define USB_OTHERREGS_OFFSET                     0x800
#define UCTRL_PWR_POL                            (1 << 9)

int board_ehci_hcd_init (int port) {
	u32 *usbnc_usb_ctrl;
	struct mxc_ccm_reg *mxc_ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;

	/*  enable clock for HUB  */
	writel( 0x010E0000, &mxc_ccm->ccosr	);

	switch (port) {
	case 0:
		/*
		  * Set daisy chain for otg_pin_id on 6q.
		 *  For 6dl, this bit is reserved.
		 */
		imx_iomux_set_gpr_register(1, 13, 1, 0);
		break;
	case 1:
		break;
	default:
		printf("MXC USB port %d not yet supported\n", port);
		return -EINVAL;
	}

	usbnc_usb_ctrl = (u32 *)(USB_BASE_ADDR + USB_OTHERREGS_OFFSET +
			port * 4);

	setbits_le32(usbnc_usb_ctrl, UCTRL_PWR_POL);

	return 0;
}
#endif  /*  CONFIG_USB_EHCI_MX6  */
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */

/*  __________________________________________________________________________
 * |                                                                          |
 * |                              DISPLAY SETTINGS                            |
 * |__________________________________________________________________________|
 */

#ifdef CONFIG_VIDEO

char *display_name;


static iomux_v3_cfg_t const lvds_pads[] = {
	IOMUX_PADS(PAD_GPIO_4__GPIO1_IO04 | MUX_PAD_CTRL(GPIO_LVDS)),
	IOMUX_PADS(PAD_GPIO_2__GPIO1_IO02 | MUX_PAD_CTRL(GPIO_LVDS)),
};


#define LVDS_BACKLIGHT_GPIO                      IMX_GPIO_NR(4, 29) 
static iomux_v3_cfg_t const lvds_backlight_pads[] = {
	IOMUX_PADS(PAD_DISP0_DAT8__GPIO4_IO29 | MUX_PAD_CTRL(NO_PAD_CTRL)),
};



void setup_iomux_backlight( int en ) {
	SETUP_IOMUX_PADS( lvds_backlight_pads );
	gpio_request( LVDS_BACKLIGHT_GPIO, "lvds backlight" );
	gpio_direction_output ( LVDS_BACKLIGHT_GPIO, !!en );
}


void enable_lvds( struct display_info_t const *dev ) {
	struct iomuxc *iomux = (struct iomuxc *) IOMUXC_BASE_ADDR;
	u32 reg = readl( &iomux->gpr[2] );
	reg |= IOMUXC_GPR2_DATA_WIDTH_CH0_24BIT;
	writel(reg, &iomux->gpr[2]);

	setup_lvds( dev );

	SETUP_IOMUX_PADS(lvds_pads);
	gpio_request( IMX_GPIO_NR(1, 2), "lvds panel on" );
	gpio_request( IMX_GPIO_NR(1, 4), "lvds backlight on" );
	gpio_direction_output( IMX_GPIO_NR(1, 2), 1 );
	gpio_direction_output( IMX_GPIO_NR(1, 4), 1 );
	gpio_free( IMX_GPIO_NR(1, 2) );
	gpio_free( IMX_GPIO_NR(1, 4) );
	setup_iomux_backlight( 1 );
}


void disable_lvds( struct display_info_t const *dev ) {
	struct iomuxc *iomux = (struct iomuxc *)IOMUXC_BASE_ADDR;

	int reg = readl( &iomux->gpr[2] );

	reg &= ~(IOMUXC_GPR2_LVDS_CH0_MODE_MASK |
		 IOMUXC_GPR2_LVDS_CH1_MODE_MASK);

	writel(reg, &iomux->gpr[2]);

	SETUP_IOMUX_PADS(lvds_pads);
	gpio_request( IMX_GPIO_NR(1, 2), "lvds panel on" );
	gpio_request( IMX_GPIO_NR(1, 4), "lvds backlight on" );

	setup_iomux_backlight( 0 );
	gpio_direction_output( IMX_GPIO_NR(1, 2), 0 );
	gpio_direction_output( IMX_GPIO_NR(1, 4), 0 );

	gpio_free( IMX_GPIO_NR(1, 2) );
	gpio_free( IMX_GPIO_NR(1, 4) );
}


#endif
/*  __________________________________________________________________________
 * |__________________________________________________________________________|
 */



#ifdef CONFIG_SECO_BOARD_NAME
char *board_name = CONFIG_SECO_BOARD_NAME;
#else
char *board_name = "Seco SBC-i.MX6 (SBC) - A62";
#endif


/*
 * Do not overwrite the console
 * Use always serial for U-Boot console
 */
int overwrite_console (void) {
	return 1;
}


int board_early_init_f( void ) {

	setup_iomux_uart();

#ifndef CONFIG_SPL_BUILD
#if defined(CONFIG_VIDEO_IPUV3)
	setup_display();
#endif
#endif

	return 0;
}


int board_init( void ) {

	/* address of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

#if defined(CONFIG_DM_REGULATOR)
	regulators_enable_boot_on( false );
#endif

	print_boot_device( );

	return 0;
}

#ifdef CONFIG_CMD_BMODE
static const struct boot_mode board_boot_modes[] = {
	/* 4 bit bus width */
	{"sd3",	 MAKE_CFGVAL(0x40, 0x30, 0x00, 0x00)},
	/* 8 bit bus width */
	{"emmc", MAKE_CFGVAL(0x60, 0x58, 0x00, 0x00)},
	{NULL,	 0},
};
#endif

int board_late_init( void ) {
#ifdef CONFIG_CMD_BMODE
	add_board_boot_modes( board_boot_modes );
#endif

#ifdef CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
	env_set( "board_name", board_name );

	if ( is_mx6dqp( ) )
		env_set( "board_rev", "MX6QP" );
	else if ( is_mx6dq( ) )
		env_set( "board_rev", "MX6Q" );
	else if  (is_mx6sdl( ) )
		env_set( "board_rev", "MX6DL" );
#endif

#ifdef CONFIG_ENV_IS_IN_MMC
	board_late_mmc_env_init( );
#endif

	fdt_set( );
	memory_set( );

	return 0;
}


void  board_custom_boot ( void ) {
}


// int checkboard(void)
// {
// 	puts("Board: MX6-SabreSD\n");
// 	return 0;
// }



#ifdef CONFIG_SPL_BUILD

#include <spl.h>
#include <linux/libfdt.h>

#include "ddr_config_4x256.h"
#include "ddr_config_4x256.h"


#ifdef CONFIG_SPL_OS_BOOT
int spl_start_uboot( void ) {
	return 0;
}
#endif


static void spl_dram_init( void ) {

#if defined( CONFIG_SECOMX6_1GB_4x256 )
	
	if ( is_mx6dq( ) ) {
			ddr_init( mx6qd_64bit_dcd_table, ARRAY_SIZE( mx6qd_64bit_dcd_table ) );
			ddr_init( mx6qd_4x256_dcd_table, ARRAY_SIZE( mx6qd_4x256_dcd_table ) );
	} else if ( is_mx6dl( ) ) {
			ddr_init( mx6dl_64bit_dcd_table, ARRAY_SIZE( mx6dl_64bit_dcd_table ) );
			ddr_init( mx6dl_4x256_dcd_table, ARRAY_SIZE( mx6dl_4x256_dcd_table ) );
	} else if ( is_mx6solo( ) ) {
	}
#endif

}


void board_init_f( ulong dummy ) {
	/* DDR initialization */
	spl_dram_init();

	/* setup AIPS and disable watchdog */
	arch_cpu_init();

	ccgr_init();
	gpr_init();

	/* iomux and setup of i2c */
	board_early_init_f();


#ifdef CONFIG_APX_WATCHDOG
	setup_iomux_apx_watchdog( );
#else
	disable_apx_watchdog( );
#endif

	/* setup GP timer */
	timer_init();

	/* UART clocks enabled and gd valid - init serial console */
	preloader_console_init();

	/* Clear the BSS. */
	memset(__bss_start, 0, __bss_end - __bss_start);

	/* load/boot image from boot device */
	board_init_r(NULL, 0);
}

void reset_cpu (ulong addr){
}

#endif

#ifdef CONFIG_SPL_LOAD_FIT
int board_fit_config_name_match(const char *name)
{
	if (is_mx6dq()) {
		if (!strcmp(name, "imx6q-seco_SBC_A62"))
			return 0;
	} else if ( is_mx6dl( ) || is_mx6solo( ) ) {
		if (!strcmp(name, "imx6dl-seco_SBC_A62"))
			return 0;
	}

	return -1;
}
#endif
