CC     				:= gcc
TARGET 				:= jellyfish
TARGET_NOFREE := jellyfish_nofree # no memory management
PARSER 				:= parser
LEXER  				:= lexer
BISON  				:= bison
FLEX   				:= flex
MAIN   				:= main
HASH	 				:= hashtable
SYMBOL 				:= symbol_table
CLAGS 				:= -O0

all : $(TARGET) $(TARGET_NOFREE)

run : $(TARGET)
	./$(TARGET)

test : $(TARGET_NOFREE) test.scm
	./$(TARGET_NOFREE) test.scm

$(TARGET_NOFREE) : $(PARSER).c $(LEXER).c $(MAIN).c $(HASH).c $(SYMBOL).c
	$(CC) -g -o $(CFLAGS) $(TARGET_NOFREE) -DNOFREE $(PARSER).c $(LEXER).c $(MAIN).c $(HASH).c $(SYMBOL).c

$(TARGET) : $(PARSER).c $(LEXER).c $(MAIN).c $(HASH).c $(SYMBOL).c
	$(CC) -g -o $(CFLAGS) $(TARGET) $(PARSER).c $(LEXER).c $(MAIN).c $(HASH).c $(SYMBOL).c

$(PARSER).c : $(PARSER).y
	$(BISON) -o $(PARSER).c -d $(PARSER).y

# -I : special interactive scanner. pre-fetch token only for needed.
$(LEXER).c : $(LEXER).l
	$(FLEX) -o $(LEXER).c -I $(LEXER).l

clean :
	rm $(TARGET) $(TARGET_NOFREE) $(PARSER).c $(PARSER).h $(LEXER).c
