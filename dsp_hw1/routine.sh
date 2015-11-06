#!/usr/bin/env sh

echo "result1.txt"
cut -f1 -d ' ' < result1.txt | sort | uniq -c
echo "========================================="
echo "ans"
cut -f1 -d ' ' < testing_answer.txt | sort | uniq -c
echo "======================================="
cat acc.txt

echo "result2.txt"
cut -f1 -d ' ' < result2.txt | sort | uniq -c
