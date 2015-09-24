all:
	g++ -std=c++0x -pthread main.cpp SerialPort.c Robot.cpp Nota.cpp -o main
clean:
	rm Robot.o
