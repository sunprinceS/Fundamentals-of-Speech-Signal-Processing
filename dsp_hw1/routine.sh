#!/usr/bin/env sh

cp model_0*.txt c_cpp/
cd c_cpp/
./test modellist.txt ../testing_data1.txt result1.txt acc.txt
echo "result1.txt"
cut -f1 -d ' ' < result1.txt | sort | uniq -c
echo "========================================="
echo "ans"
cut -f1 -d ' ' < ../testing_answer.txt | sort | uniq -c
echo "======================================="
./test modellist.txt ../testing_data2.txt result2.txt
cat acc.txt

echo "result2.txt"
cut -f1 -d ' ' < result2.txt | sort | uniq -c
