#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#define buff_size 1024
int connection_closed = 0;
void *sock_send(void *cli_socket)
{
	char buffer[buff_size] = {0,};
	int sock = *((int*)cli_socket);
	while(!connection_closed)
	{
		scanf("%s", buffer);
		strcat(buffer, "\r\n");
		if(send(sock, buffer, strlen(buffer), 0) < 0) printf("Failed send message!\n");
		memset(buffer, 0, buff_size);
	}
	return 0;
}
int main(int argc, char *argv[])
{
	char* server_ip;
	char buffer[buff_size] = {0,};
	int port, cli_socket, thread_err = 0;
	struct sockaddr_in ser_addr;
	pthread_t thread;
	if(argc != 3)
	{
		printf("[syntax error]\n");
		printf("syntax : echo-client <ip> <port>\n");
		printf("sample : echo-client 192.168.10.2 1234\n");
		return -1;
	}
	server_ip = argv[1];
	port = atoi(argv[2]);
	cli_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(cli_socket == 0)
	{
		printf("Failed create socket!\n");
		return -1;
	}
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(port);

	if(inet_pton(AF_INET, server_ip, &(ser_addr.sin_addr)) <= 0)
	{
		printf("Failed connect to %s\n", server_ip);
		return -1;	
	}
	if(connect(cli_socket, (struct sockaddr *)&ser_addr, sizeof(ser_addr)) < 0)
	{
		printf("Failed connect to server!\n");
		return -1;
	}
	
	thread_err = pthread_create(&thread, NULL, sock_send, &cli_socket);
	if(thread_err!=0)
	{
		printf("Failed create thread!\n");
		close(cli_socket);
		return -1;
	}
	int receive = 0;
	while(true)
	{
		receive = read(cli_socket, buffer, buff_size);
		if(receive == 0)
		{
			printf("Connection closed!\n");
			connection_closed = 1;
			break;
		}
		printf("%s", buffer);
		memset(buffer, 0, buff_size);
	}
	thread_err = pthread_join(thread, NULL);
	if(thread_err != 0) printf("Failed join thread!\n");
	close(cli_socket);
	return 0;
}
