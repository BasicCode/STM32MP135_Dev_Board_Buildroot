#!/usr/bin/env bash

main()
{
	local ATFBIN="tf-a-stm32mp135_dev_board-tf-a.stm32"
	local GENIMAGE_CFG="$(mktemp --suffix genimage.cfg)"
	local GENIMAGE_TMP="${BUILD_DIR}/genimage.tmp"

	sed -e "s/%ATFBIN%/${ATFBIN}/" \
		${BR2_EXTERNAL_STM32MP151_Dev_Board_Buildroot_PATH}/board/stm32mp151_dev_board/genimage.cfg.template > ${GENIMAGE_CFG}

	support/scripts/genimage.sh -c ${GENIMAGE_CFG}

	rm -f ${GENIMAGE_CFG}

	exit $?
}

main $@
