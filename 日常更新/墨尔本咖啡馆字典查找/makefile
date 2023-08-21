

# all:dict1

dict1:test.o dict1.o
	gcc -o dict1 test.o dict1.o 
test.o:test.c
	gcc -c -o test.o test.c
dict1.o:dict1.c
	gcc -c -o dict1.o dict1.c

.PHONY:clean
clean:
	rm -rf test.o dict1.o dict1