#include <stdio.h>
#include <string.h>

#define MAX_LEN 256 //256Bytes

int main() {
    
    char message[MAX_LEN], dest[MAX_LEN], filedest[MAX_LEN];
    FILE *fp;
    
    printf("Please enter your message\n");
    
    fgets(message, MAX_LEN, stdin);
    message[strlen(message) - 1] = '\0';
    
    printf("Please enter destination User or IP\n");    //but now only User
    
    fgets(dest, MAX_LEN, stdin);
    dest[strlen(dest) - 1] = '\0';
    
    strcpy(filedest, dest);
    strcat(filedest, ".txt");
    
    //Check if the file(username) exists
    if((fp = fopen(filedest, "r")) == NULL) {
        printf("can't find the %s\n", dest);
        return -1;
    }
    fclose(fp);
    
    //Write message
    fp = fopen(filedest, "w");
    fprintf(fp, "%s\n", message);
    fclose(fp);

    return 0;
}
