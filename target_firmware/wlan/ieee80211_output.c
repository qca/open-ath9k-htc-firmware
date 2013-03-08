#include <adf_os_types.h>
#include <adf_os_dma.h>
#include <adf_os_timer.h>
#include <adf_os_lock.h>
#include <adf_os_io.h>
#include <adf_os_mem.h>
#include <adf_os_module.h>
#include <adf_os_util.h>
#include <adf_os_stdtypes.h>
#include <adf_os_defer.h>
#include <adf_os_atomic.h>
#include <adf_nbuf.h>
#include <adf_net.h>

#include <if_llc.h>
#include <if_ethersubr.h>
#include "ieee80211_var.h"

#include "_ieee80211.h"
#include "ieee80211.h"
#include <wlan_hdr.h>

a_status_t
ieee80211_tgt_crypto_encap(struct ieee80211_frame *wh,
			   struct ieee80211_node_target *ni,
			   a_uint8_t keytype)
{
#define CRYPTO_KEY_TYPE_AES          2
#define CRYPTO_KEY_TYPE_TKIP         3
#define CRYPTO_KEY_TYPE_WAPI         4
#define IEEE80211_WLAN_HDR_LEN      24

	a_uint8_t *iv = NULL;
	a_uint16_t tmp;
	a_uint16_t offset = IEEE80211_WLAN_HDR_LEN;
	a_uint8_t b1, b2;

	if (IEEE80211_QOS_HAS_SEQ(wh))
		offset += 4;  // pad for 4 byte alignment

	iv = (a_uint8_t *) wh;
	iv = iv + offset;

	switch (keytype) {
	case CRYPTO_KEY_TYPE_AES:
		ni->ni_iv16++;
		if (ni->ni_iv16 == 0)
		{
			ni->ni_iv32++;
		}

		*iv++ = (a_uint8_t) ni->ni_iv16;
		*iv++ = (a_uint8_t) (ni->ni_iv16 >> 8);
		*iv++ = 0x00;
		*iv++ |= 0x20;

		tmp = (a_uint16_t) ni->ni_iv32;
		*iv++ = (a_uint8_t) tmp;
		*iv++ = (a_uint8_t) (tmp >> 8);

		tmp = (a_uint16_t) (ni->ni_iv32 >> 16);
		*iv++ = (a_uint8_t) tmp;
		*iv = (a_uint8_t) (tmp >> 8);
		break;
	case CRYPTO_KEY_TYPE_TKIP:
		ni->ni_iv16++;
		if (ni->ni_iv16 == 0)
		{
			ni->ni_iv32++;
		}

		b1 = (a_uint8_t) (ni->ni_iv16 >> 8);
		b2 = (b1 | 0x20) & 0x7f;

		*iv++ = b1;
		*iv++ = b2;

		*iv++ = (a_uint8_t) ni->ni_iv16;
		*iv++ |= 0x20;

		tmp = (a_uint16_t) ni->ni_iv32;
		*iv++ = (a_uint8_t) tmp;
		*iv++ = (a_uint8_t) (tmp >> 8);

		tmp = (a_uint16_t) (ni->ni_iv32 >> 16);
		*iv++ = (a_uint8_t) tmp;
		*iv = (a_uint8_t) (tmp >> 8);
		break;
	default:
		break;
	}

	return 1;

#undef CRYPTO_KEY_TYPE_TKIP
#undef CRYPTO_KEY_TYPE_AES
#undef CRYPTO_KEY_TYPE_WAPI
#undef IEEE80211_WLAN_HDR_LEN
}

adf_os_export_symbol(ieee80211_tgt_crypto_encap);

#undef  IEEE80211_ADDR_LEN     
