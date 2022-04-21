# Factsheet sine_model_v4
```
CNN_name: sine_model_v4
extr_txt_fp: Datasets/Feature_extraction/th_oct_feats.txt
label_txt_fp: Datasets/Feature_extraction/labels.txt
leq_txt_fp: Datasets/Feature_extraction/LEQs.txt
source_dir: Datasets/sine_data/
kernel_size: [3, 3]
pool_size: [2, 2]
batch_size: 50
epochs: 50
fs: 48000
bit_dpt: 24
fmin: 20
fmax: 12800
oct_bw_ratio: 0.3333333333333333
order: 8
n_coeffs: 6
dec_stages: 4
dec_ord: 10
frame_size: 1200
window_size: 48000
max_dur: 10
augmented: True
n_bands: 28
n_sections: 4
fps: 40
class_map: ['noise', 'sine', 'sweep']

```
Sheet written at 21/04/2022 08:33:50