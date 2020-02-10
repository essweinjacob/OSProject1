output: bt.o
	gcc bt.o -o output

bt.o: OSP1.c
	gcc -c OSP1.c

target: dependencies
	action

clean:
	rm *.o output
