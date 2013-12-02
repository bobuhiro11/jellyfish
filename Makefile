CC     := gcc
TARGET := jellyfish
PARSER := parser
LEXER  := lexer
BISON  := bison
FLEX   := flex
MAIN   := main

all : $(TARGET)

run : $(TARGET)
	./$(TARGET)

$(TARGET) : $(PARSER).c $(LEXER).c $(MAIN).c
	$(CC) -g -o $(TARGET) $(PARSER).c $(LEXER).c $(MAIN).c

$(PARSER).c : $(PARSER).y
	$(BISON) -o $(PARSER).c -d $(PARSER).y

# -I : special interactive scanner. pre-fetch token only for needed.
$(LEXER).c : $(LEXER).l
	$(FLEX) -o $(LEXER).c -I $(LEXER).l

clean : 
	rm $(TARGET) $(PARSER).c $(PARSER).h $(LEXER).c
