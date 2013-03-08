#ifndef _NET80211_IEEE80211_NODE_H_
#define _NET80211_IEEE80211_NODE_H_

#include "_ieee80211.h"
#include "ieee80211.h"
#include <ieee80211_proto.h>		/* for proto macros on node */
#include <asf_queue.h>

#define	IEEE80211_NODE_HASHSIZE	32

/* Node Table information for the Target */

struct ieee80211_node_table {
        asf_tailq_head(, ieee80211_node)        nt_node;        /* information of all nodes */
        asf_list_head(, ieee80211_node)         nt_hash[IEEE80211_NODE_HASHSIZE];
        asf_list_head(, ieee80211_wds_addr)     nt_wds_hash[IEEE80211_NODE_HASHSIZE];
};

#define IEEE80211_KEYBUF_SIZE   16
#define IEEE80211_TID_SIZE      17 
#define IEEE80211_MICBUF_SIZE   (8+8)   /* space for both tx+rx keys */

struct ieee80211_key_target {
	a_int32_t dummy ;
};

#endif
