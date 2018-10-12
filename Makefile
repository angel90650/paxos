#Makefile paxos system

server: server.o ballot.o blockchain.o inbox.o
	g++ -std=c++11 -pthread -g -o server server.o ballot.o blockchain.o inbox.o

ballot.o: ballot.cpp
	g++ -std=c++11 -g -c ballot.cpp

server.o: server.cpp
	g++ -std=c++11 -g -c server.cpp

inbox.o: inbox.cpp
	g++ -std=c++11 -g -c inbox.cpp

blockchain.o: blockchain.cpp
	g++ -std=c++11 -g -c blockchain.cpp


clean:
	rm -f server ballot.o blockchain.o ballot.o inbox.o

