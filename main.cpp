#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define op_e "-e"
#define op_b "-b"

void err_msg(void)
{
	printf("[syntax error]\n");
        printf("syntax : echo-server <port> [-e[-b]]\n");
        printf("sample : echo-server 1234 -e -b\n");
}
void *test(void *type)
{
	int i;
	int id = *((int*)type);
	return 0;
}
int main(int argc, char* argv[])
{
	int i;
	int echo = 0;
	int broad = 0;

        if(argc < 2)
        {
		err_msg();
                return -1;
        }
	for(i=2; i<argc; i++)
	{
		if(!strcmp(argv[i], op_e)) echo == 1;
		else if(!strcmp(argv[i], op_b)) broad == 1;
		else
		{
			err_msg();
			return -1;
		}
	}
	printf("OK\n");
	pthread_t A, B;
	int thread_err = 0;
	int a = 1, b= 2;
	thread_err = pthread_create(&A, NULL, test, &a);
	thread_err = pthread_create(&B, NULL, test, &b);
	thread_err = pthread_join(A, NULL);
	thread_err = pthread_join(B, NULL);
	return 0;
}

