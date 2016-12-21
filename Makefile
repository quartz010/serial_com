OBJS = main.o serial.o
all: ${OBJS}
	cc -o main ${OBJS}
clean:
	rm -f ${OBJS}
