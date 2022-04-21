#ifndef _NEX_OBJ_IDS_H_
#define _NEX_OBJ_IDS_H_


// General
#define NEX_PAGE0				0
#define NEX_PAGE1				1
#define NEX_PAGE2				2

// Transmit
#define NEX_TX_PAGE0				(char*)"page0"
#define NEX_TX_PAGE1				(char*)"page1"
#define NEX_TX_PAGE2				(char*)"page2"
#define NEX_TX_BAT_BAR				(char*)"j0"
#define NEX_TX_SPL_FIELD			(char*)"n0"
#define NEX_TX_SPL_TYPE_FIELD		(char*)"t1"
#define NEX_TX_CLASS_NAME_FIELD_1	(char*)"t3"
#define NEX_TX_CLASS_NAME_FIELD_2	(char*)"t4"
#define NEX_TX_CLASS_NAME_FIELD_3	(char*)"t5"
#define NEX_TX_CLASS_SCORE_FIELD_1	(char*)"x0"
#define NEX_TX_CLASS_SCORE_FIELD_2	(char*)"x1"
#define NEX_TX_CLASS_SCORE_FIELD_3	(char*)"x2"
#define NEX_TX_A_WEIGHT_BUT			(char*)"b0"
#define NEX_TX_Z_WEIGHT_BUT			(char*)"b1"
#define NEX_CEQ_DISPLAY				(char*)"t1"
#define NEX_TX_THRESHOLD_DISPLAY	(char*)"j1"
#define NEX_TX_THRESHOLD_PEAK		(char*)"j2"
#define NEX_TX_THRESHOLD_GET		(char*)"h0"
#define NEX_TX_ZZZ_IMG				(char*)"p0"


// Receive
#define NEX_RX_RTA_PUSH_P1		1
#define NEX_RX_RTA_PUSH_P2		2
#define NEX_RX_CLASS_PUSH_P0	35
#define NEX_RX_CLASS_PUSH_P2	1
#define NEX_RX_SET_PUSH_P0		37
#define NEX_RX_SET_PUSH_P1		4
#define NEX_RX_SET_A_WEIGHT		38
#define NEX_RX_SET_Z_WEIGHT		39
#define NEX_RX_SET_CEQ1			13
#define NEX_RX_SET_CEQ3			14
#define NEX_RX_SET_CEQ5			15
#define NEX_RX_SET_THRESH		17

#endif
