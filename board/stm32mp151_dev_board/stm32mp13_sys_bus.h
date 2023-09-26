/*
 * NOTE: This is a copy from the STLinux source code because u-Boot
 * is not up to date. 
 * It needs to be copied to the folder:
 * u-boot/include/dt-bindings/bus
 * This file should be removed when the u-boot version catches up with
 * Linux.
 */

/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause) */
/*
 * Copyright (C) STMicroelectronics 2022 - All Rights Reserved
 */
#ifndef _DT_BINDINGS_BUS_STM32MP13_SYS_BUS_H
#define _DT_BINDINGS_BUS_STM32MP13_SYS_BUS_H

/* ETZPC IDs */
#define STM32MP1_ETZPC_VREFBUF_ID	0
#define STM32MP1_ETZPC_LPTIM2_ID	1
#define STM32MP1_ETZPC_LPTIM3_ID	2
#define STM32MP1_ETZPC_LTDC_ID		3
#define STM32MP1_ETZPC_DCMIPP_ID	4
#define STM32MP1_ETZPC_USBPHYCTRL_ID	5
#define STM32MP1_ETZPC_DDRCTRLPHY_ID	6
/* IDs 7-11 reserved */
#define STM32MP1_ETZPC_IWDG1_ID		12
#define STM32MP1_ETZPC_STGENC_ID	13
/* IDs 14-15 reserved */
#define STM32MP1_ETZPC_USART1_ID	16
#define STM32MP1_ETZPC_USART2_ID	17
#define STM32MP1_ETZPC_SPI4_ID		18
#define STM32MP1_ETZPC_SPI5_ID		19
#define STM32MP1_ETZPC_I2C3_ID		20
#define STM32MP1_ETZPC_I2C4_ID		21
#define STM32MP1_ETZPC_I2C5_ID		22
#define STM32MP1_ETZPC_TIM12_ID		23
#define STM32MP1_ETZPC_TIM13_ID		24
#define STM32MP1_ETZPC_TIM14_ID		25
#define STM32MP1_ETZPC_TIM15_ID		26
#define STM32MP1_ETZPC_TIM16_ID		27
#define STM32MP1_ETZPC_TIM17_ID		28
/* IDs 29-31 reserved */
#define STM32MP1_ETZPC_ADC1_ID		32
#define STM32MP1_ETZPC_ADC2_ID		33
#define STM32MP1_ETZPC_OTG_ID		34
/* IDs 35-39 reserved */
#define STM32MP1_ETZPC_TSC_ID		37
#define STM32MP1_ETZPC_RNG_ID		40
#define STM32MP1_ETZPC_HASH_ID		41
#define STM32MP1_ETZPC_CRYP_ID		42
#define STM32MP1_ETZPC_SAES_ID		43
#define STM32MP1_ETZPC_PKA_ID		44
#define STM32MP1_ETZPC_BKPSRAM_ID	45
/* IDs 46-47 reserved */
#define STM32MP1_ETZPC_ETH1_ID		48
#define STM32MP1_ETZPC_ETH2_ID		49
#define STM32MP1_ETZPC_SDMMC1_ID	50
#define STM32MP1_ETZPC_SDMMC2_ID	51
/* ID 52 reserved */
#define STM32MP1_ETZPC_MCE_ID		53
#define STM32MP1_ETZPC_FMC_ID		54
#define STM32MP1_ETZPC_QSPI_ID		55
/* IDs 56-59 reserved */
#define STM32MP1_ETZPC_SRAM1_ID		60
#define STM32MP1_ETZPC_SRAM2_ID		61
#define STM32MP1_ETZPC_SRAM3_ID		62
/* ID 63 reserved */

#endif /* _DT_BINDINGS_BUS_STM32MP13_SYS_BUS_H */
