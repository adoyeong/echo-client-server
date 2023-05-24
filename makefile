#makefile

all: echo-server

echo-server : main.cpp
	gcc -o echo-server main.cpp

clean :
	rm -f echo-server
	rm -f *.o
