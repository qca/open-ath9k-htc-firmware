
#ifndef _NET80211_IEEE80211_PROTO_H_
#define _NET80211_IEEE80211_PROTO_H_


#define IEEE80211_TXOP_TO_US(_txop)  (a_uint32_t)(_txop) << 5


/*
 * 802.11 protocol implementation definitions.
 */

enum ieee80211_state {
	IEEE80211_S_INIT	= 0,	/* default state */
	IEEE80211_S_SCAN	= 1,	/* scanning */
	IEEE80211_S_JOIN	= 2,	/* join */
	IEEE80211_S_AUTH	= 3,	/* try to authenticate */
	IEEE80211_S_ASSOC	= 4,	/* try to assoc */
	IEEE80211_S_RUN		= 5,	/* associated */
};

#endif
