CC ?= cc
CFLAGS ?= -std=c17 -Wall -Wextra -Wpedantic -O2
CPPFLAGS ?= -Iinclude

.PHONY: all clean test

all: textle

textle: src/textle.c src/feedback.c include/textle.h
	$(CC) $(CPPFLAGS) $(CFLAGS) src/textle.c src/feedback.c -o $@

tests/test_feedback: tests/test_feedback.c src/feedback.c include/textle.h
	$(CC) $(CPPFLAGS) $(CFLAGS) tests/test_feedback.c src/feedback.c -o $@

test: tests/test_feedback
	./tests/test_feedback

clean:
	rm -f textle tests/test_feedback
