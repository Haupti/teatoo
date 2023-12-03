BUILD=build
TEST=test
SRC=src

SOURCES=$(wildcard $(SRC)/*.c)
TEST_SOURCES=$(wildcard $(TEST)/*.c)

build_prod:
	gcc -o ./$(BUILD)/tt \
		$(SOURCES) main.c

build_test:
	gcc -o ./$(BUILD)/test \
		$(SOURCES) $(TEST_SOURCES)

test: build_test
	./$(BUILD)/test

exec:
	./$(BUILD)/tt test.tt

run: build_prod exec

