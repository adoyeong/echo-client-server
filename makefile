#makefile

all: echo-server

echo-server : server_main.cpp
	gcc -o echo-server server_main.cpp

clean :
	rm -f echo-server
	rm -f *.o
