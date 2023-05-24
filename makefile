#makefile

all: echo-server echo-client

echo-server : server_main.cpp
	gcc -o echo-server server_main.cpp

echo-client : client_main.cpp
	gcc -o echo-client client_main.cpp

clean :
	rm -f echo-server
	rm -f echo-client
	rm -f *.o
