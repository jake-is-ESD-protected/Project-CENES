# Factsheet cnn_40_28_48k
```
CNN_name: cnn_40_28_48k
extr_txt_fp: Datasets/Feature_extraction/th_oct_coeffs.txt
label_txt_fp: Datasets/Feature_extraction/labels.txt
source_dir: Datasets/UrbanSound8k_augmented/
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
max_dur: 5
augmented: True
n_bands: 28
n_sections: 4
fps: 40
class_map: ['car_horn', 'children_playing', 'dog_bark', 'drilling', 'engine_idling', 'jackhammer', 'street_music']

```
Sheet written at 07/04/2022 11:13:14