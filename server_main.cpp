#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>

#define op_e "-e"
#define op_b "-b"
#define buff_size 1024

int client_list[50] = {0,};
int client_num = 0;
void err_msg(void)
{
	printf("[syntax error]\n");
        printf("syntax : echo-server <port> [-e[-b]]\n");
        printf("sample : echo-server 1234 -e -b\n");
}
void broadcast(char *buff, int len)
{
	int i;
	for(i=0; i<client_num; i++)
	{
		send(client_list[i], buff, len, 0);
	}
}
void *sock_read(void *cli_socket)
{
        int receive;
        char buffer[buff_size] = {0,};
        int sock = *((int*)cli_socket);
        while(true)
        {
                receive = read(sock, buffer, buff_size);
                if(receive == 0)
                {
                        printf("One client close connection!\n");
                        break;
                }
                printf("%s", buffer);
                memset(buffer, 0, buff_size);
        }
	close(sock);
        return 0;
}
void *sock_read_broad(void *cli_socket)
{
        int receive;
        char buffer[buff_size] = {0,};
        int sock = *((int*)cli_socket);
        while(true)
        {
                receive = read(sock, buffer, buff_size);
                if(receive == 0)
                {
                        printf("One client close connection!\n");
                        break;
                }
                printf("%s", buffer);
		broadcast(buffer, strlen(buffer));
                memset(buffer, 0, buff_size);
        }
	close(sock);
        return 0;
}
void *sock_read_send(void *cli_socket)
{
	int receive;
	char buffer[buff_size] = {0,};
	int sock = *((int*)cli_socket);
	while(true)
	{
		receive = read(sock, buffer, buff_size);
		if(receive == 0)
		{
			printf("One client close connection!\n");
			break;
		}
		printf("%s", buffer);
		send(sock, buffer, strlen(buffer), 0);
		memset(buffer, 0, buff_size);
	}
	close(sock);
	return 0;
}
int main(int argc, char* argv[])
{
	int i;
	int port;
	int echo = 0;
	int broad = 0;
	pthread_t th[50];
	struct sockaddr_in ser_addr, cli_addr;
	socklen_t seraddr_len, cliaddr_len;
	seraddr_len = sizeof(ser_addr);
	cliaddr_len = sizeof(cli_addr);
        if(argc < 2)
        {
		err_msg();
                return -1;
        }
	for(i=2; i<argc; i++)
	{
		if(!strcmp(argv[i], op_e)) echo = 1;
		else if(!strcmp(argv[i], op_b)) broad = 1;
		else
		{
			err_msg();
			return -1;
		}
	}
	port = atoi(argv[1]);
	printf("%d\n", port);
	printf("OK\n");


	int ser_socket, cli_socket;
	ser_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(ser_socket == 0)
	{
		printf("Failed create socket!\n");
		return -1;
	}
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_addr.s_addr = INADDR_ANY;
	ser_addr.sin_port = htons(port);
	if(bind(ser_socket, (struct sockaddr *)&ser_addr, seraddr_len) <0)
	{
		printf("Failed bind!\n");
		return -1;
	}

	if(listen(ser_socket, 10) < 0)
	{
		printf("Failed listen!\n");
		return -1;
	}
	int th_num = 0;
	int thread_err = 0;
	while(true)
	{
		cli_socket = accept(ser_socket, (struct sockaddr *)&cli_addr, &cliaddr_len);
		if(cli_socket < 0)
		{
			printf("Failed accept!\n");
			return -1;
		}
		client_list[client_num] = cli_socket;
		client_num++;
		if(broad == 1) thread_err = pthread_create(&th[th_num], NULL, sock_read_broad, &cli_socket);
		else if(echo == 1) thread_err = pthread_create(&th[th_num], NULL, sock_read_send, &cli_socket);
		else thread_err = pthread_create(&th[th_num], NULL, sock_read, &cli_socket);
		if(thread_err != 0)
		{
			printf("Failed create thread!\n");
		}
		th_num++;
		thread_err = 0;
	}
	for(i=0; i<th_num; i++)
	{
		thread_err = pthread_join(th[i], NULL);
		if(thread_err != 0) printf("Failed join thread!\n");
	}
	close(ser_socket);
	return 0;
}

