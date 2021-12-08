#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>

#define LENGTH 2082

// Global variables
volatile sig_atomic_t flag = 0;
int sock = 0;
char username[32];

void str_overwrite_stdout() {
    printf("%s", "> ");
    fflush(stdout);
}

void str_trim_lf (char* arr, int length){
    int i;
    for (i = 0; i < length; i++) { // trim \n
        if (arr[i] == '\n') {
            arr[i] = '\0';
            break;
        }
    }
}

void catch_ctrl_c_and_exit(int sig) {
    flag = 1;
}


bool is_send_command(char* msg, char** IP, char** PORT, char** filename) {
    char* option;
    char message[LENGTH + 1] = {};
    char tmp;
    strcpy(message, msg);
    
    tmp = message[5];
    message[5] = '\0';
    if (strcmp(message, "SEND ") == 0) {
        message[5] = tmp;
        option = message + 5;
        
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
    char message[LENGTH + 1] = {};

    strcpy(message, msg);
    message[5] = '\0';
    if (strcmp(message, "VOTE#") == 0) {
        option = msg + 5;
        option = strtok(option, "#");
        *question = option;
        return true;
    }

    return false;
}

void send_file(char* filename) {
    FILE* fp = fopen(filename, "r");
    sleep(1);
    char buffer[LENGTH + 1] = {};
    int l;
    do {
        fread(buffer, sizeof(char), LENGTH, fp);
        l = send(sock, buffer, strlen(buffer), 0);
        memset(buffer, 0, LENGTH + 1);
    } while (feof(fp) == 0);
    send(sock, "*", sizeof("*"), 0);
    fclose(fp);
}

void download_file(char* filename) {
    char buffer[LENGTH + 1] = {};
    char* tmp;
    int l = 0;
  
    FILE* fp = fopen(filename, "wb+");
    int i = 0;
    while (l = recv(sock, buffer, LENGTH, 0)) {
        if ((tmp = strstr(buffer, "*")) != NULL) {
            *tmp = '\0';
            fwrite(buffer, sizeof(char), strlen(buffer), fp);
            break;
        }
        fwrite(buffer, sizeof(char), l, fp);
    }
    printf("Download complete.\n");
    fclose(fp);
}

void send_msg_handler() {
    char message[LENGTH] = {};
    char buffer[LENGTH + 32] = {};
    char *IP, *PORT, *filename, *question;

    while(1) {
  	    str_overwrite_stdout();
        fgets(message, LENGTH, stdin);
        str_trim_lf(message, LENGTH);

        if (strcmp(message, "exit") == 0) {
			break;
        } 
        else if (is_send_command(message, &IP, &PORT, &filename)) {
            send(sock, message, strlen(message), 0);
            send_file(filename);
        }
        else if (is_vote_command(message, &question)) {
            printf("entered else if\n");
            send(sock, message, strlen(message), 0);
        }
        else {
            sprintf(buffer, "%s: %s \n", username, message);
            send(sock, buffer, strlen(buffer), 0);
        }

		bzero(message, LENGTH);
        bzero(buffer, LENGTH + 32);
    }
  catch_ctrl_c_and_exit(2);
}

void recv_msg_handler() {
	char message[LENGTH] = {};
	char* tmp, *filename;
    while (1) {
		int receive = recv(sock, message, LENGTH, 0);
    if (receive > 0) {
        if (is_send_command(message, &tmp, &tmp, &filename)) {
            printf("Download..\n");
            download_file("download.txt");
        }
        else
            printf("%s", message);
        str_overwrite_stdout();
    } 
    else if (receive == 0) {
			break;
    } 
    else {
			// -1
	}
	memset(message, 0, sizeof(message));
  }
}

int main(int argc, char **argv){
    char passwd[32];
	if(argc != 3){
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	signal(SIGINT, catch_ctrl_c_and_exit);

	printf("Please enter your username: ");
    fgets(username, 32, stdin);
    str_trim_lf(username, strlen(username));


	if (strlen(username) > 32 || strlen(username) < 2){
		printf("Username must be less than 30 and more than 2 characters.\n");
		exit(1);
	}

    printf("Please enter the Chatroom Password: ");
    fgets(passwd, 32, stdin);
    str_trim_lf(passwd, strlen(passwd));


	if (strlen(passwd) > 32 || strlen(passwd) < 2){
		printf("Password must be less than 30 and more than 2 characters.\n");
		exit(1);
	}

	struct sockaddr_in serv_addr;

	/* Socket settings */
	sock = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));


    // Connect to Server
    int err = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (err == -1) {
	    printf("ERROR: connect\n");
		exit(1);
	}

	// Send username
	send(sock, username, 32, 0);

    // Send password
	send(sock, passwd, 32, 0);

	printf(":::::::::: Capstone Design 2 Chatroom ::::::::::\n");

	pthread_t send_msg_thread;
    if(pthread_create(&send_msg_thread, NULL, (void *) send_msg_handler, NULL) != 0){
		printf("ERROR: pthread\n");
        exit(1);
	}

	pthread_t recv_msg_thread;
    if(pthread_create(&recv_msg_thread, NULL, (void *) recv_msg_handler, NULL) != 0){
		printf("ERROR: pthread\n");
		exit(1);
	}

	while (1){
		if(flag){
			printf("\nBye\n");
			break;
        }
	}

	close(sock);

	return 0;
}
