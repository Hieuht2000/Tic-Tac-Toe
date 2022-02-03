

#include "includes.h"

/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/

#define  TASK_STK_SIZE                 512       /* Size of each task's stacks (# of WORDs)            */
#define  N_TASKS                        2       /* Number of identical tasks                          */
#define PLAYER_1 0x01
#define PLAYER_2 0x02
#define DRAW 0x04
//#define HORIZONTAL 3
//#define VERTICAL 3
//char          board[HORIZONTAL][VERTICAL];
/*
*********************************************************************************************************
*                                               VARIABLES
*********************************************************************************************************
*/

OS_STK        TaskStk[N_TASKS][TASK_STK_SIZE];        /* Tasks stacks                                  */
OS_STK        TaskStartStk[TASK_STK_SIZE];
OS_FLAG_GRP* GameLogic;

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

void TaskStart(void* data);        /* Function prototype of Startup task           */
void Drawboard(void* data);   /* Function prototypes of Random_fixedPos Task     */
void Player1(void* data);
void Player2(void* data);
int checkwin();
char board[9] = { '*','*','*' ,'*' ,'*' ,'*' ,'*' ,'*' ,'*' };

/*
*********************************************************************************************************
*                                                MAIN
*********************************************************************************************************
*/

int  main(void)
{
    PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);      /* Clear the screen                         */

    OSInit();                                              /* Initialize uC/OS-II                      */

    OSTaskCreate(TaskStart, (void*)0, &TaskStartStk[TASK_STK_SIZE - 1], 0);

    OSStart();                                             /* Start multitasking                       */

    return 0;
}


/*
*********************************************************************************************************
*                                              STARTUP TASK
*********************************************************************************************************
*/
void  TaskStart(void* pdata)
{
    INT16U key;
    INT8U symbol;
    INT8U err;

    pdata = pdata;                                         /* Prevent compiler warning                 */
 
    GameLogic = OSFlagCreate(0, &err);

    OSTaskCreate(Drawboard, (void*)&symbol, (void*)&TaskStk[0][TASK_STK_SIZE - 1], 1);
    OSTaskCreate(Player1, (void*)&symbol, (void*)&TaskStk[1][TASK_STK_SIZE - 1], 2);
    OSTaskCreate(Player2, (void*)&symbol, (void*)&TaskStk[2][TASK_STK_SIZE - 1], 3);

    while (1) {

        if (PC_GetKey(&key) == TRUE) {                     /* See if key has been pressed              */
            if (key == 0x1B) {                             /* Yes, see if it's the ESCAPE key          */
                exit(0);                               /* End program                              */
            }
        }
        OSTimeDly(50);                        /* Wait one second                          */
    }
}

/*
*********************************************************************************************************
*                                                  TASKS
*********************************************************************************************************
*/

void Player1(void* ptr) {
    INT8U err;
    int i = 0;
    srand(time(NULL));
    while (1) {
        OSFlagPend(GameLogic, PLAYER_1, OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME, 0, &err);
        while (board[i] != '*') {
            i = rand() % 9;
        }
        board[i] = 'X';
        OSFlagPost(GameLogic, PLAYER_2, OS_FLAG_SET, &err);
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}
void Player2(void* ptr) {
    INT8U err;
    int i = 0;
    srand(time(NULL));
    while (1) {
        OSFlagPend(GameLogic, PLAYER_2, OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME, 0, &err);
        while (board[i] != '*') {
            i = rand() % 9;
        }
        board[i] = 'O';
        OSFlagPost(GameLogic, DRAW, OS_FLAG_SET, &err);
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}
int checkwin(char square[9])
{
    if (square[0] == square[1] && square[1] == square[2] && square[0] != '*') {
        if (square[0] == 'X') return 1;
        else return 2;
    }
    else if (square[3] == square[4] && square[4] == square[5] && square[3] != '*')
    {
        if (square[3] == 'X') return 1;
        else return 2;
    }

    else if (square[6] == square[7] && square[7] == square[8] && square[6] != '*')
    {
        if (square[6] == 'X') return 1;
        else return 2;
    }

    else if (square[0] == square[3] && square[3] == square[6] && square[0] != '*')
    {
        if (square[0] == 'X') return 1;
        else return 2;
    }

    else if (square[1] == square[4] && square[4] == square[7] && square[1] != '*')
    {
        if (square[1] == 'X') return 1;
        else return 2;
    }

    else if (square[2] == square[5] && square[5] == square[8] && square[2] != '*')
    {
        if (square[2] == 'X') return 1;
        else return 2;
    }

    else if (square[0] == square[4] && square[4] == square[8] && square[0] != '*')
    {
        if (square[0] == 'X') return 1;
        else return 2;
    }

    else if (square[2] == square[4] && square[4] == square[6] && square[2] != '*')
    {
        if (square[2] == 'X') return 1;
        else return 2;
    }
    else if (square[1] != '*' && square[2] != '*' && square[3] != '*' &&  square[4] != '*' && square[5] != '*' 
        && square[6] != '*' && square[7]  != '*' && square[8] != '*' && square[0] != '*')
        return 0;
    else
        return  -1;
}



void Drawboard(void* data)
{
    char str1[20], str2[20], str3[20];
    char square[9];
    int check = 0;
    INT8U err;
    for (;;) {
        OSFlagPend(GameLogic, DRAW, OS_FLAG_WAIT_CLR_ANY, 0, &err);
        for (int i = 0; i < 9; i++) {
            square[i] = board[i];
        }
        PC_DispClrScr(DISP_FGND_WHITE + DISP_BGND_BLACK);
        sprintf(str1, " %c | %c | %c ", square[0], square[1], square[2]);
        PC_DispStr(1, 1, (INT8U*)str1, DISP_FGND_BLACK + DISP_BGND_GREEN);
        PC_DispStr(1, 2, (INT8U*)"___________", DISP_FGND_BLACK + DISP_BGND_GREEN);
        sprintf(str2, " %c | %c | %c ", square[3], square[4], square[5]);
        PC_DispStr(1, 3, (INT8U*)str2, DISP_FGND_BLACK + DISP_BGND_GREEN);
        PC_DispStr(1, 4, (INT8U*)"___________", DISP_FGND_BLACK + DISP_BGND_GREEN);
        sprintf(str3, " %c | %c | %c ", square[6], square[7], square[8]);
        PC_DispStr(1, 5, (INT8U*)str3, DISP_FGND_BLACK + DISP_BGND_GREEN);
        check = checkwin(square);

        if (check == -1) {
            OSFlagPost(GameLogic, PLAYER_1, OS_FLAG_SET, &err);
            OSTimeDlyHMSM(0, 0, 0, 200);
        }
        else {
            if (check == 1) PC_DispStr(1, 8, (INT8U*)"Player 1 win!", DISP_FGND_BLACK + DISP_BGND_GREEN);
            else if (check == 2) PC_DispStr(1, 8, (INT8U*)"Player 2 win!", DISP_FGND_BLACK + DISP_BGND_GREEN);
            else PC_DispStr(1, 8, (INT8U*)"Draw!", DISP_FGND_BLACK + DISP_BGND_GREEN);
            OSTaskDel(1);
            OSTaskDel(2);
            OSTaskDel(3);
        }
    }
}


