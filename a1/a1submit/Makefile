CXX = g++
CXXFLAGS = -g -std=c++11	
RM = rm

all : server client

server : server.cpp
	${CXX} ${CXXFLAGS} server.cpp -o server
    
client : client.cpp
	${CXX} ${CXXFLAGS} client.cpp -o client
    
.PHONY : all clean

clean : 
	$(RM) server client

