#!/bin/bash

function test1_ {

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
echo "" > log.txt

for var in -v -c -l -n -h -o
do
  for var2 in -v -c -l -n -h -o
  do
        if [ $var != $var2 ]
        then
          TEST1="for s21_grep.c s21_grep.h Makefile $var $var2"
          echo "$TEST1"
          ./s21_grep $TEST1 > s21_grep.txt
          grep $TEST1 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
          # if [ "$DIFF_RES" == "Файлы s21_grep.txt и grep.txt идентичны" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST1" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep.txt grep.txt

          TEST2="for s21_grep.c $var $var2"
          echo "$TEST2"
          ./s21_grep $TEST2 > s21_grep.txt
          grep $TEST2 > grep.txt
          DIFF_RES="$(diff -s s21_grep.txt grep.txt)"
          if [ "$DIFF_RES" == "Files s21_grep.txt and grep.txt are identical" ]
          # if [ "$DIFF_RES" == "Файлы s21_grep.txt и grep.txt идентичны" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "$TEST2" >> log.txt
              (( COUNTER_FAIL++ ))
          fi
          rm s21_grep.txt grep.txt
        fi
  done
done

echo "SUCCESS: $COUNTER_SUCCESS"
echo "FAIL: $COUNTER_FAIL"

echo "SUCCESS: $COUNTER_SUCCESS" >> result.txt
echo "FAIL: $COUNTER_FAIL" >> result.txt

if (( COUNTER_FAIL > 0 )); then
  COUNTER_FAIL=1
fi

return $COUNTER_FAIL

}
