CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11

make all: clean s21_cat

s21_cat: clean
	$(CC) $(CFLAGS) ./s21_cat.c -o ./s21_cat
	
test:
	$(CC) $(CFLAGS) ./s21_cat.c -o ./s21_cat 
	bash ./t_cat_2.sh

style:
	cp ../../materials/linters/.clang-format .
	clang-format -n *.c *.h
	rm -f .clang-format

clean:
	rm -f s21_cat
	rm -f log.txt
