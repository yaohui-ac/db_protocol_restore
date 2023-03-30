CC = g++
LIB = -L ../lib
CFLAGS = -I ../include -std=c++17 -O3

rest_server: rest_server.cpp
	$(CC) $(CFLAGS) $< -o $@ $(LIB) -lpistache
