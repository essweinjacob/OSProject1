CC = gcc
CFLAGS = -g
TARGET = bt
OBJS = OSP1.o 
$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
OSP1.o: OSP1.c
	$(CC) $(CFLAGS) -c OSP1.c
clean:
	/bin/rm -f *.o $(TARGET)
