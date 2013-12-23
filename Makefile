CC            := gcc
CFLAGS        := -g
BISON         := bison
FLEX          := flex
TARGET        := bin/jellyfish

SOURCES_C     := $(wildcard src/*.c)
SOURCES_C     += src/parser.c src/lexer.c
SOURCES_C     := $(sort $(SOURCES_C))     # to prevent duplication

TEST          := test/test_integ.scm

all : $(TARGET)

run : $(TARGET)
	$(TARGET)

test : $(TARGET) $(TEST)
	$(TARGET) $(TEST)

$(TARGET) : $(SOURCES_C)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES_C)

src/parser.c : src/parser.y
	$(BISON) -d src/parser.y -o src/parser.c --defines="include/parser.h"

src/lexer.c : src/lexer.l
	$(FLEX) -o src/lexer.c -I src/lexer.l

clean :
	rm -rf src/lexer.c src/parser.c include/parser.h bin/*
