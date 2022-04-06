#ifndef _SAMPLE_H_
#define _SAMPLE_H_


#define UINT24_MAX		16777215

#include "main.h"
#include "params.h"
#include "err.h"
#include "run.h"

typedef struct {
	uint8_t conv_req;
	uint8_t conv_hcplt;
	uint8_t conv_cplt;
	uint16_t siz;
	void* self;
	void* pin;
	void* pout;
}buffer_t;

typedef struct {
	uint8_t init;
	uint8_t running;
}sampler_flag_t;


void init_buffers(void);


void init_i2s_dma(void);


sampler_flag_t* start_sampler(void);


sampler_flag_t* stop_sampler(void);


void sample(void);


void fill_sample_buf(uint32_t* pInBuf, int32_t* pOutBuf, uint16_t len);


#endif
