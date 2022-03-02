#include "sample.h"

uint32_t _i2s_buf[FRAME_SIZE];
int32_t _dest_buf[FRAME_SIZE];

buffer_t i2s_buf;
buffer_t dest_buf;

sampler_flag_t sampler_flag = {
	.init = 0,
	.running = 0
};

void init_buffers(void){

	i2s_buf.conv_req = 		0;
	i2s_buf.conv_hcplt = 	0;
	i2s_buf.conv_cplt = 	0;
	i2s_buf.siz = 			FRAME_SIZE;
	i2s_buf.self = 			&_i2s_buf[0];
	i2s_buf.pin = 			&_i2s_buf[0];
	i2s_buf.pout = 			&_i2s_buf[FRAME_SIZE / 2];

	dest_buf.conv_req = 	0;
	dest_buf.conv_hcplt = 	0;
	dest_buf.conv_cplt = 	0;
	dest_buf.siz = 			FRAME_SIZE;
	dest_buf.self = 		&_dest_buf[0];
	dest_buf.pin = 			&_dest_buf[0];
	dest_buf.pout = 		&_dest_buf[FRAME_SIZE / 2];

	sampler_flag.init = 1;
}


void init_i2s_dma(void){
	HAL_StatusTypeDef stat = HAL_SAI_Receive_DMA(&hsai_BlockA1, (uint8_t*)i2s_buf.self, FRAME_SIZE);
	if(stat != HAL_OK){
//		err e(stat);
//		e.hdler_generic();
	}
}


sampler_flag_t* start_sampler(void){

	sampler_flag.running = 1;
	return &sampler_flag;
}


sampler_flag_t* stop_sampler(void){

	sampler_flag.running = 0;
	return &sampler_flag;
}


void sample(void){

	while(sampler_flag.running){

		if(i2s_buf.conv_hcplt){
			fill_sample_buf((uint32_t*)i2s_buf.pout, &((int32_t*)dest_buf.pin)[0], dest_buf.siz / 2);
			i2s_buf.conv_hcplt = 0;
		}
		if(i2s_buf.conv_cplt){

			fill_sample_buf((uint32_t*)i2s_buf.pout, &((int32_t*)dest_buf.pin)[FRAME_SIZE / 2], dest_buf.siz / 2);
			i2s_buf.conv_cplt = 0;
		}
	}
}


void fill_sample_buf(uint32_t* pInBuf, int32_t* pOutBuf, uint16_t len_out){

	for(uint16_t i = 0; i < len_out; i++){

		pOutBuf[i] = ((int32_t)(pInBuf[i] << 8)) >> 8 ;
	}

}


void HAL_SAI_RxHalfCpltCallback(SAI_HandleTypeDef* hsai){

	i2s_buf.pin = &((uint32_t*)i2s_buf.self)[FRAME_SIZE / 2];
	i2s_buf.pout = &((uint32_t*)i2s_buf.self)[0];
	i2s_buf.conv_hcplt = 1;
}


void HAL_SAI_RxCpltCallback(SAI_HandleTypeDef* hsai){

	i2s_buf.pin = &((uint32_t*)i2s_buf.self)[0];
	i2s_buf.pout = &((uint32_t*)i2s_buf.self)[FRAME_SIZE / 2];
	i2s_buf.conv_cplt = 1;
}
