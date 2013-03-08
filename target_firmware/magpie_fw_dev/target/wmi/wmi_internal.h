/*
 * @File: 
 * 
 * @Abstract: internal data and structure definitions for WMI service
 * 
 * @Notes: 
 * 
 * 
 * Copyright (c) 2007 Atheros Communications Inc.
 * All rights reserved.
 *
 */

#ifndef WMI_INTERNAL_H_
#define WMI_INTERNAL_H_

#define WMI_CMD_ALIGNMENT_SIZE  128

#ifdef WMI_DEBUG

/* WMI debug log definitions */

#define WMI_DBG0_LOG(debugid) \
    DBGLOG_ARG0_RECORD(DBGLOG_HEADER_UPPER_HALF(debugid, \
                       DBGLOG_MODULEID_WMI, 0))

#define WMI_DBG1_LOG(debugid, arg1) \
    DBGLOG_ARG1_RECORD(DBGLOG_HEADER_UPPER_HALF(debugid, \
                       DBGLOG_MODULEID_WMI, 1), arg1)

#define WMI_DBG2_LOG(debugid, arg1, arg2) \
    DBGLOG_ARG2_RECORD(DBGLOG_HEADER_UPPER_HALF(debugid, \
                       DBGLOG_MODULEID_WMI, 2), arg1, arg2)

#else
#define WMI_DBG0_LOG(debugid)
#define WMI_DBG1_LOG(debugid, arg1)
#define WMI_DBG2_LOG(debugid, arg1, arg2)
#endif /* WMI_DEBUG */

#define EVT_PKT_IN_USE        (1 << 0)
#define EVT_PKT_IS_FREE(e)    !((e)->Flags & EVT_PKT_IN_USE)  
#define EVT_MARK_FREE(e)      (e)->Flags &= ~EVT_PKT_IN_USE;
#define EVT_MARK_INUSE(e)     (e)->Flags |= EVT_PKT_IN_USE
#define IS_EVT_CLASS_BUFFERED(ec)   ((ec) != WMI_EVT_CLASS_DIRECT_BUFFER)

typedef struct _WMI_POOL_STATE {
	int     MaxAllocation;      /* maximum allocations allowed for this pool */
	int     CurrentAllocation;  /* current allocations outstanding */
} WMI_POOL_STATE; 

typedef struct _WMI_SVC_CONTEXT {
	htc_handle_t         HtcHandle;
	pool_handle_t        PoolHandle;    
	int                  PendingEvents;                      /* no. of pending events */
	HTC_SERVICE          WMIControlService;                  /* registered control service */
	HTC_ENDPOINT_ID      ControlEp;                          /* endpoint assigned to us */
	WMI_DISPATCH_TABLE  *pDispatchHead;                      /* dispatch list head ptr  */
	WMI_DISPATCH_TABLE  *pDispatchTail;                      /* dispatch list tail ptr */   

	// Left a door for extension the structure
	void *pReserved;
} WMI_SVC_CONTEXT;

#endif /*WMI_INTERNAL_H_*/
