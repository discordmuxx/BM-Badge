SDK_TARGET ?= $(PRJ_ROOT)/output/libnordic-sdk.a
SRC_ROOT := $(SDK_ROOT)

SDK_SRCS := \
	$(SDK_ROOT)/components/ble/ble_advertising/ble_advertising.c \
	$(SDK_ROOT)/components/ble/ble_db_discovery/ble_db_discovery.c \
	$(SDK_ROOT)/components/ble/ble_services/ble_lbs/ble_lbs.c \
	$(SDK_ROOT)/components/ble/common/ble_advdata.c \
	$(SDK_ROOT)/components/ble/common/ble_conn_params.c \
	$(SDK_ROOT)/components/ble/common/ble_conn_state.c \
	$(SDK_ROOT)/components/ble/common/ble_srv_common.c \
	$(SDK_ROOT)/components/ble/nrf_ble_gatt/nrf_ble_gatt.c \
	$(SDK_ROOT)/components/ble/nrf_ble_qwr/nrf_ble_qwr.c \
	$(SDK_ROOT)/components/ble/peer_manager/gatt_cache_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/gatts_cache_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/id_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_data_storage.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_database.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_id.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/pm_buffer.c \
	$(SDK_ROOT)/components/ble/peer_manager/security_dispatcher.c \
	$(SDK_ROOT)/components/ble/peer_manager/security_manager.c \
	$(SDK_ROOT)/components/boards/boards.c \
	$(SDK_ROOT)/components/libraries/atomic/nrf_atomic.c \
	$(SDK_ROOT)/components/libraries/atomic_fifo/nrf_atfifo.c \
	$(SDK_ROOT)/components/libraries/atomic_flags/nrf_atflags.c \
	$(SDK_ROOT)/components/libraries/balloc/nrf_balloc.c \
	$(SDK_ROOT)/components/libraries/block_dev/sdc/nrf_block_dev_sdc.c \
	$(SDK_ROOT)/components/libraries/bsp/bsp.c \
	$(SDK_ROOT)/components/libraries/button/app_button.c \
	$(SDK_ROOT)/components/libraries/experimental_section_vars/nrf_section_iter.c \
	$(SDK_ROOT)/components/libraries/fds/fds.c \
	$(SDK_ROOT)/components/libraries/fifo/app_fifo.c \
	$(SDK_ROOT)/components/libraries/fstorage/nrf_fstorage.c \
	$(SDK_ROOT)/components/libraries/fstorage/nrf_fstorage_sd.c \
	$(SDK_ROOT)/components/libraries/hardfault/hardfault_implementation.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_rtt.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_serial.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_uart.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_default_backends.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_str_formatter.c \
	$(SDK_ROOT)/components/libraries/memobj/nrf_memobj.c \
	$(SDK_ROOT)/components/libraries/pwm/app_pwm.c \
	$(SDK_ROOT)/components/libraries/pwr_mgmt/nrf_pwr_mgmt.c \
	$(SDK_ROOT)/components/libraries/queue/nrf_queue.c \
	$(SDK_ROOT)/components/libraries/ringbuf/nrf_ringbuf.c \
	$(SDK_ROOT)/components/libraries/scheduler/app_scheduler.c \
	$(SDK_ROOT)/components/libraries/sdcard/app_sdcard.c \
	$(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c \
	$(SDK_ROOT)/components/libraries/timer/app_timer.c \
	$(SDK_ROOT)/components/libraries/uart/app_uart_fifo.c \
	$(SDK_ROOT)/components/libraries/uart/retarget.c \
	$(SDK_ROOT)/components/libraries/usbd/app_usbd.c \
	$(SDK_ROOT)/components/libraries/usbd/app_usbd_core.c \
	$(SDK_ROOT)/components/libraries/usbd/app_usbd_serial_num.c \
	$(SDK_ROOT)/components/libraries/usbd/app_usbd_string_desc.c \
	$(SDK_ROOT)/components/libraries/usbd/class/cdc/acm/app_usbd_cdc_acm.c \
	$(SDK_ROOT)/components/libraries/util/app_error.c \
	$(SDK_ROOT)/components/libraries/util/app_error_handler_gcc.c \
	$(SDK_ROOT)/components/libraries/util/app_error_weak.c \
	$(SDK_ROOT)/components/libraries/util/app_util_platform.c \
	$(SDK_ROOT)/components/libraries/util/nrf_assert.c \
	$(SDK_ROOT)/components/softdevice/common/nrf_sdh.c \
	$(SDK_ROOT)/components/softdevice/common/nrf_sdh_ble.c \
	$(SDK_ROOT)/components/softdevice/common/nrf_sdh_soc.c \
	$(SDK_ROOT)/external/fatfs/port/diskio_blkdev.c \
	$(SDK_ROOT)/external/fatfs/src/ff.c \
	$(SDK_ROOT)/external/fprintf/nrf_fprintf.c \
	$(SDK_ROOT)/external/fprintf/nrf_fprintf_format.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_Syscalls_GCC.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_printf.c \
	$(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_clock.c \
	$(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_power.c \
	$(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_ppi.c \
	$(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_rng.c \
	$(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_spi.c \
	$(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_twi.c \
	$(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_uart.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_clock.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_gpiote.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_power.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_ppi.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_rng.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_saadc.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_spi.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_spim.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_systick.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_timer.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_twi.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_twim.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uart.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_uarte.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_usbd.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/prs/nrfx_prs.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_i2s.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_qspi.c \
	$(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf52840.S \
	$(SDK_ROOT)/modules/nrfx/mdk/system_nrf52840.c \
	$(SDK_ROOT)/modules/nrfx/soc/nrfx_atomic.c

SDK_INCLUDES := \
	-I$(SDK_ROOT)/components \
	-I$(SDK_ROOT)/components/ble/ble_advertising \
	-I$(SDK_ROOT)/components/ble/ble_dtm \
	-I$(SDK_ROOT)/components/ble/ble_racp \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_ancs_c \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_ans_c \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_bas \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_bas_c \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_cscs \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_cts_c \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_dfu \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_dis \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_gls \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_hids \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_hrs \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_hrs_c \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_hts \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_ias \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_ias_c \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_lbs \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_lbs_c \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_lls \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_nus \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_nus_c \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_rscs \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_rscs_c \
	-I$(SDK_ROOT)/components/ble/ble_services/ble_tps \
	-I$(SDK_ROOT)/components/ble/common \
	-I$(SDK_ROOT)/components/ble/nrf_ble_gatt \
	-I$(SDK_ROOT)/components/ble/nrf_ble_qwr \
	-I$(SDK_ROOT)/components/ble/peer_manager \
	-I$(SDK_ROOT)/components/boards \
	-I$(SDK_ROOT)/components/libraries/atomic \
	-I$(SDK_ROOT)/components/libraries/atomic_fifo \
	-I$(SDK_ROOT)/components/libraries/atomic_flags \
	-I$(SDK_ROOT)/components/libraries/balloc \
	-I$(SDK_ROOT)/components/libraries/block_dev \
	-I$(SDK_ROOT)/components/libraries/block_dev/sdc \
	-I$(SDK_ROOT)/components/libraries/bootloader/ble_dfu \
	-I$(SDK_ROOT)/components/libraries/bsp \
	-I$(SDK_ROOT)/components/libraries/button \
	-I$(SDK_ROOT)/components/libraries/cli \
	-I$(SDK_ROOT)/components/libraries/crc16 \
	-I$(SDK_ROOT)/components/libraries/crc32 \
	-I$(SDK_ROOT)/components/libraries/crypto \
	-I$(SDK_ROOT)/components/libraries/csense \
	-I$(SDK_ROOT)/components/libraries/csense_drv \
	-I$(SDK_ROOT)/components/libraries/delay \
	-I$(SDK_ROOT)/components/libraries/ecc \
	-I$(SDK_ROOT)/components/libraries/experimental_section_vars \
	-I$(SDK_ROOT)/components/libraries/experimental_task_manager \
	-I$(SDK_ROOT)/components/libraries/fds \
	-I$(SDK_ROOT)/components/libraries/fifo \
	-I$(SDK_ROOT)/components/libraries/fstorage \
	-I$(SDK_ROOT)/components/libraries/gfx \
	-I$(SDK_ROOT)/components/libraries/gpiote \
	-I$(SDK_ROOT)/components/libraries/hardfault \
	-I$(SDK_ROOT)/components/libraries/hci \
	-I$(SDK_ROOT)/components/libraries/led_softblink \
	-I$(SDK_ROOT)/components/libraries/log \
	-I$(SDK_ROOT)/components/libraries/log/src \
	-I$(SDK_ROOT)/components/libraries/low_power_pwm \
	-I$(SDK_ROOT)/components/libraries/mem_manager \
	-I$(SDK_ROOT)/components/libraries/memobj \
	-I$(SDK_ROOT)/components/libraries/mpu \
	-I$(SDK_ROOT)/components/libraries/mutex \
	-I$(SDK_ROOT)/components/libraries/pwm \
	-I$(SDK_ROOT)/components/libraries/pwr_mgmt \
	-I$(SDK_ROOT)/components/libraries/queue \
	-I$(SDK_ROOT)/components/libraries/ringbuf \
	-I$(SDK_ROOT)/components/libraries/scheduler \
	-I$(SDK_ROOT)/components/libraries/sdcard \
	-I$(SDK_ROOT)/components/libraries/slip \
	-I$(SDK_ROOT)/components/libraries/sortlist \
	-I$(SDK_ROOT)/components/libraries/spi_mngr \
	-I$(SDK_ROOT)/components/libraries/stack_guard \
	-I$(SDK_ROOT)/components/libraries/strerror \
	-I$(SDK_ROOT)/components/libraries/timer \
	-I$(SDK_ROOT)/components/libraries/twi_mngr \
	-I$(SDK_ROOT)/components/libraries/twi_sensor \
	-I$(SDK_ROOT)/components/libraries/uart \
	-I$(SDK_ROOT)/components/libraries/usbd \
	-I$(SDK_ROOT)/components/libraries/usbd/class/audio \
	-I$(SDK_ROOT)/components/libraries/usbd/class/cdc \
	-I$(SDK_ROOT)/components/libraries/usbd/class/cdc/acm \
	-I$(SDK_ROOT)/components/libraries/usbd/class/hid \
	-I$(SDK_ROOT)/components/libraries/usbd/class/hid/generic \
	-I$(SDK_ROOT)/components/libraries/usbd/class/hid/kbd \
	-I$(SDK_ROOT)/components/libraries/usbd/class/hid/mouse \
	-I$(SDK_ROOT)/components/libraries/usbd/class/msc \
	-I$(SDK_ROOT)/components/libraries/util \
	-I$(SDK_ROOT)/components/nfc/ndef/conn_hand_parser \
	-I$(SDK_ROOT)/components/nfc/ndef/conn_hand_parser/ac_rec_parser \
	-I$(SDK_ROOT)/components/nfc/ndef/conn_hand_parser/ble_oob_advdata_parser \
	-I$(SDK_ROOT)/components/nfc/ndef/conn_hand_parser/le_oob_rec_parser \
	-I$(SDK_ROOT)/components/nfc/ndef/connection_handover/ac_rec \
	-I$(SDK_ROOT)/components/nfc/ndef/connection_handover/ble_oob_advdata \
	-I$(SDK_ROOT)/components/nfc/ndef/connection_handover/ble_pair_lib \
	-I$(SDK_ROOT)/components/nfc/ndef/connection_handover/ble_pair_msg \
	-I$(SDK_ROOT)/components/nfc/ndef/connection_handover/common \
	-I$(SDK_ROOT)/components/nfc/ndef/connection_handover/ep_oob_rec \
	-I$(SDK_ROOT)/components/nfc/ndef/connection_handover/hs_rec \
	-I$(SDK_ROOT)/components/nfc/ndef/connection_handover/le_oob_rec \
	-I$(SDK_ROOT)/components/nfc/ndef/generic/message \
	-I$(SDK_ROOT)/components/nfc/ndef/generic/record \
	-I$(SDK_ROOT)/components/nfc/ndef/launchapp \
	-I$(SDK_ROOT)/components/nfc/ndef/parser/message \
	-I$(SDK_ROOT)/components/nfc/ndef/parser/record \
	-I$(SDK_ROOT)/components/nfc/ndef/text \
	-I$(SDK_ROOT)/components/nfc/ndef/uri \
	-I$(SDK_ROOT)/components/nfc/t2t_lib \
	-I$(SDK_ROOT)/components/nfc/t2t_parser \
	-I$(SDK_ROOT)/components/nfc/t4t_lib \
	-I$(SDK_ROOT)/components/nfc/t4t_parser/apdu \
	-I$(SDK_ROOT)/components/nfc/t4t_parser/cc_file \
	-I$(SDK_ROOT)/components/nfc/t4t_parser/hl_detection_procedure \
	-I$(SDK_ROOT)/components/nfc/t4t_parser/tlv \
	-I$(SDK_ROOT)/components/softdevice/common \
	-I$(SDK_ROOT)/components/softdevice/s140/headers \
	-I$(SDK_ROOT)/components/softdevice/s140/headers/nrf52 \
	-I$(SDK_ROOT)/components/toolchain/cmsis/include \
	-I$(SDK_ROOT)/external/fatfs/port/ \
	-I$(SDK_ROOT)/external/fatfs/src \
	-I$(SDK_ROOT)/external/fprintf \
	-I$(SDK_ROOT)/external/protothreads \
	-I$(SDK_ROOT)/external/protothreads/pt-1.4/ \
	-I$(SDK_ROOT)/external/segger_rtt \
	-I$(SDK_ROOT)/external/utf_converter \
	-I$(SDK_ROOT)/integration/nrfx \
	-I$(SDK_ROOT)/integration/nrfx/legacy \
	-I$(SDK_ROOT)/modules/nrfx \
	-I$(SDK_ROOT)/modules/nrfx/drivers/include \
	-I$(SDK_ROOT)/modules/nrfx/hal \
	-I$(SDK_ROOT)/modules/nrfx/mdk
