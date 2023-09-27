.PHONY:all
all:clientFifo serverFifo
clientFifo:clientFifo.cpp
	g++ -o clientFifo clientFifo.cpp -std=c++11
serverFifo:serverFifo.cpp
	g++ -o serverFifo serverFifo.cpp -std=c++11

.PHONY:clean
clean:
	rm -rf serverFifo clientFifo

