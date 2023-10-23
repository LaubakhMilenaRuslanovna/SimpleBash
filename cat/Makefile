CC =gcc -std=c11
CFLAGS =-Wall -Werror -Wextra

all: clean s21_cat

clean:
	rm -rf s21_cat log.txt *.out
	
rebuild: 
	$(MAKE) clean
	$(MAKE) all

s21_cat:
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

test:
	$(MAKE) all
	sh testing_cat.sh
