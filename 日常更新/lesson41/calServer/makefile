.PHONY:all
all:clientTcp serverTcp
#-D选项就是 命令行定义这个值 意思就是采用自己 造的轮子。
Method=-DMY_SELF 

clientTcp: clientTcp.cc
	g++ -o $@ $^ $(Method) -std=c++11 -ljsoncpp
serverTcp: serverTcp.cc
	g++ -o $@ $^ $(Method) -std=c++11 -lpthread -ljsoncpp

.PHONY:clean
clean:
	rm -f serverTcp clientTcp


