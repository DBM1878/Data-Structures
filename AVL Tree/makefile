CXX = g++
CXXFLAGS = -Wall

mytest: mytest.cpp driver.cpp fleet.o
	$(CXX) $(CXXFLAGS) mytest.cpp driver.cpp fleet.o -o mytest

fleet.o: fleet.h fleet.cpp
	$(CXX) $(CXXFLAGS) -c fleet.cpp

run:
	./mytest

val:
	valgrind ./mytest

clean:
	rm *~
	rm *.o
