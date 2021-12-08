#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <stdbool.h>

#define MAX_CLIENTS 100
#define BUFFER_SZ 2082
#define DATA_SIZE 100
#define TRUE 1
#define MAX 1000

static _Atomic unsigned int clnt_count = 0;
static int uid = 10;

void result_vote()
    {
    char s4[100]; // #stop
    char* s5 = "#stop"; // Comparing with #stop
    char buffer[MAX] = { 0, }; // read vote.txt buf
    FILE* fp = fopen("vote.txt", "r+"); // read 'vote.txt' file

         printf("Enter '#stop' to check the results\n");
    printf("");
    scanf("%s", s4);
    if (strcmp(s4, s5) == 0)
    {
        fread(buffer, 1, MAX, fp); //전체 읽기
        printf("%s", buffer);

        fclose(fp);
    }
    return 0;
}

/*Hash Function: Convert the password into a hash*/
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/* Client structure */
typedef struct{
	struct sockaddr_in address;
	int sockfd;
	int uid;
	char username[32];
    char passwd[32];
} client_t;

client_t *clients[MAX_CLIENTS];

pthread_mutex_t clnt_mutex = PTHREAD_MUTEX_INITIALIZER;

bool is_send_command(char* msg, char** IP, char** PORT, char** filename) {
    char* option;
    char message[BUFFER_SZ + 1] = {};

    strcpy(message, msg);
    message[5] = '\0';
    if (strcmp(message, "SEND ") == 0) {
        option = msg + 5;
        option = strtok(option, " ");
        *IP = option;
        option = strtok(NULL, " ");
        *PORT = option;
        option = strtok(NULL, " ");
        *filename = option;

        return true;
    }

    return false;
}

bool is_vote_command(char* msg, char** question) {
    char* option;
    char message[BUFFER_SZ + 1] = {};

    strcpy(message, msg);
    message[5] = '\0';
    printf("%s\n%s\n", msg, message);
    if (strcmp(message, "VOTE#") == 0) {
        printf("TRUE\n");
        option = msg + 5;
        option = strtok(option, "#");
        *question = option;
        return true;
    }

    return false;
}
void vote(int uid){
    int n, stop;
    printf("Entered Vote Function\n");
    send_message("\n1) YES\n2) NO\n3) NONE\n", uid);
    printf("1) YES\n2) NO\n3) NONE\n");
    printf(""); // input number
    scanf("%d", &n);
    if (n > 0 && n <= 3)
    {
        Vote_data(n);
    }
    else
    {
    printf("Enter Valid input\n");
    }
    Vote_data(stop);


}

void Vote_data(int choice)
{
    int a = 0, b = 0, c = 0;
    FILE* fp = fopen("vote.txt", "r"); 
    if (fp == NULL)
    {
        fp = fopen("vote.txt", "w");
    }
    if (!feof(fp))
    {
        fscanf(fp, "YES = %d\n", &a);
        fscanf(fp, "NO = %d\n", &b);
        fscanf(fp, "NONE = %d\n", &c);
    }
    fp = fopen("vote.txt", "w");
    if (choice == 1)
    {
        a++;
    }
    if (choice == 2)
    {
        b++;
    }
    if (choice == 3)
    {
        c++;
    }
    fprintf(fp, "YES = %d\n", a);
    fprintf(fp, "NO = %d\n", b);
    fprintf(fp, "NONE = %d\n", c);
        if ((a > b) && (a > c))
        {
            fprintf(fp, "YES won with votes of '%d'\n", a);
        }
        else if ((b > a) && (b > c))
        {
            fprintf(fp, "NO won with votes of '%d'\n", b);
        }
        else if ((c > b) && (c > a))
        {
            fprintf(fp, "NONE won with votes of '%d'\n", c);
        }
        else if ((a == b) && (b == c))
        {
            fprintf(fp, "Everything Equal with votes of '%d'\n", a);
        }
        else if (a == b)
        {
            fprintf(fp, "YES and NO Won Equal with votes of '%d' '%d'\n", a, b);
        }
        else if (a == c)
        {
            fprintf(fp, "YES and NONE Won Equal with votes of '%d' '%d'\n", a, c);
        }
        else if (b == c)
        {
            fprintf(fp, "NO and NONE Won Equal with votes of '%d' '%d'\n", b, c);
        }
        fclose(fp);
    }

void download_file(client_t* cli, char* IP, char* PORT) {
    char buffer[BUFFER_SZ + 1] = {};
    char* tmp;
    int l = 0;

    FILE* fp = fopen("download.txt", "wb+");
    int i = 0;
    while (l = recv(cli->sockfd, buffer, BUFFER_SZ, 0)) {
        if ((tmp = strstr(buffer, "*")) != NULL) {
            send_message_to(buffer, IP, PORT);
            break;
        }
        send_message_to(buffer, IP, PORT);
    }
}

void update_log(char* message, char* filename) {
    FILE *fp;
    time_t current = time(NULL);
    struct tm *now = localtime(&current);
    
    fp = fopen(filename, "a");
    fprintf(fp, "[%04d/%02d/%02d] %02d:%02d:%02d %s"
            ,1900 + now->tm_year, now->tm_mon + 1, now->tm_mday
            ,now->tm_hour, now->tm_min, now->tm_sec, message);
    fclose(fp);
}

void str_overwrite_stdout() {
    printf("\r%s", "> ");
    fflush(stdout);
}

void str_trim_lf (char* arr, int length) {
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}

void print_client_addr(struct sockaddr_in serv_addr){
    printf("%d.%d.%d.%d",
        serv_addr.sin_addr.s_addr & 0xff,
        (serv_addr.sin_addr.s_addr & 0xff00) >> 8,
        (serv_addr.sin_addr.s_addr & 0xff0000) >> 16,
        (serv_addr.sin_addr.s_addr & 0xff000000) >> 24);
}

/* Add clients to queue */
void queue_add(client_t *clnt){
	pthread_mutex_lock(&clnt_mutex);

	for(int i=0; i < MAX_CLIENTS; ++i){
		if(!clients[i]){
			clients[i] = clnt;
			break;
		}
	}
	pthread_mutex_unlock(&clnt_mutex);
}

/* Remove clients to queue */
void queue_remove(int uid){
	pthread_mutex_lock(&clnt_mutex);

	for(int i=0; i < MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid == uid){
				clients[i] = NULL;
				break;
			}
		}
	}

	pthread_mutex_unlock(&clnt_mutex);
}

/* Send message to all clients except sender */
void send_message(char *s, int uid){
	pthread_mutex_lock(&clnt_mutex);

	for(int i=0; i<MAX_CLIENTS; ++i){
		if(clients[i]){
			if(clients[i]->uid != uid){
				if(write(clients[i]->sockfd, s, strlen(s)) < 0){
					perror("ERROR: write to descriptor failed");
					break;
				}
			}
		}
	}

	pthread_mutex_unlock(&clnt_mutex);
}

void send_message_to(char* s, char* IP, char* PORT) {
    pthread_mutex_lock(&clnt_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i]) {
            if ((strcmp(inet_ntoa(clients[i]->address.sin_addr), IP) == 0)
                && (clients[i]->address.sin_port == atoi(PORT))) {
                if (write(clients[i]->sockfd, s, strlen(s)) < 0) {
                    perror("ERROR: write to descriptor failed");
                    break;
                }
            }
        }
    }

    pthread_mutex_unlock(&clnt_mutex);
}

/* Handle all communication with the client */
void *handle_client(void *arg){
	char buff_out[BUFFER_SZ];
    char buff_out_vote[BUFFER_SZ];
	char username[32];
    char passwd[32];
	int leave_flag = 0;
    char hashpass2[100];
    char line[1000];
    char* IP, *PORT, *filename, *question;

	clnt_count++;
	client_t *cli = (client_t *)arg;

	// username
	if(recv(cli->sockfd, username, 32, 0) <= 0 || strlen(username) <  2 || strlen(username) >= 32-1){
		printf("Didn't enter the Username.\n");
		leave_flag = 1;
	} 
    else{
        //password
        if(recv(cli->sockfd, passwd, 32, 0) <= 0 || strlen(passwd) <  2 || strlen(passwd) >= 32-1){
            printf("Didn't enter the Password.\n");
		    leave_flag = 1;
	    }
        else{
            strcpy(cli->passwd, passwd);

            // converting the resultant hash(int) to hashpass2(char)
            unsigned long hash1 = hash(passwd);
            snprintf( hashpass2, DATA_SIZE, "%d", hash1 );
            FILE * fPtr;

            
            /*
            * Open file in r (read) mode.
            * "user_auth.txt" is relative path to create file
            */
            fPtr = fopen("user_auth.txt", "r");

            /* fopen() return NULL if last operation was unsuccessful */
            if(fPtr == NULL){
                /* File not created hence exit */
                printf("Unable to read user_auth.txt file.\n");
                exit(EXIT_FAILURE);
            }

            fgets(line, 1000, fPtr);

            if(strcmp(line, hashpass2) == 10)
            {
                strcpy(cli->username, username);
                    sprintf(buff_out, "%s:%d  \"%s\" has joined\n",
                    inet_ntoa(cli->address.sin_addr), 
                    cli->address.sin_port,
                    cli->username);
                update_log(buff_out, "login.log");
                update_log(buff_out, "chatting.log");
		        printf("%s", buff_out);
		        send_message(buff_out, cli->uid);
                fclose(fPtr);
            }
            else{
                printf("Incorrect Password.\n");
                sprintf(buff_out, "%s enter incorrect Password.\n", username);
                update_log(buff_out, "login.log");
		        leave_flag = 1;
            }
        }
    }

	bzero(buff_out, BUFFER_SZ);

	while(1){
		if (leave_flag) {
			break;
		}

		int receive = recv(cli->sockfd, buff_out, BUFFER_SZ, 0);
		if (receive > 0){
			if(strlen(buff_out) > 0){
                update_log(buff_out, "chatting.log");
                if (is_send_command(buff_out, &IP, &PORT, &filename)) {
                    send_message_to(buff_out, IP, PORT);
                    download_file(cli, IP, PORT);
                }
                else if (is_vote_command(buff_out, &question)){
                    printf("Entered elseif \n");
                    send_message(buff_out, cli->uid);
                    vote(cli->uid);
                    result_vote();
                }
                else {
                    printf("Else Condition entered. NOT VOTE/SEND\n");
                    send_message(buff_out, cli->uid);
                    str_trim_lf(buff_out, strlen(buff_out));
                    printf("%s -> %s\n", buff_out, cli->username);
                }
			}
		} 
        else if (receive == 0 || strcmp(buff_out, "exit") == 0){
			sprintf(buff_out, "%s has left\n", cli->username);
			printf("%s", buff_out);
            update_log(buff_out, "chatting.log");
            update_log(buff_out, "login.log");
			send_message(buff_out, cli->uid);
			leave_flag = 1;
		} 
        else {
			printf("ERROR: -1\n");
			leave_flag = 1;
		}

		bzero(buff_out, BUFFER_SZ);
	}

  /* Delete client from queue and yield thread */
	close(cli->sockfd);
    queue_remove(cli->uid);
    free(cli);
    clnt_count--;
    pthread_detach(pthread_self());

	return NULL;
}

int main(int argc, char **argv){
    char hashpass[100];
	if(argc != 3){
		printf("Usage: %s <port> <password>\n", argv[0]);
		exit(1);
	}

    //Creating a password file
    FILE * fPtr;
    fPtr = fopen("user_auth.txt", "w+");

    /* fopen() return NULL if last operation was unsuccessful */
    if(fPtr == NULL)
    {
        /* File not created hence exit */
        printf("Unable to create file.\n");
        exit(EXIT_FAILURE);
    }

    // converting the resultant hash(int) to hashpass(char)
    unsigned long hash1 = hash(argv[2]);
    snprintf( hashpass, DATA_SIZE, "%d\n", hash1 );

    /* Write data to file */
    fputs(hashpass, fPtr);

    /* Close file to save file data */
    fclose(fPtr);

	int option = 1;
	int serv_sock = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    pthread_t tid;

    /* Socket settings */
    serv_sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    /* Ignore pipe signals */
	signal(SIGPIPE, SIG_IGN);

	if(setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR,(char*)&option,sizeof(option)) < 0){
		perror("ERROR: setsockopt failed");
        exit(1);
	}

	/* Bind */
    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR: Socket binding failed");
        exit(1);
    }

    /* Listen */
    if (listen(serv_sock, 10) < 0) {
        perror("ERROR: Socket listening failed");
        exit(1);
	}

	printf("<>?<>?<>?<>? Capstone Design 2 Chatroom Server ?<>?<>?<>?<>\n");

	while(1){
		socklen_t clilen = sizeof(clnt_addr);
		connfd = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clilen);

		/* Check if max clients is reached */
		if((clnt_count + 1) == MAX_CLIENTS){
			printf("Max clients reached. Rejected: ");
			print_client_addr(clnt_addr);
			printf(":%d\n", clnt_addr.sin_port);
			close(connfd);
			continue;
		}

		/* Client settings */
		client_t *cli = (client_t *)malloc(sizeof(client_t));
		cli->address = clnt_addr;
		cli->sockfd = connfd;
		cli->uid = uid++;

		/* Add client to the queue and fork thread */
		queue_add(cli);
		pthread_create(&tid, NULL, &handle_client, (void*)cli);

		/* Reduce CPU usage */
		sleep(1);
	}

	return 0;
}
