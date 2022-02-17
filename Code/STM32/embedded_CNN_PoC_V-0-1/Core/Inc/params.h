
/* THIS IS AN AUTO-GENERATED FILE! DO NOT MODIFY MANUALLY!
 * f_name:      params.h
 * auth:        Jakob Tschavoll
 * ver:         1.0, deploying "N4CED_V-0-2"
 * brief:       sync all params with jupyter-notebooks
 * datetime:    17/02/2022 15:11:35
 * */

#ifndef _PARAMS_H_
#define _PARAMS_H_

#define SAMPLE_RATE         48000
#define IIR_ORDER           8
#define N_COEFFS            6
#define N_DEC_STAGES        4
#define N_BANDS             28
#define N_IIR_SECTIONS      4
#define DEC_FIR_ORDER       10
#define FRAME_SIZE          1000
#define WIN_SIZE            48000
#define FPS                 48

char class_map[][20] = {
	"car_horn",
	"children_playing",
	"dog_bark",
	"drilling",
	"engine_idling",
	"jackhammer",
	"street_music",
};


#endif
                