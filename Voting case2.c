#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM_NAME 10
#define MAX_LEN_NAME 100


//You can receive up to 10 voting names



void input_name(char** name_data); //Enter the voting type
void vote_name(char** named_data, int* vote_data); //Vote for the types.
void show_rank(char** name_data, int* vote_data); //Let's vote.

int main()
{
    int choice = 0; //Menu selection variable.
    char* name_data[MAX_NUM_NAME] = { 0 }; //Save type name.
    int vote_data[MAX_NUM_NAME]; //Save the number of votes.
    int i;
       
    for (i = 0; i < MAX_NUM_NAME; i++) //Initialize voting and pointer values.
    {
        vote_data[i] = 0;
    }

    while (choice != 4)
    {
        //Main
        printf("1, Input Type\n");
        printf("2, Vote\n");
        printf("3, Rankings\n");
        printf("4, Exit\n");
 
        scanf("%d", &choice); //Menu input.
        
        
        switch (choice)
        {
        case 1:
            printf("\n***Input the voting type***\n");
            input_name((char**)name_data);
            break;
        case 2:
            printf("\n***Vote choice***\n");
            vote_name(name_data, vote_data);
            break;
        case 3:
            printf("\n***Rankings***\n");
            show_rank(name_data, vote_data);
            break;
        }
    }
    return 0;
}

void input_name(char** name_data)
{
    char buffer[300]; //String buffer to receive a temporary name.
    int rtn;
    int i;

    for (i = 0; i < MAX_NUM_NAME; i++) //Finding empty space.
    {
        if (name_data[i] == NULL)
            break;
    }

    if (i == MAX_NUM_NAME)
    {
        printf("ERROR:저장공간이 충분하지 않음\n");
        return;
    }

    while (1) 
    {
        printf("Input text:");
        rtn = scanf("%s", buffer);

        if (rtn > MAX_LEN_NAME - 1)
            continue;
        else
            break;
    }
    name_data[i] = (char*)malloc(sizeof(char) * rtn + 1);//Allocate memory only as long as the name.
    strcpy(name_data[i], buffer);

}

void vote_name(char** name_data, int* vote_data)
{
    int vote_num = -1;
    int i;
    printf("\n***Name List***");
    for (i = 0; i < MAX_NUM_NAME; i++)
    {
        if (name_data[i] == NULL)
            break;
        printf("\n%d번:%d표 %s", i + 1, vote_data[i], name_data[i]);

    }

    while (1)
    {
        printf("\nInput the number to vote for: ");
        scanf("%d", &vote_num);
        if ((vote_num <= 0) || (vote_num > MAX_NUM_NAME))  //Invalid input value.
        {
            continue;
        }

        if (name_data[vote_num - 1] == NULL) //Vote for empty slots.
        {
            printf("\nERROR\n");
        }
        else {
            vote_data[vote_num - 1]++;
            break;
        }
    }


}

void show_rank(char** name_data, int* vote_data)
{
    int i, j;
    int tmp;
    char* p_tmp;

    for (i = 0; i < MAX_NUM_NAME - 1; i++) //According to the voting value.
    {
        if (name_data[i] == NULL) //If you reach the last space
            break;

        for (j = i + 1; j < MAX_NUM_NAME; j++)
        {
            if (name_data[j] == NULL) //If you reach the last space
                break;
            if (vote_data[i] < vote_data[j])//Exchange seats.
            {
                tmp = vote_data[i]; //Let's exchange seats.
                vote_data[i] = vote_data[j];
                vote_data[j] = tmp;

                p_tmp = name_data[i]; //Name exchange.
                name_data[i] = name_data[j];
                name_data[j] = p_tmp;
            }

        }
    }

    for (i = 0; i < MAX_NUM_NAME; i++) //The sorted ranking data is displayed on the screen.
    {
        if (name_data[i] == NULL)
            break;
        printf("\n%2d위:%3d표 %s\n", i + 1, vote_data[i], name_data[i]);

    }
}