src=$(wildcard src/*.cpp)

server:
	g++ -std=c++11 -pthread -g \
	$(src) \
	server.cpp \
	-o server
	

thread_test:
	g++ -std=c++11 -pthread src/ThreadPool.cpp thread_test.cpp -o thread_test

single_client:
	g++ -std=c++11 src/util.cpp src/Connection.cpp src/Buffer.cpp src/Socket.cpp src/ThreadPool.cpp \
	-pthread \
	single_client.cpp -o single_client

multiple_client:
	g++ -std=c++11 src/util.cpp src/Connection.cpp src/Buffer.cpp src/Socket.cpp src/ThreadPool.cpp \
	-pthread \
	multiple_client.cpp -o multiple_client

clean:
	rm server && rm client && rm test