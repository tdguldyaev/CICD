#!/bin/bash

. ./test1.sh

. ./my_test.sh

. ./t_grep.sh

error=0

function main() {

test1_
fail1=$?

my_test_
fail2=$?

t_grep_
fail3=$?


total_fail=$(($fail1+$fail2+$fail3))

echo $total_fail



if (( total_fail > 0 )); then
  error=1
fi

return $error

}

main
