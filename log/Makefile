.PHONY: clean global_clean liblog

liblog: ../liblog.a

../liblog.a: log.o
	ar ru ../liblog.a log.o

log.o: log.h log.cpp

global_clean: clean

clean:
	rm -f *.o
