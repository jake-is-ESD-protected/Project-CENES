#include "ai.h"
#include "uterm.h"

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



void cnn::normalize(float pData[FPS][N_BANDS], float leq)
{
	for(uint16_t i = 0; i < FPS; i++)
	{
		for(uint16_t j = 0; j < N_BANDS; j++)
		{
			in_data[i][j] = pData[i][j] - leq;
			if(in_data[i][j] < THRESHOLD)
			{
				in_data[i][j] = THRESHOLD;

			}
			uTerm.printf("%.1f ", in_data[i][j]);
		}
	}
	uTerm.printf("\r\n");
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
