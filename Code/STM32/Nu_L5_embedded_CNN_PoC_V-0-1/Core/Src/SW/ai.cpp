#include "ai.h"

cnn cnn_instance;


cnn::cnn()
{

}



void cnn::init()
{
	const ai_handle acts[] = { activations };
	err = ai_n4ced_v02_create_and_init(&pNN, acts, NULL);
	if (err.type != AI_ERROR_NONE)
	{
		// TODO: handle error
	}

	ai_input = ai_n4ced_v02_inputs_get(pNN, NULL);
	ai_output = ai_n4ced_v02_outputs_get(pNN, NULL);

	ai_input[0].data = AI_HANDLE_PTR(in_data);
	ai_output[0].data = AI_HANDLE_PTR(out_data);
}



void cnn::scale_inputs(float pData[FPS][N_BANDS])
{
	volatile float db_mean = 0;
	volatile float sd = 0;
	volatile float temp = 0;

	for(uint16_t i = 0; i < FPS; i++)
	{
		for(uint16_t j = 0; j < N_BANDS; j++)
		{
			db_mean += pData[i][j];
		}
	}

	db_mean /= (FPS * N_BANDS);

	for(uint16_t i = 0; i < FPS; i++)
	{
		for(uint16_t j = 0; j < N_BANDS; j++)
		{
			temp = pData[i][j] - db_mean;
			sd += (temp * temp);
		}
	}

	sd /= (FPS * N_BANDS);
	sd = sqrtf(sd);


	for(uint16_t i = 0; i < FPS; i++)
	{
		for(uint16_t j = 0; j < N_BANDS; j++)
		{
			in_data[i][j] = (pData[i][j] - db_mean) / sd;
		}
	}
}



ai_float* cnn::run()
{
	n_batch = ai_n4ced_v02_run(pNN, &ai_input[0], &ai_output[0]);
	if (n_batch != 1)
	{
		err = ai_n4ced_v02_get_error(pNN);
		// TODO: handle error
	}
	return out_data;
}



uint8_t cnn::get_top_index(void)
{
	return top_class_idx;
}



void cnn::set_top_index(uint8_t idx)
{
	top_class_idx = idx;
}



uint8_t cnn::get_top_score(void)
{
	return top_class_score;
}



void cnn::set_top_score(uint8_t score)
{
	top_class_score = score;
}
