CXX = g++
CXXFLAGS = -Wall

mytest: mytest.cpp driver.cpp squeue.o
	$(CXX) $(CXXFLAGS) mytest.cpp driver.cpp squeue.o -o mytest

squeue.o: squeue.h squeue.cpp
	$(CXX) $(CXXFLAGS) -c squeue.cpp

run:
	./mytest

val:
	valgrind ./mytest

clean:
	rm *~
	rm *.o
