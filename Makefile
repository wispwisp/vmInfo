.PHONY: all clean

SERVER:=server
CLIENT:=vmInfo
TEST:=test

CPP:=gcc
CFLAGS:=-lstdc++ -std=c++14 -ggdb -Wall -Wextra -Wshadow -pedantic -Weffc++ -O0
LIBS:=-lpugixml -lvirt -lboost_system

all: $(SERVER) $(CLIENT) $(TEST)

$(SERVER): Server/main.cpp Server/Server.cpp Command/Command.cpp Communication/Request.cpp Communication/Result.cpp
	$(CPP) $(CFLAGS) -o $@.out $^ $(LIBS)

$(CLIENT): Client/main.cpp Client/Responce.cpp Client/Body.cpp
	$(CPP) $(CFLAGS) -o $@.out $^ -lpugixml -lboost_system -lpthread

$(TEST): test.cpp Command/Command.cpp Communication/Request.cpp Communication/Result.cpp
	$(CPP) $(CFLAGS) -o $@.out $^ $(LIBS) -lboost_unit_test_framework

clean:
	rm -f $(SERVER).out $(TEST).out $(CLIENT).out *.o *~
