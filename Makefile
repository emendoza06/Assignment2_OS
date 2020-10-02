CC = gcc 
CFLAGS = -g
TARGET = master 
OBJS = master.o palin.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

master.o: master.c
	$(CC) $(CFLAGS) -c master.c

palin.o: palin.c
	$(CC) $(CFLAGS) -c palin.c

clean: 
	/bin/rm -f *.o $(TARGET)
