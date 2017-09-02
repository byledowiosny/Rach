BASE   = /home/jerzy/bin
CC     = gcc
CFLAGS = -O -Wall -g
EFILE  = $(BASE)/compta
INCLS  = 
LIBS   =
LOC    =
OBJS   = date.o result.o main.o tva.o input.o bilan.o plan.o            	 rapport.o open.o stock.o menu.o prix.o new.o
$(EFILE): $(OBJS)
	@echo "linking ..."
	@$(CC) $(CFLAGS) -o $(EFILE) $(OBJS) $(LIBS)
$(OBJS): C.h
	$(CC) $(CFLAGS) $(INCLS)  -c $*.c
