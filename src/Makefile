.PHONY: all install uninstall clean dvi dist tests check memcheck test
cpp = g++
CFLAGS = -Wall -Werror -Wextra -std=c++17
TESTFLAGS := -lgtest -pthread -lgtest_main
OS = $(shell uname -s)
SOURCES = model/*.cc  controller/*.cc
all: install open

clean:
	rm -rf view/*.o *.gcda *.gcno
	rm -rf test
	rm -rf coverage.info
	rm -rf report
	rm -rf build

rebuild: clean all

tests: clean
	$(cpp) $(CFLAGS) $(SOURCES) tests/tests.cc --coverage $(TESTFLAGS) -o test
	 ./test
test: tests

memcheck: test
ifeq ($(OS), Darwin)
	leaks -atExit -- ./test
else
	valgrind --trace-children=yes --track-fds=yes --track-origins=yes --leak-check=full --show-leak-kinds=all ./test
endif

report: test
		@lcov --capture  --no-external --directory . --output-file coverage.info
		@genhtml coverage.info --output-directory report
		@open report/index.html

check:
	cp ../materials/linters/.clang-format ./
	clang-format -n ./*/*.cc
	clang-format -n ./*/*.h
	rm -rf .clang-format

install: uninstall
	make clean
	mkdir build
	cd view && qmake && make && make clean && rm Makefile && cd ../ && mv view/view build

open:
	./build/view

uninstall:
	rm -rf build*

dist:
	rm -rf Archive_SmartCalc_v2.0/
	mkdir Archive_SmartCalc_v2.0/
	mkdir Archive_SmartCalc_v2.0/src
	mv ./build/Calc.app Archive_SmartCalc_v2.0/src/
	tar cvzf Archive_SmartCalc_v2.0.tgz Archive_SmartCalc_v2.0/
	rm -rf Archive_SmartCalc_v2.0/

dvi:
	open readme_calc.md
