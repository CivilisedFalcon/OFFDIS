#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM_NAME 10
#define MAX_LEN_NAME 100


//최대 10개의 투표이름을 입력 받울 수 있다



void input_name(char** name_data); //투표 종류 입력받기
void vote_name(char** named_data, int* vote_data); //종류에 투표
void show_rank(char** name_data, int* vote_data); //투표 순위 출력

int main()
{
    int choice = 0; //메뉴선택 변수
    char* name_data[MAX_NUM_NAME] = { 0 }; //종류이름을 저장
    int vote_data[MAX_NUM_NAME]; //투표수를 저장
    int i;
       
    for (i = 0; i < MAX_NUM_NAME; i++) //투표값&포인터값 초기화
    {
        vote_data[i] = 0;
    }

    while (choice != 4)
    {
        //메인
        printf("1, 종류 입력\n");
        printf("2, 투표하기\n");
        printf("3, 순위보기\n");
        printf("4, 종료\n");
 
        scanf("%d", &choice); //메뉴 입력받기
        
        
        switch (choice)
        {
        case 1:
            printf("\n***투표할 종류 입력***\n");
            input_name((char**)name_data);
            break;
        case 2:
            printf("\n***투표 선택***\n");
            vote_name(name_data, vote_data);
            break;
        case 3:
            printf("\n***투표 순위***\n");
            show_rank(name_data, vote_data);
            break;
        }
    }
    return 0;
}

void input_name(char** name_data)
{
    char buffer[300]; //이름을 임시적으로 입력받을 문자열 버퍼
    int rtn;
    int i;

    for (i = 0; i < MAX_NUM_NAME; i++) //빈공간 찾기
    {
        if (name_data[i] == NULL)
            break;
    }

    if (i == MAX_NUM_NAME)
    {
        printf("ERROR:저장공간이 충분하지 않음\n");
        return;
    }

    while (1) //이름의 길이가 일정길이 이하일때까지 계속 입력받음
    {
        printf("텍스트를 입력하시오:");
        rtn = scanf("%s", buffer);

        if (rtn > MAX_LEN_NAME - 1)
            continue;
        else
            break;
    }
    name_data[i] = (char*)malloc(sizeof(char) * rtn + 1);//이름 길이만큼만 메모리 할당
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
        printf("\n\n투표할 번호를 입력: ");
        scanf("%d", &vote_num);
        if ((vote_num <= 0) || (vote_num > MAX_NUM_NAME))  //잘못된입력값
        {
            continue;
        }

        if (name_data[vote_num - 1] == NULL) //빈슬롯에 투표할때
        {
            printf("\nERROR:잘못된 번호입니다.\n");
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

    for (i = 0; i < MAX_NUM_NAME - 1; i++) //투표값에 따라 정렬
    {
        if (name_data[i] == NULL) //마지막 칸까지 도달했을경우
            break;

        for (j = i + 1; j < MAX_NUM_NAME; j++)
        {
            if (name_data[j] == NULL) //마지막 칸까지 도달했을경우
                break;
            if (vote_data[i] < vote_data[j])//자리교환
            {
                tmp = vote_data[i]; //투표값 자리교환
                vote_data[i] = vote_data[j];
                vote_data[j] = tmp;

                p_tmp = name_data[i]; //이름 자리교환
                name_data[i] = name_data[j];
                name_data[j] = p_tmp;
            }

        }
    }

    for (i = 0; i < MAX_NUM_NAME; i++) //정렬된 순위 데이터를 화면에 출력
    {
        if (name_data[i] == NULL)
            break;
        printf("\n%2d위:%3d표 %s\n", i + 1, vote_data[i], name_data[i]);

    }
}