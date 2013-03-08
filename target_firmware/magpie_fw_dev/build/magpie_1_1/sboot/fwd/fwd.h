#ifndef __FWD_H
#define __FWD_H

#define FWD_TGT_RX_BUFS     5

typedef void (*jmp_func)(void);
/*
 * XXX Pack 'em
 */
typedef struct {
  a_uint16_t  more_data;     /* Is there more data? */
  a_uint16_t len;           /* Len this segment    */
  a_uint32_t offset;        /* Offset in the file  */
} fwd_cmd_t;
/*
 * No enums across platforms
 */
#define FWD_RSP_ACK     0x1
#define FWD_RSP_SUCCESS 0x2
#define FWD_RSP_FAILED  0x3

typedef struct {
    a_uint32_t  rsp;       /* ACK/SUCCESS/FAILURE */ 
    a_uint32_t  offset;    /* rsp for this ofset  */
}fwd_rsp_t;

typedef struct  {
    a_uint32_t     addr;
    hif_handle_t   hif_handle;
    a_uint8_t      rx_pipe;
    a_uint8_t      tx_pipe;
} fwd_tgt_softc_t;


hif_handle_t fwd_init();

void
fwd_retbuf_handler(VBUF *buf, void *ServiceCtx);
void 
fwd_hifrecv_handler(VBUF *hdr_buf, VBUF *buf, void *ServiceCtx );

#endif //__FWD_H
