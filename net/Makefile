.PHONY: clean global_clean libnet

libnet: ../libnet.a

../libnet.a: server.o service.o socket.o
	ar ru ../libnet.a server.o service.o socket.o

server.o: server.h server.cpp 

service.o: service.h service.cpp 

socket.o: socket.h socket.cpp

global_clean: clean

clean:
	rm -f *.o
