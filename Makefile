CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
OBJS = main.o serial.o

all: ${OBJS}
	$(CC) -o main ${OBJS}
clean:
	rm -f ${OBJS}
