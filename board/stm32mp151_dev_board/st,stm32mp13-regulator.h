/*
 * NOTE: For whatever reason this file (or any 'regulator' files) isn't included in u-boot.
 * Please copy it to the u-boot/include/dt-bindings/regulator folder.
*/

/* SPDX-License-Identifier: (GPL-2.0 OR BSD-2-Clause) */
/*
 * Copyright (C) 2022, STMicroelectronics - All Rights Reserved
 */

#ifndef __DT_BINDINGS_REGULATOR_ST_STM32MP13_REGULATOR_H
#define __DT_BINDINGS_REGULATOR_ST_STM32MP13_REGULATOR_H

/* SCMI voltage domains identifiers */

/* SOC Internal regulators */
#define VOLTD_SCMI_REG11		0
#define VOLTD_SCMI_REG18		1
#define VOLTD_SCMI_USB33		2
#define VOLTD_SCMI_SDMMC1_IO		3
#define VOLTD_SCMI_SDMMC2_IO		4
#define VOLTD_SCMI_VREFBUF		5

/* STPMIC1 regulators */
#define VOLTD_SCMI_STPMIC1_BUCK1	6
#define VOLTD_SCMI_STPMIC1_BUCK2	7
#define VOLTD_SCMI_STPMIC1_BUCK3	8
#define VOLTD_SCMI_STPMIC1_BUCK4	9
#define VOLTD_SCMI_STPMIC1_LDO1	        10
#define VOLTD_SCMI_STPMIC1_LDO2	        11
#define VOLTD_SCMI_STPMIC1_LDO3	        12
#define VOLTD_SCMI_STPMIC1_LDO4	        13
#define VOLTD_SCMI_STPMIC1_LDO5	        14
#define VOLTD_SCMI_STPMIC1_LDO6	        15
#define VOLTD_SCMI_STPMIC1_VREFDDR	16
#define VOLTD_SCMI_STPMIC1_BOOST	17
#define VOLTD_SCMI_STPMIC1_PWR_SW1	18
#define VOLTD_SCMI_STPMIC1_PWR_SW2	19

/* External regulators */
#define VOLTD_SCMI_REGU0		20
#define VOLTD_SCMI_REGU1		21
#define VOLTD_SCMI_REGU2		22
#define VOLTD_SCMI_REGU3		23
#define VOLTD_SCMI_REGU4		24

#endif /*__DT_BINDINGS_REGULATOR_ST_STM32MP13_REGULATOR_H */