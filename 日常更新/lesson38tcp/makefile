
.PHONY:all
all:clientTcp serverTcp

clientTcp:clientTcp.cc
	g++ -o $@ $^ -std=c++11
serverTcp: serverTcp.cc
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f serverTcp clientTcp