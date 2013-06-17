# Metnum TP3 - OCR + SVD

./tp3-gen --verbose -i ../data/train-images.idx3-ubyte -l ../data/train-labels.idx1-ubyte -q 30 -d 1 -e 8

./tp3-classif --verbose -i ../data/t10k-images.idx3-ubyte -l ../data/t10k-labels.idx1-ubyte -o resultados.m -k 30 ./tp3_data_delta_1e-08.mdat
