#!/bin/bash
time ./main <simulacao_100.txt> out_100.txt
time ./main <simulacao_1k.txt> out_1k.txt
time ./main <simulacao_5k.txt> out_5k.txt
time ./main <simulacao_10k.txt> out_10k.txt 
time ./main <simulacao_100k.txt> out_100k.txt
time ./main <simulacao_500k.txt> out_500k.txt
time ./main <simulacao_1M.txt> out_1M.txt 