all: integral

main.o : main.c
	gcc -m32 -c -o main.o main.c 
func.o : func.asm
	nasm -f elf32 -o func.o func.asm
integral: main.o func.o
	gcc -o integral main.o func.o -m32 -lm 
	
clean :
	rm -rf main.o func.o 