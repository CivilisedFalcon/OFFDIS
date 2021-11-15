#include <stdio.h>
#include <string.h>

#define MAX_LEN 256 //256Bytes

int main() {
    
    char message[MAX_LEN], dest[MAX_LEN], filedest[MAX_LEN];
    FILE *fp;
    
    printf("Please enter your message\n");
    
    scanf("%s", &message);
    
    printf("Please enter destination User or IP\n");    //but now only User
    
    scanf("%s", &dest);
    
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
