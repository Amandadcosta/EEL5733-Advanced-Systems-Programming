#compile statements
	CC      = gcc
	RM      = rm -f


default: all
all:transfProg

transfProg: transfProg.c
	$(CC) -pthread -o transfProg transfProg.c

clean veryclean:
	$(RM) transfProg
