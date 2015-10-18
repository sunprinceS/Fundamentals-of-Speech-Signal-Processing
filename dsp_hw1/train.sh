#!/usr/bin/env sh

c_cpp/train 30 model_init.txt seq_model_01.txt model_01.txt &
c_cpp/train 30 model_init.txt seq_model_02.txt model_02.txt &
c_cpp/train 30 model_init.txt seq_model_03.txt model_03.txt &
c_cpp/train 30 model_init.txt seq_model_04.txt model_04.txt &
c_cpp/train 30 model_init.txt seq_model_05.txt model_05.txt &

wait
echo "Training finished!"
