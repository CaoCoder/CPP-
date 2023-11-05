.PHONY:all
all:clientTcp serverTcpd

clientTcp: clientTcp.cc
	g++ -o $@ $^ -std=c++11
serverTcpd: serverTcpd.cc
	g++ -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f serverTcpd clientTcp