#makefile

all: echo-client-server

echo-client-server : main.cpp
	gcc -o echo-client-server main.cpp

clean :
	rm -f echo-client-server
	rm -f *.o
