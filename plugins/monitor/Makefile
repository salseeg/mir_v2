.PHONY: clean global_clean monitor

monitor: ../monitor.so

global_clean: clean

clean:
	rm -f *.o

file_sender.o: file_sender.cpp file_sender.h

../monitor.so: monitor.cpp file_sender.o
	$(CXX) $(CXXFLAGS) $(LIBS) -fPIC -shared -o ../monitor.so monitor.cpp $(L_LIBS)
