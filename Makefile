CC=gcc

all:
	$(CC) -o tictactoe tictactoe.c
clean:
	rm -f tictactoe
