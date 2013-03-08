#include <OTUS/OTUS_soc.h>
#include <cmnos_api.h>
#include <hif_api.h>
#include <Magpie_api.h>
#include <dma_lib.h>
#include <hif_pci.h>
#include <vbuf_api.h>

#include "fwd.h"

fwd_tgt_softc_t fwd_sc;

void    fwd_tgt_recv(VBUF *hdr_buf, VBUF *buf, void *ctx );
void    fwd_retbuf_handler(VBUF *buf, void *ServiceCtx);

hif_handle_t fwd_init() 
 {
    HIF_CALLBACK    hifconfig;
    A_UINT32      size, res_headroom;
    
    hifconfig.send_buf_done = fwd_retbuf_handler;
    hifconfig.recv_buf      = fwd_tgt_recv;
    hifconfig.context       = &fwd_sc;
    
    res_headroom = HIF_get_reserved_headroom(NULL);
    
    size = sizeof(fwd_rsp_t) + res_headroom;

    HIF_register_callback(NULL, &hifconfig);
    
    HIF_get_default_pipe(NULL, &fwd_sc.rx_pipe, &fwd_sc.tx_pipe);
    
    return NULL;
 }
 
void
fwd_retbuf_handler(VBUF *buf, void *ServiceCtx)
{
 HIF_return_recv_buf(fwd_sc.hif_handle, fwd_sc.rx_pipe, buf);
}

a_status_t
fwd_tgt_process_last(A_UINT32 size, A_UINT32 cksum)
{
    int i, checksum   = 0;
    A_UINT32   *image = (A_UINT32 *)fwd_sc.addr;
    
    for (i = 0 ; i < size; i += 4, image++) 
        checksum  =   checksum ^ *image;
        
    if (checksum == cksum)
        return A_STATUS_OK;
    else
        return A_STATUS_FAILED;
}        
        
   

void 
fwd_tgt_recv(VBUF *hdr_buf, VBUF *buf, void *ctx )
{
    volatile a_uint8_t *data;
    A_UINT32 len, seglen, offset, i, more, eloc;
    volatile A_UINT32 *image, *daddr;
    volatile fwd_cmd_t *c;  
    volatile fwd_rsp_t *r;
    jmp_func funcptr;
    a_status_t  status;
    VDESC *desc = NULL;

    data = buf->desc_list->buf_addr + buf->desc_list->data_offset;
    seglen  = buf->desc_list->data_size;

    c      =  (fwd_cmd_t *)data;
    len    =  c->len;
    offset =  c->offset;
    more   =  c->more_data;
    image  =  (A_UINT32 *)(c + 1);

    if (offset == 0) {
        fwd_sc.addr  = (A_UINT32)(*image);
        image ++;
    }

    daddr = (A_UINT32 *)(fwd_sc.addr + offset);

    if (!more) {
        len -= 4;
	}
 
    for (i = 0 ; i < len; i += 4) {
        *daddr       =   *image; 
        image ++;
        daddr ++;
    }
    
    desc = buf->desc_list; 
     while(desc->next_desc != NULL)
        desc = desc->next_desc;
    desc->data_size -= seglen;
    buf->buf_length -= seglen;

    r = (fwd_rsp_t *)(desc->buf_addr + desc->data_offset + desc->data_size);
    desc->data_size += sizeof(fwd_rsp_t); 
    buf->buf_length += sizeof(fwd_rsp_t); 

    r->offset = c->offset;

    if (more) {
        r->rsp = FWD_RSP_ACK;
        goto done;
    }
    
    status = fwd_tgt_process_last(offset + len,  *image);
 
    /* reach to the jump location */
    image++;
    eloc   =  *image;
        
    if (status == A_STATUS_OK)
        r->rsp = FWD_RSP_SUCCESS;
    else
        r->rsp = FWD_RSP_FAILED;
    
     
done:       
    HIF_send_buffer(fwd_sc.hif_handle, fwd_sc.tx_pipe, buf);

    if (!more && (status == A_STATUS_OK)) {
        funcptr = (jmp_func)eloc;
        funcptr();
    }
}
