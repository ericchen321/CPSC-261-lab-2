#!/bin/bash

# argument is all OR the name of the test, eg. bigtest, error, etc.
if [ "$1" == "all" ];
then
  cmp -b A1TestFiles/bigtest.mem A1OutFiles/memFiles/out_bigtest.mem
  cmp -b A1TestFiles/error.mem A1OutFiles/memFiles/out_error.mem
  cmp -b A1TestFiles/max_64.mem A1OutFiles/memFiles/out_max_64.mem
  cmp -b A1TestFiles/pipetest.mem A1OutFiles/memFiles/out_pipetest.mem
  cmp -b A1TestFiles/poptest.mem A1OutFiles/memFiles/out_poptest.mem
  cmp -b A1TestFiles/sort_64.mem A1OutFiles/memFiles/out_sort_64.mem
  cmp -b A1TestFiles/sum_64.mem A1OutFiles/memFiles/out_sum_64.mem
  cmp -b A1TestFiles/sumjmp.mem A1OutFiles/memFiles/out_sumjmp.mem
  cmp -b A1TestFiles/first_byte_non_zero_test.mem A1OutFiles/memFiles/out_first_byte_non_zero_test.mem
  cmp -b A1TestFiles/halt_1_test.mem A1OutFiles/memFiles/out_halt_1_test.mem
  cmp -b A1TestFiles/halt_2_test.mem A1OutFiles/memFiles/out_halt_2_test.mem
  cmp -b A1TestFiles/halt_3_test.mem A1OutFiles/memFiles/out_halt_3_test.mem
  cmp -b A1TestFiles/halt_4_test.mem A1OutFiles/memFiles/out_halt_4_test.mem
  cmp -b A1TestFiles/halt_5_test.mem A1OutFiles/memFiles/out_halt_5_test.mem
  cmp -b A1TestFiles/halt_6_test.mem A1OutFiles/memFiles/out_halt_6_test.mem
  cmp -b A1TestFiles/words_test.mem A1OutFiles/memFiles/out_words_test.mem
  cmp -b A1TestFiles/zero_1_test.mem A1OutFiles/memFiles/out_zero_1_test.mem
  cmp -b A1TestFiles/zero_2_test.mem A1OutFiles/memFiles/out_zero_2_test.mem

else
  cmp -b A1TestFiles/$1.mem A1OutFiles/memFiles/out_$1.mem
fi