#!/bin/bash

# argument is all OR the name of the test, eg. bigtest, error, etc.
if [ "$1" == "all" ];
then
  rm -f A1OutFiles/ysFiles/*
  ./disassembler A1TestFiles/halt_6_test.mem A1OutFiles/ysFiles/out_halt_6_test.ys
  ./disassembler A1TestFiles/halt_5_test.mem A1OutFiles/ysFiles/out_halt_5_test.ys
  ./disassembler A1TestFiles/halt_4_test.mem A1OutFiles/ysFiles/out_halt_4_test.ys
  ./disassembler A1TestFiles/halt_3_test.mem A1OutFiles/ysFiles/out_halt_3_test.ys
  ./disassembler A1TestFiles/halt_2_test.mem A1OutFiles/ysFiles/out_halt_2_test.ys
  ./disassembler A1TestFiles/halt_1_test.mem A1OutFiles/ysFiles/out_halt_1_test.ys
  ./disassembler A1TestFiles/words_test.mem A1OutFiles/ysFiles/out_words_test.ys
  ./disassembler A1TestFiles/invalid_ops_test.mem A1OutFiles/ysFiles/out_invalid_ops_test.ys
  ./disassembler A1TestFiles/first_byte_non_zero_test.mem A1OutFiles/ysFiles/out_first_byte_non_zero_test.ys
  ./disassembler A1TestFiles/zero_2_test.mem A1OutFiles/ysFiles/out_zero_2_test.ys
  ./disassembler A1TestFiles/zero_1_test.mem A1OutFiles/ysFiles/out_zero_1_test.ys
  ./disassembler A1TestFiles/bigtest.mem A1OutFiles/ysFiles/out_bigtest.ys
  ./disassembler A1TestFiles/error.mem A1OutFiles/ysFiles/out_error.ys
  ./disassembler A1TestFiles/max_64.mem A1OutFiles/ysFiles/out_max_64.ys
  ./disassembler A1TestFiles/pipetest.mem A1OutFiles/ysFiles/out_pipetest.ys
  ./disassembler A1TestFiles/poptest.mem A1OutFiles/ysFiles/out_poptest.ys
  ./disassembler A1TestFiles/sort_64.mem A1OutFiles/ysFiles/out_sort_64.ys
  ./disassembler A1TestFiles/sum_64.mem A1OutFiles/ysFiles/out_sum_64.ys
  ./disassembler A1TestFiles/sumjmp.mem A1OutFiles/ysFiles/out_sumjmp.ys

else
rm -f A1OutFiles/*
  ./disassembler A1TestFiles/$1.mem A1OutFiles/ysFiles/out_$1.ys
fi