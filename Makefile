CC=g++
CPPFLAGS=-g -Wall

trainer: trainer.o NNet.o resultChecker.o

trainer.o: NNet.h resultChecker.h

NNet.o: NNet.h

resultChecker.o: resultChecker.h

clean:
	rm -f *.o trainer
