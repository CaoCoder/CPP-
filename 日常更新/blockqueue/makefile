CC=g++
FLAGS=-std=c++11
LD=-lpthread
bin=blockQueue
src=BlockQueueTest.cc

$(bin):$(src)
	$(CC) -o $@ $^ $(LD) $(FLAGS)
.PHONY:clean
clean:
	rm -f $(bin)