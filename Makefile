CC            := gcc
#CFLAGS         := -Wall
BISON         := bison
FLEX          := flex
TARGET        := bin/jellyfish

SOURCES_C     := $(wildcard src/*.c)
SOURCES_C     += src/parser.c src/lexer.c

TEST          := test/test_integ.scm

all : $(TARGET)

run : $(TARGET)
	$(TARGET)

test : $(TARGET) $(TEST)
	$(TARGET) $(TEST)

$(TARGET) : $(SOURCES_C)
	$(CC) -g -o $(CFLAGS) $(TARGET) $(SOURCES_C)

src/parser.c : src/parser.y
	$(BISON) -d src/parser.y

src/lexer.c : src/lexer.l
	$(FLEX) -o src/lexer.c -I src/lexer.l

clean :
	rm src/lexer.c src/parser.c include/parser.h bin/*
