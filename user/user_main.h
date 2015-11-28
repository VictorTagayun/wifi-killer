
struct RxControl 
{
	signed rssi:8;
	unsigned rate:4;
	unsigned is_group:1;
	unsigned:1;
	unsigned sig_mode:2;
	unsigned legacy_length:12;
	unsigned damatch0:1;
	unsigned damatch1:1;
	unsigned bssidmatch0:1;
	unsigned bssidmatch1:1;
	unsigned MCS:7;
	unsigned CWB:1;
	unsigned HT_length:16;
	unsigned Smoothing:1;
	unsigned Not_Sounding:1;
	unsigned:1;
	unsigned Aggregation:1;
	unsigned STBC:2;
	unsigned FEC_CODING:1;
	unsigned SGI:1;
	unsigned rxend_state:8;
	unsigned ampdu_cnt:8;
	unsigned channel:4;
	unsigned:12;
};

struct RxPacket 
{
	struct RxControl rx_ctl;
	uint8 data[];
};
 
typedef struct ieee80211_frame_control 
{
	uint8_t		version:2;
	uint8_t		type:2;
	uint8_t		subtype:4;
	uint8_t		to_ds:1;
	uint8_t		from_ds:1;
	uint8_t		more_frag:1;
	uint8_t		retry:1;
	uint8_t		power:1;
	uint8_t		more_data:1;
	uint8_t		wep:1;
	uint8_t		order:1;
} __attribute__ ((__packed__)) ieee80211_frame_control;

typedef struct ieee80211_mgmt_frame 
{
	ieee80211_frame_control ctl;
	uint16_t	duration;
	uint8_t		addr1[6];
	uint8_t		addr2[6];
	uint8_t		addr3[6];
	uint16_t	seq_ctrl;
} __attribute__ ((__packed__)) ieee80211_mgmt_frame;

