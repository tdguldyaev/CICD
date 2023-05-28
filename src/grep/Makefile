CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
# CFLAGS = -Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

make all: clean s21_grep

s21_grep: clean
	$(CC) $(CFLAGS) ./s21_grep.c -o ./s21_grep

test: s21_grep
	$(CC) $(CFLAGS) ./s21_grep.c -o ./s21_grep
	bash testing.sh

valgrind: s21_grep
	bash valgrind.sh

style:
	cp ../../materials/linters/.clang-format .
	clang-format -n *.c *.h
	rm -f .clang-format

clean:
	rm -f s21_grep
	rm -f log.txt
	rm -f result.txt
	rm -f valgrind_output.txt
