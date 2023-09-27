.PHONY:all
all:IpcShmCli IpcShmSer

IpcShmCli:IpcShmCli.cc
	g++ -o $@ $^ -std=c++11
IpcShmSer:IpcShmSer.cc
	g++ -o $@ $^ -std=c++11
.PHONY:clean
clean:
	rm -rf IpcShmCli IpcShmSer .fifo