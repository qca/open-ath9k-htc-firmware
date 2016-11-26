##
 # Copyright (c) 2013 Qualcomm Atheros, Inc.
 #
 # All rights reserved.
 #
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted (subject to the limitations in the
 # disclaimer below) provided that the following conditions are met:
 #
 #  * Redistributions of source code must retain the above copyright
 #    notice, this list of conditions and the following disclaimer.
 #
 #  * Redistributions in binary form must reproduce the above copyright
 #    notice, this list of conditions and the following disclaimer in the
 #    documentation and/or other materials provided with the
 #    distribution.
 #
 #  * Neither the name of Qualcomm Atheros nor the names of its
 #    contributors may be used to endorse or promote products derived
 #    from this software without specific prior written permission.
 #
 # NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
 # GRANTED BY THIS LICENSE.  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
 # HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 # WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 # MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 # DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 # LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 # CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 # SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 # BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 # WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 # OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 # IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ##

SET(CMAKE_C_COMPILER_WORKS 1)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ath9k_firmware NONE)
ENABLE_LANGUAGE(C)

ADD_DEFINITIONS(
	-g -Os
	-Wunused-label
	-Wunused-variable
	-Wunused-value
	-Wpointer-arith
	-nostdlib
	-Wundef
	-Wunused-but-set-variable
	-Wmaybe-uninitialized
)
ADD_DEFINITIONS(
	-D_RAM_
	-DBIG_ENDIAN -D_BYTE_ORDER=_BIG_ENDIAN
	-D__XTENSA__
	-DFUSION_USB_FW -DRX_SCATTER
	-DFUSION_USB_ENABLE_TX_STREAM -DFUSION_USB_ENABLE_RX_STREAM
	-DATH_ENABLE_CABQ
)

SET(SOURCES
	magpie_fw_dev/target/init/app_start.c
	magpie_fw_dev/target/init/init.c
	magpie_fw_dev/target/init/magpie.c
	magpie_fw_dev/target/adf/adf_nbuf.c
	magpie_fw_dev/target/adf/adf_net.c
	magpie_fw_dev/target/adf/adf_os_defer_pvt.c
	magpie_fw_dev/target/adf/adf_os_dma.c
	magpie_fw_dev/target/adf/adf_os_irq_pvt.c
	magpie_fw_dev/target/adf/adf_os_timer.c
	magpie_fw_dev/target/buf_pool/buf_pool_static.c
	magpie_fw_dev/target/cmnos/dbg_api.c
	magpie_fw_dev/target/cmnos/cmnos_sflash.c
	magpie_fw_dev/target/hif/usb_api_main_patch.c
	wlan/ah.c
	wlan/ah_osdep.c
	wlan/ar5416Phy.c
	wlan/ar5416_hw.c
	wlan/ar5416_phy.c
	wlan/ratectrl_11n_ln.c
	wlan/if_owl.c
	wlan/if_ath.c
	wlan/if_ath_pci.c
	magpie_fw_dev/target/wlan/wlan_pci.c
	wlan/ieee80211_output.c
	magpie_fw_dev/target/htc/htc.c
	magpie_fw_dev/target/wmi/wmi_svc.c
)

IF(TARGET_K2)
	SET(SOURCES ${SOURCES}
		magpie_fw_dev/target/hif/k2_HIF_usb_patch.c
		magpie_fw_dev/target/hif/usb_api_k2_patch.c
	)
	SET(LIBS ${LIBS} hif)
	ADD_DEFINITIONS(-DPROJECT_K2)
	SET(PLATFORM_NAME k2)
	SET(SOURCES ${SOURCES}
		magpie_fw_dev/target/cmnos/k2_cmnos_clock_patch.c
		magpie_fw_dev/target/cmnos/k2_fw_cmnos_printf.c
	)
	SET(FIRMWARE_NAME htc_9271.fw)
ELSEIF(TARGET_MAGPIE)
	ADD_DEFINITIONS(
		-DROM_VER_1_1 -D_ROM_1_1_
		-DPROJECT_MAGPIE
		-DMAGPIE_MERLIN
	)
	SET(SOURCES ${SOURCES}
		magpie_fw_dev/target/hif/usb_api_magpie_patch.c
		magpie_fw_dev/target/rompatch/cmnos_clock_patch.c
		magpie_fw_dev/target/rompatch/HIF_usb_patch.c
	)
	INCLUDE_DIRECTORIES(
		${CMAKE_SOURCE_DIR}/magpie_fw_dev/target/adf
	)
	SET(PLATFORM_NAME magpie)
	SET(FIRMWARE_NAME htc_7010.fw)
ELSE()
	MESSAGE(FATAL_ERROR "Unknown target name")
ENDIF()

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/magpie_fw_dev/target/
	${CMAKE_SOURCE_DIR}/magpie_fw_dev/target/inc
	${CMAKE_SOURCE_DIR}/magpie_fw_dev/target/inc/xtensa-elf
	${CMAKE_SOURCE_DIR}/magpie_fw_dev/target/inc/${PLATFORM_NAME}
	${CMAKE_SOURCE_DIR}/magpie_fw_dev/target/inc/${PLATFORM_NAME}/${PLATFORM_NAME}
	${CMAKE_SOURCE_DIR}/wlan
	${CMAKE_SOURCE_DIR}/wlan/include
	${CMAKE_SOURCE_DIR}/wlan/include/${PLATFORM_NAME}
	${CMAKE_SOURCE_DIR}/magpie_fw_dev/target/wlan
	${CMAKE_SOURCE_DIR}/magpie_fw_dev/target/cmnos
	${CMAKE_SOURCE_DIR}/magpie_fw_dev/target/wmi
	${CMAKE_SOURCE_DIR}/magpie_fw_dev/target/adf
	${CMAKE_SOURCE_DIR}/magpie_fw_dev/target/buf_pool
	${CMAKE_SOURCE_DIR}/magpie_fw_dev/target/htc
)

ADD_LIBRARY(firmware ${SOURCES})

ADD_CUSTOM_COMMAND(
	OUTPUT fw.elf
	DEPENDS firmware
	COMMAND ${CMAKE_C_COMPILER}
		-nostdlib
		-fno-lto
		-Wl,--start-group
		${CMAKE_BINARY_DIR}/libfirmware.a
		-Wl,--end-group
		-Wl,-T,${CMAKE_SOURCE_DIR}/ram-${PLATFORM_NAME}.ld
		-Wl,-T,${CMAKE_SOURCE_DIR}/rom-addrs-${PLATFORM_NAME}.ld
		-o fw.elf
)
ADD_CUSTOM_COMMAND(
	OUTPUT fw.bin
	DEPENDS fw.elf
	COMMAND ${CMAKE_OBJCOPY}
		--change-section-lma .boot-0x400000
		--change-section-vma .boot-0x400000
		--change-section-lma .text-0x400000
		--change-section-vma .text-0x400000
		-O binary fw.elf fw.bin
)
ADD_CUSTOM_COMMAND(
	OUTPUT ${FIRMWARE_NAME}
	DEPENDS fw.bin
	COMMAND ${CMAKE_SOURCE_DIR}/firmware-crc.pl < fw.bin > ${FIRMWARE_NAME}
)
ADD_CUSTOM_TARGET(gen-firmware ALL DEPENDS ${FIRMWARE_NAME})
