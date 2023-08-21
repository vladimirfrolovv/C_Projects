CC=gcc -std=c11 -g
CFLAGS=-Wall -Werror -Wextra
GCOVFLAGS = -fprofile-arcs -ftest-coverage
SRCS=$(wildcard s21_*.c)
OBJS=$(SRCS:.c=.o)
TESTSFLAGS_MAC= -lcheck 
# -lpthread -lsubunit -lrt -lm
LIB_NAME=s21_matrix

all: clean test

to_style:
	clang-format -style=google -i *.c *.h

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIB_NAME).a: $(OBJS)
	ar rc $(LIB_NAME).a $^
	ranlib $(LIB_NAME).a
	rm -rf *.o

test: $(LIB_NAME).a 
	$(CC) $(CFLAGS) test.c -L. $(LIB_NAME).a $(TESTSFLAGS_MAC) -o test
	./test


add_coverage_flag:
	$(eval CFLAGS += --coverage)

gcov_report: add_coverage_flag test
	./test
	lcov -t "test" -o Coverage_Report.info -c -d .
	genhtml -o ./report Coverage_Report.info
	rm -f *.gcno *.gcda *.info report.out *.gcov *.a
	open ./report/index-sort-f.html

clean:
	rm -rf *.o test *.a *.gcno *.gcda *.gcov *.html *.css *.out *.info

rebuild: all 