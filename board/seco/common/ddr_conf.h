#ifndef _SECO_DDR_CONFIG_H_
#define _SECO_DDR_CONFIG_H_

static inline void ddr_init( int *table, int size ) {
	int i;
	for ( i = 0; i < size / 2 ; i++ )
		writel( table[2 * i + 1], table[2 * i] );
}


static inline void ccgr_init( void ) {
#ifdef CONFIG_MX6SX
	struct mxc_ccm_reg *ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;

	writel(0xFFFFFFFF, &ccm->CCGR0);
	writel(0xFFFFFFFF, &ccm->CCGR1);
	writel(0xFFFFFFFF, &ccm->CCGR2);
	writel(0xFFFFFFFF, &ccm->CCGR3);
	writel(0xFFFFFFFF, &ccm->CCGR4);
	writel(0xFFFFFFFF, &ccm->CCGR5);
	writel(0xFFFFFFFF, &ccm->CCGR6);
	writel(0xFFFFFFFF, &ccm->CCGR7);
#else
	struct mxc_ccm_reg *ccm = (struct mxc_ccm_reg *)CCM_BASE_ADDR;

	writel( 0x00C03F3F, &ccm->CCGR0 );
	writel( 0x0030FFFF, &ccm->CCGR1 );
	writel( 0x0FFFC000, &ccm->CCGR2 );
	writel( 0x3FF00000, &ccm->CCGR3 );
	writel( 0x00FFF300, &ccm->CCGR4 );
	writel( 0x0F0000C3, &ccm->CCGR5 );
	writel( 0x000003FF, &ccm->CCGR6 );
#endif
}


#ifndef CONFIG_MX6SX
#if defined( CONFIG_SECOMX6_1GB_4x256 ) || defined( CONFIG_SECOMX6_2GB_4x512 ) || \
	defined( CONFIG_SECOMX6_4GB_8x512 ) || defined( CONFIG_SECOMX6_1GB_2x512 ) || \
	defined( CONFIG_SECOMX6_DDR_DYNAMIC_SIZE )

static int mx6qd_64bit_dcd_table[] = {
	/* DDR IO TYPE */
	0x020e0798, 0x000C0000,  // MX6_IOM_GRP_DDR_TYPE
	0x020e0758, 0x00000000,  // MX6_IOM_GRP_DDRPKE
	/* CLOCK */
	0x020e0588, 0x00000030,  // MX6_IOM_DRAM_SDCLK_0
	0x020e0594, 0x00000030,  // MX6_IOM_DRAM_SDCLK_1
	/* ADDRESS */
	0x020e056c, 0x00000030,  // MX6_IOM_DRAM_CAS
	0x020e0578, 0x00000030,  // MX6_IOM_DRAM_RAS
	0x020e074c, 0x00000030,  // MX6_IOM_GRP_ADDDS
	/* CONTROL */
	0x020e057c, 0x00000030,  // MX6_IOM_DRAM_RESET
	0x020e058c, 0x00000000,  // MX6_IOM_DRAM_SDBA2
	0x020e059c, 0x00000030,  // MX6_IOM_DRAM_SDODT0
	0x020e05a0, 0x00000030,  // MX6_IOM_DRAM_SDODT1
	0x020e078c, 0x00000030,  // MX6_IOM_GRP_CTLDS
	/* DATA STROBE */
	0x020e0750, 0x00020000,  // MX6_IOM_DDRMODE_CTL
	0x020e05a8, 0x00000030,  // MX6_IOM_DRAM_SDQS0
	0x020e05b0, 0x00000030,  // MX6_IOM_DRAM_SDQS1
	0x020e0524, 0x00000030,  // MX6_IOM_DRAM_SDQS2
	0x020e051c, 0x00000030,  // MX6_IOM_DRAM_SDQS3
	0x020e0518, 0x00000030,  // MX6_IOM_DRAM_SDQS4
	0x020e050c, 0x00000030,  // MX6_IOM_DRAM_SDQS5
	0x020e05b8, 0x00000030,  // MX6_IOM_DRAM_SDQS6
	0x020e05c0, 0x00000030,  // MX6_IOM_DRAM_SDQS7
	/* DATA */
	0x020e0774, 0x00020000,  // MX6_IOM_GRP_DDRMODE
	0x020e0784, 0x00000030,  // MX6_IOM_GRP_B0DS
	0x020e0788, 0x00000030,  // MX6_IOM_GRP_B1DS
	0x020e0794, 0x00000030,  // MX6_IOM_GRP_B2DS
	0x020e079c, 0x00000030,  // MX6_IOM_GRP_B3DS
	0x020e07a0, 0x00000030,  // MX6_IOM_GRP_B4DS
	0x020e07a4, 0x00000030,  // MX6_IOM_GRP_B5DS
	0x020e07a8, 0x00000030,  // MX6_IOM_GRP_B6DS
	0x020e0748, 0x00000030,  // MX6_IOM_GRP_B7DS
	0x020e05ac, 0x00000030,  // MX6_IOM_DRAM_DQM0
	0x020e05b4, 0x00000030,  // MX6_IOM_DRAM_DQM1
	0x020e0528, 0x00000030,  // MX6_IOM_DRAM_DQM2
	0x020e0520, 0x00000030,  // MX6_IOM_DRAM_DQM3
	0x020e0514, 0x00000030,  // MX6_IOM_DRAM_DQM4
	0x020e0510, 0x00000030,  // MX6_IOM_DRAM_DQM5
	0x020e05bc, 0x00000030,  // MX6_IOM_DRAM_DQM6
	0x020e05c4, 0x00000030,  // MX6_IOM_DRAM_DQM7
	/* DDR3 SETTINGS: Read Data Bit Delay */
	0x021b081c, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY0DL
	0x021b0820, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY1DL
	0x021b0824, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY2DL
	0x021b0828, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY3DL
	0x021b481c, 0x33333333,  // MX6_MMDC_P1_MPRDDQBY0DL
	0x021b4820, 0x33333333,  // MX6_MMDC_P1_MPRDDQBY1DL
	0x021b4824, 0x33333333,  // MX6_MMDC_P1_MPRDDQBY2DL
	0x021b4828, 0x33333333,  // MX6_MMDC_P1_MPRDDQBY3DL
};



static int mx6dl_64bit_dcd_table[] = {
	/* DDR IO TYPE */
	0x020e0774, 0x000C0000,  // MX6_IOM_GRP_DDR_TYPE
	0x020e0754, 0x00000000,  // MX6_IOM_GRP_DDRPKE
	/* CLOCK */
	0x020e04ac, 0x00000028,  // MX6_IOM_DRAM_SDCLK_0
	0x020e04b0, 0x00000028,  // MX6_IOM_DRAM_SDCLK_1
	/* ADDRESS */
	0x020e0464, 0x00000028,  // MX6_IOM_DRAM_CAS
	0x020e0490, 0x00000028,  // MX6_IOM_DRAM_RAS
	0x020e074c, 0x00000028,  // MX6_IOM_GRP_ADDDS
	/* CONTROL */
	0x020e0494, 0x00000028,  // MX6_IOM_DRAM_RESET
	0x020e04a0, 0x00000000,  // MX6_IOM_DRAM_SDBA2
	0x020e04b4, 0x00000028,  // MX6_IOM_DRAM_SDODT0
	0x020e04b8, 0x00000028,  // MX6_IOM_DRAM_SDODT1
	0x020e076c, 0x00000028,  // MX6_IOM_GRP_CTLDS
	/* DATA STROBE */
	0x020e0750, 0x00020000,  // MX6_IOM_DDRMODE_CTL
	0x020e04bc, 0x00000028,  // MX6_IOM_DRAM_SDQS0
	0x020e04c0, 0x00000028,  // MX6_IOM_DRAM_SDQS1
	0x020e04c4, 0x00000028,  // MX6_IOM_DRAM_SDQS2
	0x020e04c8, 0x00000028,  // MX6_IOM_DRAM_SDQS3
	0x020e04cc, 0x00000028,  // MX6_IOM_DRAM_SDQS4
	0x020e04d0, 0x00000028,  // MX6_IOM_DRAM_SDQS5
	0x020e04d4, 0x00000028,  // MX6_IOM_DRAM_SDQS6
	0x020e04d8, 0x00000028,  // MX6_IOM_DRAM_SDQS7
	/* DATA */
	0x020e0760, 0x00020000,  // MX6_IOM_GRP_DDRMODE
	0x020e0764, 0x00000028,  // MX6_IOM_GRP_B0DS
	0x020e0770, 0x00000028,  // MX6_IOM_GRP_B1DS
	0x020e0778, 0x00000028,  // MX6_IOM_GRP_B2DS
	0x020e077c, 0x00000028,  // MX6_IOM_GRP_B3DS
	0x020e0780, 0x00000028,  // MX6_IOM_GRP_B4DS
	0x020e0784, 0x00000028,  // MX6_IOM_GRP_B5DS
	0x020e078c, 0x00000028,  // MX6_IOM_GRP_B6DS
	0x020e0748, 0x00000028,  // MX6_IOM_GRP_B7DS
	0x020e0470, 0x00000028,  // MX6_IOM_DRAM_DQM0
	0x020e0474, 0x00000028,  // MX6_IOM_DRAM_DQM1
	0x020e0478, 0x00000028,  // MX6_IOM_DRAM_DQM2
	0x020e047c, 0x00000028,  // MX6_IOM_DRAM_DQM3
	0x020e0480, 0x00000028,  // MX6_IOM_DRAM_DQM4
	0x020e0484, 0x00000028,  // MX6_IOM_DRAM_DQM5
	0x020e0488, 0x00000028,  // MX6_IOM_DRAM_DQM6
	0x020e048c, 0x00000028,  // MX6_IOM_DRAM_DQM7
	/* DDR3 SETTINGS: Read Data Bit Delay */
	0x021b081c, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY0DL
	0x021b0820, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY1DL
	0x021b0824, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY2DL
	0x021b0828, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY3DL
	0x021b481c, 0x33333333,  // MX6_MMDC_P1_MPRDDQBY0DL
	0x021b4820, 0x33333333,  // MX6_MMDC_P1_MPRDDQBY1DL
	0x021b4824, 0x33333333,  // MX6_MMDC_P1_MPRDDQBY2DL
	0x021b4828, 0x33333333,  // MX6_MMDC_P1_MPRDDQBY3DL
};


#endif


#if defined( CONFIG_SECOMX6_512MB_2x256 ) || defined( CONFIG_SECOMX6_DDR_DYNAMIC_SIZE )


static int mx6qd_32bit_dcd_table[] = {	/* DDR IO TYPE */
	0x020e0798, 0x000C0000,  // MX6_IOM_GRP_DDR_TYPE
	0x020e0758, 0x00000000,  // MX6_IOM_GRP_DDRPKE
	/* CLOCK */
	0x020e0588, 0x00000030,  // MX6_IOM_DRAM_SDCLK_0
	0x020e0594, 0x00000030,  // MX6_IOM_DRAM_SDCLK_1
	/* ADDRESS */
	0x020e056c, 0x00000030,  // MX6_IOM_DRAM_CAS
	0x020e0578, 0x00000030,  // MX6_IOM_DRAM_RAS
	0x020e074c, 0x00000030,  // MX6_IOM_GRP_ADDDS
	/* CONTROL */
	0x020e057c, 0x00000030,  // MX6_IOM_DRAM_RESET
	0x020e058c, 0x00000000,  // MX6_IOM_DRAM_SDBA2
	0x020e059c, 0x00000030,  // MX6_IOM_DRAM_SDODT0
	0x020e05a0, 0x00000030,  // MX6_IOM_DRAM_SDODT1
	/* DATA STROBE */
	0x020e0750, 0x00020000,  // MX6_IOM_DDRMODE_CTL
	0x020e05a8, 0x00000030,  // MX6_IOM_DRAM_SDQS0
	0x020e05b0, 0x00000030,  // MX6_IOM_DRAM_SDQS1
	0x020e0524, 0x00000030,  // MX6_IOM_DRAM_SDQS2
	0x020e051c, 0x00000030,  // MX6_IOM_DRAM_SDQS3
	/* DATA */
	0x020e0774, 0x00020000,  // MX6_IOM_GRP_DDRMODE
	0x020e0784, 0x00000030,  // MX6_IOM_GRP_B0DS
	0x020e0788, 0x00000030,  // MX6_IOM_GRP_B1DS
	0x020e0794, 0x00000030,  // MX6_IOM_GRP_B2DS
	0x020e079c, 0x00000030,  // MX6_IOM_GRP_B3DS
	0x020e05ac, 0x00000030,  // MX6_IOM_DRAM_DQM0
	0x020e05b4, 0x00000030,  // MX6_IOM_DRAM_DQM1
	0x020e0528, 0x00000030,  // MX6_IOM_DRAM_DQM2
	0x020e0520, 0x00000030,  // MX6_IOM_DRAM_DQM3
	/* DDR3 SETTINGS: Read Data Bit Delay */
	0x021b081c, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY0DL
	0x021b0820, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY1DL
	0x021b0824, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY2DL
	0x021b0828, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY3DL
	0x021b481c, 0x33333333,  // MX6_MMDC_P1_MPRDDQBY0DL
	0x021b4820, 0x33333333,  // MX6_MMDC_P1_MPRDDQBY1DL
	0x021b4824, 0x33333333,  // MX6_MMDC_P1_MPRDDQBY2DL
	0x021b4828, 0x33333333,  // MX6_MMDC_P1_MPRDDQBY3DL
};



static int mx6dl_32bit_dcd_table[] = {
		/* DDR IO TYPE */
	0x020e0774, 0x000C0000,  // MX6_IOM_GRP_DDR_TYPE
	0x020e0754, 0x00000000,  // MX6_IOM_GRP_DDRPKE
	/* CLOCK */
	0x020e04ac, 0x00000028,  // MX6_IOM_DRAM_SDCLK_0
	0x020e04b0, 0x00000028,  // MX6_IOM_DRAM_SDCLK_1
	/* ADDRESS */
	0x020e0464, 0x00000028,  // MX6_IOM_DRAM_CAS
	0x020e0490, 0x00000028,  // MX6_IOM_DRAM_RAS
	0x020e074c, 0x00000028,  // MX6_IOM_GRP_ADDDS
	/* CONTROL */
	0x020e076c, 0x00000030,  // MX6_IOM_GRP_CTLDS
	0x020e0494, 0x00000028,  // MX6_IOM_DRAM_RESET
	0x020e04a0, 0x00000000,  // MX6_IOM_DRAM_SDBA2
	0x020e04b4, 0x00000028,  // MX6_IOM_DRAM_SDODT0
	0x020e04b8, 0x00000028,  // MX6_IOM_DRAM_SDODT1
	/* DATA STROBE */
	0x020e0750, 0x00020000,  // MX6_IOM_DDRMODE_CTL
	0x020e04bc, 0x00000028,  // MX6_IOM_DRAM_SDQS0
	0x020e04c0, 0x00000028,  // MX6_IOM_DRAM_SDQS1
	0x020e04c4, 0x00000028,  // MX6_IOM_DRAM_SDQS2
	0x020e04c8, 0x00000028,  // MX6_IOM_DRAM_SDQS3
	/* DATA */
	0x020e0760, 0x00020000,  // MX6_IOM_GRP_DDRMODE
	0x020e0764, 0x00000028,  // MX6_IOM_GRP_B0DS
	0x020e0770, 0x00000028,  // MX6_IOM_GRP_B1DS
	0x020e0778, 0x00000028,  // MX6_IOM_GRP_B2DS
	0x020e077c, 0x00000028,  // MX6_IOM_GRP_B3DS
	0x020e0470, 0x00000028,  // MX6_IOM_DRAM_DQM0
	0x020e0474, 0x00000028,  // MX6_IOM_DRAM_DQM1
	0x020e0478, 0x00000028,  // MX6_IOM_DRAM_DQM2
	0x020e047c, 0x00000028,  // MX6_IOM_DRAM_DQM3
	/* DDR3 SETTINGS: Read Data Bit Delay */
	0x021b081c, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY0DL
	0x021b0820, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY1DL
	0x021b0824, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY2DL
	0x021b0828, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY3DL
};



static int mx6solo_32bit_dcd_table[] = {
		/* DDR IO TYPE */
	0x020e0774, 0x000C0000,  // MX6_IOM_GRP_DDR_TYPE
	0x020e0754, 0x00000000,  // MX6_IOM_GRP_DDRPKE
	/* CLOCK */
	0x020e04ac, 0x00000028,  // MX6_IOM_DRAM_SDCLK_0
	0x020e04b0, 0x00000028,  // MX6_IOM_DRAM_SDCLK_1
	/* ADDRESS */
	0x020e0464, 0x00000028,  // MX6_IOM_DRAM_CAS
	0x020e0490, 0x00000028,  // MX6_IOM_DRAM_RAS
	0x020e074c, 0x00000028,  // MX6_IOM_GRP_ADDDS
	/* CONTROL */
	0x020e0494, 0x00000028,  // MX6_IOM_DRAM_RESET
	0x020e04a0, 0x00000000,  // MX6_IOM_DRAM_SDBA2
	0x020e04b4, 0x00000028,  // MX6_IOM_DRAM_SDODT0
	0x020e04b8, 0x00000028,  // MX6_IOM_DRAM_SDODT1
	0x020e076c, 0x00000028,  // MX6_IOM_GRP_CTLDS
	/* DATA STROBE */
	0x020e0750, 0x00020000,  // MX6_IOM_DDRMODE_CTL
	0x020e04bc, 0x00000028,  // MX6_IOM_DRAM_SDQS0
	0x020e04c0, 0x00000028,  // MX6_IOM_DRAM_SDQS1
	0x020e04c4, 0x00000028,  // MX6_IOM_DRAM_SDQS2
	0x020e04c8, 0x00000028,  // MX6_IOM_DRAM_SDQS3
	/* DATA */
	0x020e0760, 0x00020000,  // MX6_IOM_GRP_DDRMODE
	0x020e0764, 0x00000028,  // MX6_IOM_GRP_B0DS
	0x020e0770, 0x00000028,  // MX6_IOM_GRP_B1DS
	0x020e0778, 0x00000028,  // MX6_IOM_GRP_B2DS
	0x020e077c, 0x00000028,  // MX6_IOM_GRP_B3DS
	0x020e0470, 0x00000028,  // MX6_IOM_DRAM_DQM0
	0x020e0474, 0x00000028,  // MX6_IOM_DRAM_DQM1
	0x020e0478, 0x00000028,  // MX6_IOM_DRAM_DQM2
	0x020e047c, 0x00000028,  // MX6_IOM_DRAM_DQM3
	/* DDR3 SETTINGS: Read Data Bit Delay */
	0x021b081c, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY0DL
	0x021b0820, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY1DL
	0x021b0824, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY2DL
	0x021b0828, 0x33333333,  // MX6_MMDC_P0_MPRDDQBY3DL
};


#endif
#endif	/* CONFIG_MX6SX */



#endif    /*  _SECO_DDR_CONFIG_H_  */