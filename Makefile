CC     := gcc
TARGET := lisp
PARSER := parser
LEXER  := lexer
BISON  := bison
FLEX   := flex

all : $(TARGET)

run : $(TARGET)
	./$(TARGET)

$(TARGET) : $(PARSER).c $(LEXER).c
	$(CC) -o $(TARGET) $(PARSER).c $(LEXER).c

$(PARSER).c : $(PARSER).y
	$(BISON) -o $(PARSER).c -d $(PARSER).y

# -I : $BI,MW$J>l9g$K$7$+@hFI$_$r$7$J$$FCJL$JBPOC7?%9%-%c%J(B
$(LEXER).c : $(LEXER).l
	$(FLEX) -o $(LEXER).c -I $(LEXER).l

clean : 
	rm $(TARGET) $(PARSER).c $(PARSER).h $(LEXER).c
