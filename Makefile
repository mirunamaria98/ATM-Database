build: tema.o
	gcc tema.o -o atm -Wall
tema.o: tema.c
	gcc -c tema.c -Wall
run: build
	./atm
clean:
	rm -f *.o
	rm -f atm
