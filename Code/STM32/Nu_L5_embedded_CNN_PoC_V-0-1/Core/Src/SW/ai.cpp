#include "ai.h"

cnn cnn_instance;


cnn::cnn()
{

}



void cnn::init()
{
	const ai_handle acts[] = { activations };
	err = ai_cnn_create_and_init(&pNN, acts, NULL);
	if (err.type != AI_ERROR_NONE)
	{
		e_handler.act(init_fail, ai_e);
	}

	ai_input = ai_cnn_inputs_get(pNN, NULL);
	ai_output = ai_cnn_outputs_get(pNN, NULL);

	ai_input[0].data = AI_HANDLE_PTR(in_data);
	ai_output[0].data = AI_HANDLE_PTR(out_data);
}



void cnn::scale_inputs(float pData[FPS][N_BANDS])
{
//	volatile float db_mean = 0;
//	volatile float sd = 0;
//	volatile float temp = 0;
//
//	for(uint16_t i = 0; i < FPS; i++)
//	{
//		for(uint16_t j = 0; j < N_BANDS; j++)
//		{
//			db_mean += pData[i][j];
//		}
//	}
//
//	db_mean /= (FPS * N_BANDS);
//
//	for(uint16_t i = 0; i < FPS; i++)
//	{
//		for(uint16_t j = 0; j < N_BANDS; j++)
//		{
//			temp = pData[i][j] - db_mean;
//			sd += (temp * temp);
//		}
//	}
//
//	sd /= (FPS * N_BANDS);
//	sd = sqrtf(sd);


	for(uint16_t i = 0; i < FPS; i++)
	{
		for(uint16_t j = 0; j < N_BANDS; j++)
		{
//			in_data[i][j] = (pData[i][j] - db_mean) / sd;
			in_data[i][j] = pData[i][j];
		}
	}
}



ai_float* cnn::run()
{
	n_batch = ai_cnn_run(pNN, &ai_input[0], &ai_output[0]);
	if (n_batch != 1)
	{
		err = ai_cnn_get_error(pNN);
		e_handler.act(generic, ai_e);
	}
	return out_data;
}
