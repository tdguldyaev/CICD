#!/bin/bash

function my_test_ {

COUNTER_SUCCESS=0
COUNTER_FAIL=0
DIFF_RES=""
echo "" > log.txt

for var in -e -i -v -c -l -n
      do
        
        TEST1="$var hi 1.txt"
        
          echo "$TEST1"
          ./s21_grep $TEST1 > 1
          grep $TEST1 > 2
          DIFF_RES="$(diff -s 1 2)"
          if [ "$DIFF_RES" == "Files 1 and 2 are identical" ]
          # if [ "$DIFF_RES" == "Файлы 1 и 2 идентичны" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "FAIL"
              (( COUNTER_FAIL++ ))
          fi
          rm 1 2
        
        TEST2="$var hi 2.txt"
        
          echo "$TEST2"
          ./s21_grep $TEST2 > 1
          grep $TEST2 > 2
          DIFF_RES="$(diff -s 1 2)"
          if [ "$DIFF_RES" == "Files 1 and 2 are identical" ]
          # if [ "$DIFF_RES" == "Файлы 1 и 2 идентичны" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "FAIL"
              (( COUNTER_FAIL++ ))
          fi
          rm 1 2

        TEST3="$var hi 3.txt"
        
          echo "$TEST3"
          ./s21_grep $TEST3 > 1
          grep $TEST3 > 2
          DIFF_RES="$(diff -s 1 2)"
          if [ "$DIFF_RES" == "Files 1 and 2 are identical" ]
          # if [ "$DIFF_RES" == "Файлы 1 и 2 идентичны" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "FAIL"
              (( COUNTER_FAIL++ ))
          fi
          rm 1 2

          TEST4="$var hey 1.txt 2.txt"
        
          echo "$TEST4"
          ./s21_grep $TEST4 > 1
          grep $TEST4 > 2
          DIFF_RES="$(diff -s 1 2)"
          if [ "$DIFF_RES" == "Files 1 and 2 are identical" ]
          # if [ "$DIFF_RES" == "Файлы 1 и 2 идентичны" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "FAIL"
              (( COUNTER_FAIL++ ))
          fi
          rm 1 2

          TEST5="$var hey 1.txt 3.txt"
        
          echo "$TEST5"
          ./s21_grep $TEST5 > 1
          grep $TEST5 > 2
          DIFF_RES="$(diff -s 1 2)"
          if [ "$DIFF_RES" == "Files 1 and 2 are identical" ]
          # if [ "$DIFF_RES" == "Файлы 1 и 2 идентичны" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "FAIL"
              (( COUNTER_FAIL++ ))
          fi
          rm 1 2

          TEST6="$var hey 2.txt 3.txt"
        
          echo "$TEST6"
          ./s21_grep $TEST6 > 1
          grep $TEST6 > 2
          DIFF_RES="$(diff -s 1 2)"
          if [ "$DIFF_RES" == "Files 1 and 2 are identical" ]
          # if [ "$DIFF_RES" == "Файлы 1 и 2 идентичны" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "FAIL"
              (( COUNTER_FAIL++ ))
          fi
          rm 1 2


          TEST7="$var hey 1.txt 2.txt 3.txt"
        
          echo "$TEST7"
          ./s21_grep $TEST7 > 1
          grep $TEST7 > 2
          DIFF_RES="$(diff -s 1 2)"
          if [ "$DIFF_RES" == "Files 1 and 2 are identical" ]
          # if [ "$DIFF_RES" == "Файлы 1 и 2 идентичны" ]
            then
              (( COUNTER_SUCCESS++ ))
            else
              echo "FAIL"
              (( COUNTER_FAIL++ ))
          fi
          rm 1 2
        
    done
    for var1 in -e -i -v -c -l -n; do
    for pattern in "[0-9]" "[a-z]" "void" "21"; do
        TEST8="flag: $var1 pattern: $pattern"
        echo "$TEST8"
        ./s21_grep "$var1" "$pattern" s21_grep.c > 1
        grep "$var1" "$pattern" s21_grep.c > 2
        DIFF_RES="$(diff -s 1 2)"
        if [ "$DIFF_RES" == "Files 1 and 2 are identical" ]; then
        # if [ "$DIFF_RES" == "Файлы 1 и 2 идентичны" ]; then
        (( COUNTER_SUCCESS++ ))
        else
        echo "FAIL"
        (( COUNTER_FAIL ++))
        fi
        rm 1 2
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
        
