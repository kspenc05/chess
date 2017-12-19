FLAGS = -Wall -pedantic -std=c99 -g

all: move.o check.o board.o player.o direc.o draw.o
	gcc $(FLAGS) \
	    player.o move.o board.o check.o direc.o draw.o chess.c -o chess

board.o: player.o
	gcc -c $(FLAGS) boardFunctions.c -o board.o

check.o: player.o
	gcc -c $(FLAGS) check.c

move.o: player.o
	gcc -c $(FLAGS) movement.c -o move.o
	
player.o:
	gcc -c $(FLAGS) player.c
	
direc.o:
	gcc -c $(FLAGS) directions.c -o direc.o
	
draw.o:
	gcc -c $(FLAGS) draw.c
    
clean:
	rm chess *.o