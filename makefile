all: assembler

assembler: assembler.o func.o firstpass.o
	gcc -ansi -Wall -pedantic -o assembler assembler.o func.o firstpass.o table.c secondpass.c -lm

assembler.o: assembler.c firstpass.h func.h
	gcc -ansi -Wall -pedantic -c assembler.c -lm

func.o: func.c func.h
	gcc -ansi -Wall -pedantic -c func.c -lm

firstpass.o : firstpass.c firstpass.h
	gcc -ansi -Wall -pedantic -c firstpass.c -lm

table.o : table.c table.h
	gcc -ansi -Wall -pedantic -c table.c -lm

secondpass.o: secondpass.c secondpass.h
	gcc -ansi -Wall -pedantic -c secondpass.c -lm
clean:
	rm -rf *.o *.ob *.ext *.ent

