# Factsheet esc50_model_v9
```
CNN_name: esc50_model_v9
extr_txt_fp: Datasets/Feature_extraction/th_oct_feats.txt
label_txt_fp: Datasets/Feature_extraction/labels.txt
leq_txt_fp: Datasets/Feature_extraction/LEQs.txt
source_dir: Datasets/ESC50_augmented/
kernel_size: [5, 5]
pool_size: [2, 2]
batch_size: 50
epochs: 70
fs: 32000
bit_dpt: 24
fmin: 20
fmax: 12800
oct_bw_ratio: 0.3333333333333333
order: 8
n_coeffs: 6
dec_stages: 4
dec_ord: 10
frame_size: 1280
window_size: 32000
max_dur: 6
threshold: -25
augmented: True
n_bands: 28
n_sections: 4
fps: 25
class_map: ['airplane', 'car_horn', 'engine', 'helicopter', 'train']

```
Sheet written at 04/05/2022 17:52:12