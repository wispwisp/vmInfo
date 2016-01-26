.PHONY: all clean

NAME:=vmInfo
CPP:=gcc
CFLAGS:=-lstdc++ -std=c++14 -ggdb -Wall -Wextra -Wshadow -pedantic -Weffc++ -O0
LIBS:=-lpugixml -lvirt -lboost_system

all: $(NAME)

$(NAME): Server/main.cpp Server/Server.cpp Command/Command.cpp Communication/Request.cpp Communication/Result.cpp
	$(CPP) $(CFLAGS) -o $@.out $^ $(LIBS)

client: Client/main.cpp Communication/Request.cpp Communication/Result.cpp
	$(CPP) $(CFLAGS) -o $@.out $^ -lpugixml

test: test.cpp Command/Command.cpp Communication/Request.cpp Communication/Result.cpp
	$(CPP) $(CFLAGS) -o $@.out $^ $(LIBS) -lboost_unit_test_framework

clean:
	rm -f $(NAME).out test.out client.out *.o *~
