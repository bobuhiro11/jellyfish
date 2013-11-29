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

# -I : 必要な場合にしか先読みをしない特別な対話型スキャナ
$(LEXER).c : $(LEXER).l
	$(FLEX) -o $(LEXER).c -I $(LEXER).l

clean : 
	rm $(TARGET) $(PARSER).c $(PARSER).h $(LEXER).c
