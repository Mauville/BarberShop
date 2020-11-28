# change application name here (executable output name)
TARGET=cortapelo

# compiler
CC=gcc
# debug
DEBUG=-g
# optimisation
OPT=-O0
# warnings
WARN=-Wall -Wextra

PTHREAD=-pthread

CCFLAGS=$(DEBUG) $(OPT) $(WARN) $(PTHREAD) -pipe

GTKLIB=`pkg-config --cflags --libs gtk+-3.0`

# linker
LD=gcc
LDFLAGS=$(PTHREAD) $(GTKLIB) -export-dynamic

OBJS=    main.o

model: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(LDFLAGS)
	./cortapelo
    
main.o: src/GUI.c
	$(CC) -c $(CCFLAGS) src/GUI.c $(GTKLIB) -o main.o
    
clean:
	gcc -o limpia src/limpia.c -lpthread
	./limpia
	rm -f *.o $(TARGET)
	rm -f limpia controller

controller:
	gcc -o controller src/Controller.c -lpthread
	./main
