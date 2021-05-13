#include <stdio.h>
#include <stdlib.h>
#include "omok.h"

void printboard(char board[17][17]);

int main()
{
    int i, j;
    char board[17][17];    
    for (i = 0; i < 17; i++)
        for (j = 0; j < 17; j++)
            board[i][j] = EMPTY;
    
    char buf[1024];
    int x, y, t;
    char stone;

    printf("__________________________________\n");
    printf("HOW TO | input <x> <y> <b|w|e> (e is for emptifying grid)\n");
    printf(" PLAY  | no player turn control, its on your own.\n");
    while(1) {
        printboard(board);
        printf("move(<x> <y> <b|w|e>): ");
        fgets(buf, 1024, stdin);
        sscanf(buf, "%d %d %c \n", &x, &y, &stone);
        
        if (x < LBOUND || x > HBOUND || y < LBOUND || y > HBOUND) {
            printf("wrong coordinate\n");
            continue;
        }

        // BLACK
        if (stone == 'b' || stone == 'B') {
            printf("test..\n");
            t = testblack(board, x, y);
            printf("test done\n");
            switch(t) {
                case TEST_ILL:
                    printf("black illegal move\n");
                    continue;
                case TEST_WIN:
                    board[x][y] = BLACK;
                    printboard(board);
                    printf("black win\n");
                    return 0;                
            }
            board[x][y] = BLACK;
        }

        // WHITE
        else if (stone == 'w' || stone == 'W') {
            t = testwhite(board, x, y);
            switch(t) {
            case TEST_ILL:
                continue;
            case TEST_LEG:
                board[x][y] = WHITE;
                continue;
            case TEST_WIN:
                board[x][y] = WHITE;
                printboard(board);
                printf("white win\n");
                return 0;
            }
            
        }

        // EMPTY
        else if (stone == 'E' || stone == 'e') {
            board[x][y] = EMPTY;
        }

        // EXCEPTION
        else
            printf("wrong stone\n");
    }
}

void printboard(char board[17][17])
{
    int i, j;
    char look;
    printf("__________________________________\n");
    printf("   1 2 3 4 5 6 7 8 9 ~11 ~13 ~15 |\n");
    for (i = 1; i <= 15; i++) {
        printf("%2d ", i);

        for (j = 1; j <= 15; j++) {
            look = board[i][j];
            switch (look) {
            case BLACK:
                printf("● "); break;                
            case WHITE:
                printf("○ "); break;
            case EMPTY:
                printf("- "); break;
            }
            
        }
            
        printf("|\n");
    }
}