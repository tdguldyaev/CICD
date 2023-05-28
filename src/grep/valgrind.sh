#!/bin/bash

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
echo "" > log.txt

for var in -v -c -l -n -h -o
do
  for var2 in -v -c -l -n -h -o
  do
      for var3 in -v -c -l -n -h -o
      do
        if [ $var != $var2 ] && [ $var2 != $var3 ] && [ $var != $var3 ]
        then
          TEST1="for s21_grep.c s21_grep.h $var $var2 $var3"
          echo "$TEST1"
          valgrind --leak-check=full ./s21_grep $TEST1 >> valgrind_output.txt 2>&1

          TEST2="for s21_grep.c $var $var2 $var3"
          echo "$TEST2"
          valgrind --leak-check=full ./s21_grep $TEST2 >> valgrind_output.txt 2>&1

          TEST3="-e for -e ^regex s21_grep.c s21_grep.h $var $var2 $var3"
          echo "$TEST3"
          valgrind --leak-check=full ./s21_grep $TEST3 >> valgrind_output.txt 2>&1

          TEST4="-e for -e hey s21_grep.c $var $var2 $var3"
          echo "$TEST4"
          valgrind --leak-check=full ./s21_grep $TEST1 >> valgrind_output.txt 2>&1

          TEST5="-e for -e ^regex s21_grep.c $var $var2 $var3 -f pattern.txt"
          echo "$TEST5"
          valgrind --leak-check=full ./s21_grep $TEST1 >> valgrind_output.txt 2>&1

          TEST6="-e hey -e regex s21_grep.c s21_grep.h Makefile $var $var2 $var3 -f pattern.txt"
          echo "$TEST6"
          valgrind --leak-check=full ./s21_grep $TEST1 >> valgrind_output.txt 2>&1

        fi
      done
  done
done

success=$(grep "ERROR SUMMARY: 0" valgrind_output.txt -c)


if [ $success -eq 720 ]; then
  echo "ALL OK"
else
  echo "FAIL"
fi
