// @TODO: Should define the memory region later~
#define ALLOCRAM_START       ( ((unsigned int)&_fw_image_end) + 4)
#define ALLOCRAM_SIZE        ( SYS_RAM_SZIE - ( ALLOCRAM_START - SYS_D_RAM_REGION_0_BASE) - SYS_D_RAM_STACK_SIZE)

#include "regdump.h"

#define SBOOT_PATTERN 0x5342
#define IS_FLASHBOOT() (((DEBUG_SYSTEM_STATE&~(0x0000ffff))>>16==SBOOT_PATTERN))

// patch for exception handle
void AR6002_fatal_exception_handler_patch(CPU_exception_frame_t *exc_frame);
void exception_reset(struct register_dump_s *dump);

void (* _assfail_ori)(struct register_dump_s *);
void HTCMsgRecvHandler_patch(adf_nbuf_t hdr_buf, adf_nbuf_t buffer, void *context);
void HTCControlSvcProcessMsg_patch(HTC_ENDPOINT_ID EndpointID, adf_nbuf_t hdr_buf,
				   adf_nbuf_t pBuffers, void *arg);

#if defined(PROJECT_K2)

#if MOVE_PRINT_TO_RAM
extern int fw_cmnos_printf(const char *fmt, ...);

extern uint16_t u8UsbConfigValue;
extern uint16_t u8UsbInterfaceValue;
extern uint16_t u8UsbInterfaceAlternateSetting;
#endif

extern void _fw_usb_fw_task(void);
extern void _fw_usb_reset_fifo(void);

#endif


void fatal_exception_func();
void init_mem();
void wlan_task();
