CC =gcc -std=c11
CFLAGS =-Wall -Werror -Wextra

all: clean s21_grep

clean:
	rm -rf s21_grep log.txt *.out
	
rebuild: 
	$(MAKE) clean
	$(MAKE) all

s21_grep:
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

test:
	$(MAKE) all
	sh testing_grep.sh
