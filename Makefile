COMPILER = gcc
CCFLAGS = -Wall -g
AR = ar

all: rminq

rminq: test.o librmq.a
	$(COMPILER) $(CCFLAGS) test.o librmq.a -o rminq -lm

librmq.a: rmq.o
	$(AR) -r librmq.a rmq.o

rmq.o:
	$(COMPILER) $(CCFLAGS) -c rmq.c rmq.h

test.o:
	$(COMPILER) $(CCFLAGS) -c test.c

clean:
	rm *.o *.a *.gch rminq
