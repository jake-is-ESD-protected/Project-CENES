/*
*****************************************************************************************
file name:       	nextion.h
auth:               Jakob Tschavoll
date:               10.03.22
brief:              display abstraction as object
version:            V0.1
notes:				Shape of RX-signal:
						start	|	page	|	id	|	action	|	end
						0x65	|	0xXX	|	0xXX|	0xXX	|	0xFF 0xFF 0xFF
*****************************************************************************************
*/

#ifndef _NEXTION_H_
#define _NEXTION_H_

#include "serial.h"
#include "params.h"
#include "nex_obj_ids.h"


#define NEX_TX_BUF_SIZE	512
#define NEX_RX_BUF_SIZE	7

#define MAX_SPL_LVL_SHOWN	120.
#define MIN_SPL_LVL_SHOWN	-20.

#define BIN_REFRESH_STEP	(FPS / 12)
#define LVL_REFRESH_STEP	(FPS)
#define LEQ_5s				(FPS * 5)

#define NEX_BUF_RDY_FLAG	0x00000100U
#define NEX_LVL_RDY_FLAG	0x00001000U

#define NEX_META_INFO		0
#define NEX_FRAME_PAGE		1
#define NEX_FRAME_ID		2
#define NEX_FRAME_ACT		3

#define NEX_GET_VAL_TYPE	0x71



typedef enum{
	u8_num_val,
	i16_num_val,
	txt_val,
	u8_page,
	f32_num_val,
	get_slider,
	show_graphics,
	hide_graphics
}nex_tx_t;


/* [CLASS] nextionHMI
 * @brief:	objectification of the nextion display
 * @intent:	data exchange over on-device TFT-display
 * */
class nextionHMI: public uart
{
public:
	using uart::uart;

	// bank output formatted as value from 0% to 100%
	uint8_t disp_data[N_BANDS];
	int16_t disp_lvl;




	/* Initializer
	 * @params: 	`void`
	 * @returns 	`void`
	 * @brief:		`init nextion IT RX`
	 * @notes:		needed because singleton-implementation forces
	 * 				a method which inits the UART IT settings AFTER
	 * 				HAL_init(), since the uart-object is created before
	 * 				the call of main(), similar to arduino `Serial.begin()`
	 * */
	void reset(void);



	void tx(char* id, void* pVal, nex_tx_t type);



	void handle(cmd c);



	void update_page(int8_t page);



	void rx_callback(void);



	char tx_buf[NEX_TX_BUF_SIZE];
	volatile char rx_buf[NEX_RX_BUF_SIZE];

	char bin_id_buf[N_BANDS][4] = {	"j38", "j37", "j36", "j35", "j34", "j33", "j32", "j31", "j30", "j29", "j28", "j27", "j26",
									"j25", "j24", "j23", "j22", "j21", "j20", "j19", "j18", "j17", "j16", "j15", "j14", "j13",
									"j12", "j11"};
	const char nex_end[3] = {0xFF, 0xFF, 0xFF};

private:

	uint8_t format(char* id, void* pVal, nex_tx_t type)
	{
		uint8_t len = 0;

		if(type == u8_num_val)
		{
			uint8_t temp_val = *((uint8_t*)pVal);
			len = sprintf(tx_buf, "%s.val=%d", id, temp_val); // @suppress("Invalid arguments")
		}
		else if(type == i16_num_val)
		{
			int16_t temp_val = *((int16_t*)pVal);
			len = sprintf(tx_buf, "%s.val=%d", id, temp_val); // @suppress("Invalid arguments")
		}
		else if(type == txt_val)
		{
			char* temp_str = (char*)pVal;
			len = sprintf(tx_buf, "%s.txt=\"%s\"", id, temp_str); // @suppress("Invalid arguments")
		}

		else if(type == u8_page)
		{
			uint8_t temp_val = *((uint8_t*)pVal);
			len = sprintf(tx_buf, "page %d", temp_val); // @suppress("Invalid arguments")
		}
		else if(type == f32_num_val)
		{
			float temp_val = *((float*)pVal);
			len = sprintf(tx_buf, "%s.val=%.0f", id, temp_val * 1000); // @suppress("Invalid arguments")
		}
		else if(type == get_slider)
		{
			len = sprintf(tx_buf, "get %s.val", id); // @suppress("Invalid arguments")
		}
		else if(type == show_graphics)
		{
			len = sprintf(tx_buf, "vis %s,1", id); // @suppress("Invalid arguments")
		}
		else if(type == hide_graphics)
		{
			len = sprintf(tx_buf, "vis %s,0", id); // @suppress("Invalid arguments")
		}

		for(uint8_t i = 0; i < 3; i++)
		{
			tx_buf[len] = 0xFF;
			len++;
		}
		return len;
	}



	cmd str2cmd(void)
	{
		cmd c = {.type = no_info, .origin = nextion_e, .destination = nextion_e, .prio = HIGH, .params = NULL};

		uint8_t meta = rx_buf[NEX_META_INFO];
		uint8_t page = rx_buf[NEX_FRAME_PAGE];
		uint8_t id = rx_buf[NEX_FRAME_ID];

		if(meta == NEX_GET_VAL_TYPE)
		{
			c.type = set_thresh_num;
			c.params = (void*)(&rx_buf[1]);
			return c;
		}

		switch(page)
		{
			// command sent from page 0 (RTA)
			case NEX_PAGE0:
			{
				switch(id)
				{
					case NEX_RX_CLASS_PUSH_P0:
					{
						c.type = switch_ai;
					}break;

					case NEX_RX_SET_PUSH_P0:
					{
						c.type = switch_settings;
					}break;

					case NEX_RX_SET_Z_WEIGHT:
					{
						c.type = set_Z_weight;
					}break;

					case NEX_RX_SET_A_WEIGHT:
					{
						c.type = set_A_weight;
					}break;
				}
			}break;

			// command sent from page 1 (Classification)
			case NEX_PAGE1:
			{
				switch(id)
				{
					case NEX_RX_RTA_PUSH_P1:
					{
						c.type = switch_rta;
					}break;

					case NEX_RX_SET_PUSH_P1:
					{
						c.type = switch_settings;
					}break;

					case NEX_RX_SET_CEQ1:
					{
						c.type = set_CEQ1;
					}break;

					case NEX_RX_SET_CEQ3:
					{
						c.type = set_CEQ3;
					}break;

					case NEX_RX_SET_CEQ5:
					{
						c.type = set_CEQ5;
					}break;

					case NEX_RX_SET_THRESH:
					{
						c.type = set_thresh_req;
					}
					break;
				}
			}break;

			// command sent from page 2 (Settings)
			case NEX_PAGE2:
			{
				switch(id)
				{
					case NEX_RX_RTA_PUSH_P2:
					{
						c.type = switch_rta;
					}break;

					case NEX_RX_CLASS_PUSH_P2:
					{
						c.type = switch_ai;
					}break;
				}
			}break;
		}
		return c;
	}
};

extern nextionHMI nextion;

#endif
