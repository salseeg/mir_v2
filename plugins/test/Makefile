.PHONY : clean global_clean test

test: ../test.so

../test.so: test.cpp 
	$(CXX) $(CXXFLAGS) -fPIC -shared -o ../test.so test.cpp 

global_clean: clean

clean:
