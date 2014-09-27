CXX := clang
TAB := algoTableau
PTR := algoPointeur
 
SRC = algoTableau.c  $(PTR).c  $(PTR)2.c $(PTR)3.c $(PTR)4.c
OBJ = $(SRC:.c=.o)
BIN = $(OBJ:.o=)

all: $(OBJ) $(BIN)

%.o: %.c
	$(CXX) -Wall -c $<
	
%: %.o
	$(CXX) -o  $@  $^
	
clean:
	rm -f *~
	rm -f $(OBJ)
	
veryclean: clean
	rm -f $(BIN) 
