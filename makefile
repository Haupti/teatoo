BUILD=build
TEST=test
SRC=src
LIB=lib

SOURCES=$(wildcard $(SRC)/**/**/*.c) $(wildcard $(SRC)/**/*.c) $(wildcard $(SRC)/*.c)
TEST_SOURCES=$(wildcard $(TEST)/**/**/*.c) $(wildcard $(TEST)/**/*.c) $(wildcard $(TEST)/*.c)
LIB_SRC=$(wildcard $(LIB)/src/**/*.c) $(wildcard $(LIB)/src/*.c)
LIB_TEST=$(wildcard $(LIB)/test/**/*.c) $(wildcard $(LIB)/test/*.c)

build_prod:
	gcc -o ./$(BUILD)/tt \
		$(SOURCES) $(LIB_SRC) main.c

build_test:
	gcc -o ./$(BUILD)/test \
		$(SOURCES) $(LIB_SRC) $(LIB_TEST) $(TEST_SOURCES)

test: build_test
	./$(BUILD)/test

exec:
	./$(BUILD)/tt test.tt

run: build_prod exec
