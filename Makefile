CC     := gcc
TARGET := jellyfish
PARSER := parser
LEXER  := lexer
BISON  := bison
FLEX   := flex
MAIN   := main
HASH	 := hashtable
SYMBOL := symbol_table

all : $(TARGET)

run : $(TARGET)
	./$(TARGET)

$(TARGET) : $(PARSER).c $(LEXER).c $(MAIN).c $(HASH).c $(SYMBOL).c
	$(CC) -g -o $(TARGET) $(PARSER).c $(LEXER).c $(MAIN).c $(HASH).c $(SYMBOL).c

$(PARSER).c : $(PARSER).y
	$(BISON) -o $(PARSER).c -d $(PARSER).y

# -I : special interactive scanner. pre-fetch token only for needed.
$(LEXER).c : $(LEXER).l
	$(FLEX) -o $(LEXER).c -I $(LEXER).l

clean : 
	rm $(TARGET) $(PARSER).c $(PARSER).h $(LEXER).c
