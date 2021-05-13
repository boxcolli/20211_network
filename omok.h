#include <stdio.h>
#include <stdlib.h>

//
// First test -> Then modify
//
// Before making move, use testblack() & testwhite()
// to check it is <Illegal move | Legal move | Win>,
// then modify board data[17][17].
//
// Remember the board data given to test*() is not yet
// modified.
//
// 
// test return
#define TEST_ILL    0
#define TEST_LEG    1
#define TEST_WIN    2
// board data
#define BLACK 66 // changeable
#define WHITE 87
#define EMPTY 45
#define BSIZE 17
#define LBOUND 0
#define LBORDR 1
#define HBOUND 16
#define HBORDR 15
// abstract board data
#define A_BLACK 0
#define A_ILLEG 1
#define A_EMPTY 2
// way
#define W_VERT  0
#define W_HORZ  1
#define W_DGNE  2
#define W_DGSE  3
// direction
#define D_N     0
#define D_NE    1
#define D_E     2
#define D_SE    3
#define D_S     4
#define D_SW    5
#define D_W     6
#define D_NW    7
// testway return
#define T_ILL  0
#define T_CLO  1
#define T_WIN  2
#define T_OP3  3   // unsafe
#define T_OP4  4   // unsafe
#define T_UN1  5   // unsafe return found 1
#define T_UN2  6   // unsafe return found both d
#define T_BS3  7
#define T_BS4  8
#define T_UNK  9   // un1, must be examined both direction

struct BSP {
    int len;
    int by;
    int x;
    int y;
};
struct MSP {
    int len;
    int by;
    int abs[4];
    int x;
    int y;
};
struct WDATA {
    struct BSP bs[2];
    struct MSP ms[2];
    int tw[2];
};
int testblack(char board[17][17], int x, int y);
int testway(char board[17][17], int x, int y, int w, struct BSP bs[2], struct MSP ms[2], int tw[2]);
int testmargin(int bslen, struct MSP* msp);
void findBSP(char board[17][17], int x, int y, int d, struct BSP* bsp);
void findMSP(char board[17][17], int x, int y, int d, struct MSP* msp);

int testwhite(char board[17][17], int x, int y);
int findWSP(char board[17][17], int x, int y, int d);

void copyboard(char src[17][17], char dest[17][17]);

int testblack(char board[17][17], int x, int y)
{
    if (x < 1 || x > 15 || y < 1 || y > 15)
        return TEST_ILL;
    
    struct WDATA wd[4];
    int twr[4];
    int win = 0, un = 0;
    int i;

    for (i = 0; i < 4; i++) {
        twr[i] = testway(board, x, y, i, wd[i].bs, wd[i].ms, wd[i].tw);
        switch (twr[i]) {
        case T_ILL:
            return TEST_ILL;
        case T_CLO:
            break;
        case T_WIN:
            win++; break;
        case T_OP3:
        case T_OP4:
        case T_UN1:
        case T_BS3:
        case T_BS4:
        case T_UNK:
            un++; break;
        case T_UN2:
            un += 2; break;
        }
    }

    if (un <= 1) {
        if (win >= 1)
            return TEST_WIN;
        return TEST_LEG;
    }
    int o3 = 0, o4 = 0;
    int j, bslen, wresult, tresult;
    int rx, ry; // for recursive call
    char rb[17][17];
    copyboard(board, rb);
    rb[x][y] = BLACK;

    for (i = 0; i < 4; i++) {
        switch (twr[i]) {
        case T_CLO:
        case T_WIN:
            break;

        case T_BS3:
            if (testblack(rb, wd[i].bs[0].x, wd[i].bs[0].y) == TEST_ILL || // OR!
            testblack(rb, wd[i].bs[1].x, wd[i].bs[1].y) == TEST_ILL) {
                un--;
            }
            o3++;
            break;

        case T_BS4:
            if (testblack(rb, wd[i].bs[0].x, wd[i].bs[0].y) == TEST_ILL && // AND!
            testblack(rb, wd[i].bs[1].x, wd[i].bs[1].y) == TEST_ILL) {
                un--;
            }
            o4++;
            break;

        case T_UN1:
        case T_UN2:
        case T_UNK:
            bslen = wd[i].bs[0].len + wd[i].bs[1].len + 1;

            switch (bslen) {
            case 1:
            case 2:
                for (j = 0; j < 2; j++) {
                    tresult = wd[i].tw[j];
                    switch (tresult) {
                    case T_CLO: // pass
                        break;

                    case T_OP3: // bs || ms
                        if (testblack(rb, wd[i].bs[j].x, wd[i].bs[j].y) == TEST_ILL ||
                        testblack(rb, wd[i].ms[j].x, wd[i].ms[j].y) == TEST_ILL) {
                            un--;
                        }
                        else
                            o3++;
                        break;

                    case T_OP4: // bs
                        if (testblack(rb, wd[i].bs[j].x, wd[i].bs[j].y) == TEST_ILL) {
                            un--;
                        }
                        else
                            o4++;
                        break;

                    default:
                        fprintf(stderr, "test UN? bslen<1|2> err\n");
                        exit(1);
                    }
                }
                break;

            case 3:
                for (j = 0; j < 2; j++) {
                    tresult = wd[i].tw[j];
                    switch (tresult) {
                    case T_CLO: // pass
                    case T_BS3: // trivial, OP4 is on the other side 
                        break;
                    case T_OP4: // bs
                        if (testblack(rb, wd[i].bs[j].x, wd[i].bs[j].y) == TEST_ILL) {
                            un--;
                        }
                        else
                            o4++;
                        break;

                    default:
                        fprintf(stderr, "test UN? bslen3 err\n");
                        exit(1);
                    }
                }
                break;

            case 4:
                for (j = 0; j < 2; j++) {
                    tresult = wd[i].tw[j];
                    switch (tresult) {
                    case T_CLO: // pass
                    case T_OP4: // ??
                        break;
                    case T_BS4: // bs, another side is closed
                        if (testblack(rb, wd[i].bs[j].x, wd[i].bs[j].y) == TEST_ILL) {
                            un--;
                        }
                        else
                            o4++;
                        break;

                    default:
                        fprintf(stderr, "test UN? bslen3 err\n");
                        exit(1);
                    }
                }
                break;

            default:
                exit(1);
            }
            break; // case T_UN1 T_UN2 T_UNK end

        case T_OP3:
        case T_OP4:
            printf("wrong return\n");
            exit(1);
        }

        //printf("test: un=%d o3=%d o4=%d\n", un, o3, o4);
        if (un <= 1) {
            if (win >= 1)
                return TEST_WIN;
            return TEST_LEG;
        }
        if (o3 >= 2 || o4 >= 2)
            return TEST_ILL;        
        
    } // for(i) end

}
int testway(char board[17][17], int x, int y, int w, struct BSP bs[2], struct MSP ms[2], int tw[2])
{
    int d[2];
    switch (w) {
    case W_VERT:    d[0] = D_N;   d[1] = D_S;   break;
    case W_HORZ:    d[0] = D_W;   d[1] = D_E;   break;
    case W_DGNE:    d[0] = D_NE;  d[1] = D_SW;  break;
    case W_DGSE:    d[0] = D_NW;  d[1] = D_SE;  break;
    }

    findBSP(board, x, y, d[0], &(bs[0]));
    findBSP(board, x, y, d[1], &(bs[1]));
    int bslen = bs[0].len + bs[1].len + 1;
    int i;
    //printf("\ttestway: bs0len=%d bs1len=%d bs0by=%d bs1by=%d\n", bs[0].len, bs[1].len, bs[0].by, bs[1].by);


    switch (bslen) {
    case 5:
        return T_WIN;
    case 6:
    case 7:
    case 8:
    case 9:
        return T_ILL;
    default:
        break;
    }

    // stuck both side
    if (bs[0].by == A_ILLEG && bs[1].by == A_ILLEG)
        return T_CLO;
    
    /*
    testmargin returns:
        T_CLO
        T_OP3
        T_OP4
        T_BS3
        T_BS4
    testway should return:
        T_UN1
        T_UN2
    */

    for (i = 0; i < 2; i++) {
        if (bs[i].by == A_EMPTY) {
            findMSP(board, bs[i].x, bs[i].y, d[i], &(ms[i]));
            tw[i] = testmargin(bslen, &(ms[i]));
            //printf("\ttestway: tw[%d](testm)=%d\n", i, tw[i]);
        }
        else
            tw[i] = T_CLO;
    }
    if (tw[0] == T_CLO && tw[1] == T_CLO) // both stuck
        return T_CLO;
    if (tw[0] == T_BS3 && tw[1] == T_BS3) { // BS=3 with no additional MS
        if (ms[0].abs[0] == A_ILLEG && ms[1].abs[0] == A_ILLEG)
            return T_CLO;
        return T_BS3;
    }
    if (tw[0] == T_BS4 && tw[1] == T_BS4) // BS=4 with no additional MS
        return T_BS4;


    switch (bslen) {
    case 1:
    case 2:
        if (tw[0] == T_CLO || tw[1] == T_CLO) // CLO & OP
            return T_UN1;
        if (tw[0] == T_OP4 && tw[1] == T_OP4) // OP4 & OP4
            return T_UN2;
        return T_UNK; // OP3 & OP3, OP3 & OP4
    case 3:
        if (tw[0] == T_CLO || tw[1] == T_CLO) // CLO & OP4
            return T_UN1;  // one T_OP4
        if (tw[0] == T_BS3 || tw[1] == T_BS3) // BS3 & OP4
            return T_UN2;
        return T_UNK; // OP4 & OP4
    case 4:
        return T_UN1; // CLO & BS4
    }
}
int testmargin(int bslen, struct MSP* msp)
{
    int mslen = bslen + msp->len;
    int l6pos = 4 - bslen;
    
    // going L6
    if (msp->abs[l6pos] == A_BLACK)
            return T_CLO;

    // too less black
    switch (mslen) {
    case 1:
    case 2:
        return T_CLO;
    }

    // no black found in MS
    if (bslen == mslen)
        return (bslen == 3)? T_BS3 : T_BS4;
    
    // final touch
    switch (mslen) {
    case 3:
        if (msp->by == A_ILLEG)
            return T_CLO;
        return T_OP3;
    case 4:
        return T_OP4;
    }
    // err!
    exit(1);
}
void findBSP(char board[17][17], int x, int y, int d, struct BSP* bsp)
{
    // find Black Space Partial toward d
    // stretch until first encountering non-black
    // bsp->by : white(==border) or empty
    int n;
    int ix = x, ixincre, ixbound;
    int iy = y, iyincre, iybound;
    char look;
    
    switch (d) {
        case D_N:  ixincre = -1; iyincre = 0;  break;
        case D_NE: ixincre = -1; iyincre = +1; break;
        case D_E:  ixincre = 0;  iyincre = +1; break;
        case D_SE: ixincre = +1; iyincre = +1; break;
        case D_S:  ixincre = +1; iyincre = 0;  break;
        case D_SW: ixincre = +1; iyincre = -1; break;
        case D_W:  ixincre = 0;  iyincre = -1; break;
        case D_NW: ixincre = -1; iyincre = -1; break;
    }
    switch (ixincre) {
        case +1: ixbound = HBORDR; break;
        case 0:  ixbound = -1;     break;
        case -1: ixbound = LBORDR; break;
    }
    switch (iyincre) {
        case +1: iybound = HBORDR; break;
        case 0:  iybound = -1;     break;
        case -1: iybound = LBORDR; break;
    }



    bsp->len = 0;
    bsp->x = ix;
    bsp->y = iy;

    for (n = 0; n < 6; n++) {
        if (ix == ixbound || iy == iybound){
            bsp->by = A_ILLEG;
            break;
        }        
        
        ix += ixincre; iy += iyincre;
        bsp->x = ix;
        bsp->y = iy;
        look = board[ix][iy];
        
        if (look == EMPTY) {
            bsp->by = A_EMPTY;
                       
            break;
        }

        if (look == WHITE) {
            bsp->by = A_ILLEG;                
            break;
        }

        bsp->len++;
        bsp->by = A_BLACK;
    }
}
void findMSP(char board[17][17], int ex, int ey, int d, struct MSP* msp)
{
    // lookup 4 Margin Space from (x, y) with direction d
    // the board is afterboard where caller's board[x][y] is BLACK
    // every EMPTY must checked legal or illegal
    // border, white is changed to A_ILLEG
    int n;
    int ix = ex, ixincre, ixbound;
    int iy = ey, iyincre, iybound;
    char look;

    switch (d) {
        case D_N:  ixincre = -1; iyincre = 0;  break;
        case D_NE: ixincre = -1; iyincre = +1; break;
        case D_E:  ixincre = 0;  iyincre = +1; break;
        case D_SE: ixincre = +1; iyincre = +1; break;
        case D_S:  ixincre = +1; iyincre = 0;  break;
        case D_SW: ixincre = +1; iyincre = -1; break;
        case D_W:  ixincre = 0;  iyincre = -1; break;
        case D_NW: ixincre = -1; iyincre = -1; break;
    }
    switch (ixincre) {
        case +1: ixbound = HBORDR; break;
        case 0:  ixbound = -1;     break;
        case -1: ixbound = LBORDR; break;
    }
    switch (iyincre) {
        case +1: iybound = HBORDR; break;
        case 0:  iybound = -1;     break;
        case -1: iybound = LBORDR; break;
    }
    


    int lflag = 0;
    msp->x = ix;
    msp->y = iy;
    msp->len = 0;
    for (n = 0; n < 4; n++) {



        // check boundary
        if (ix == ixbound || iy == iybound) {
            msp->abs[n] = A_ILLEG;

            if (lflag == 1)
                continue;
            msp->x = ix;
            msp->y = iy;
            msp->by = A_ILLEG;
            continue;
        }



        // look
        ix += ixincre;  iy += iyincre;
        look = board[ix][iy];
        
        switch(look) {
            
            case EMPTY:
                msp->abs[n] = A_EMPTY;
                if (lflag == 1) break;
                //
                msp->by = A_EMPTY;
                msp->x = ix;
                msp->y = iy;
                lflag = 1;
                break;
                


            case WHITE:
                msp->abs[n] = A_ILLEG;
                if (lflag == 1) break;
                //
                msp->by = A_ILLEG;
                msp->x = ix;
                msp->y = iy;
                lflag = 1;
                break;
                


            case BLACK:
                msp->abs[n] = A_BLACK;
                if (lflag == 1) break;
                //
                msp->x = ix;
                msp->y = iy;
                msp->len++;
                break;
                


            default:
                exit(1);
        }
    }
}
int testwhite(char board[17][17], int x, int y)
{
    if (x < 1 || x > 15 || y < 1 || y > 15)
        return TEST_ILL; // for exception

    int d[2], wslen;
    int i;
    
    for (i = 0; i < 4; i++) {
        switch (i) {
        case W_VERT:    d[0] = D_N;   d[1] = D_S;   break;
        case W_HORZ:    d[0] = D_W;   d[1] = D_E;   break;
        case W_DGNE:    d[0] = D_NE;  d[1] = D_SW;  break;
        case W_DGSE:    d[0] = D_NW;  d[1] = D_SE;  break;
        }
        wslen = findWSP(board, x, y, d[0]) + findWSP(board, x, y, d[1]) + 1;
        if (wslen >= 5)
            return TEST_WIN;
    }
    
    return TEST_LEG;
}
int findWSP(char board[17][17], int x, int y, int d)
{
    int n;
    int ix = x, ixincre, ixbound;
    int iy = y, iyincre, iybound;

    switch (d) {
        case D_N:  ixincre = -1; iyincre = 0;  break;
        case D_NE: ixincre = -1; iyincre = +1; break;
        case D_E:  ixincre = 0;  iyincre = +1; break;
        case D_SE: ixincre = +1; iyincre = +1; break;
        case D_S:  ixincre = +1; iyincre = 0;  break;
        case D_SW: ixincre = +1; iyincre = -1; break;
        case D_W:  ixincre = 0;  iyincre = -1; break;
        case D_NW: ixincre = -1; iyincre = -1; break;
    }
    switch (ixincre) {
        case +1: ixbound = HBORDR; break;
        case 0:  ixbound = -1;     break;
        case -1: ixbound = LBORDR; break;
    }
    switch (iyincre) {
        case +1: iybound = HBORDR; break;
        case 0:  iybound = -1;     break;
        case -1: iybound = LBORDR; break;
    }

    int len = 0;
    char look;

    for (n = 0; n < 4; n++) {

        // check boundary
        if (ix == ixbound || iy == iybound) {
            return len;
        }

        // look
        ix += ixincre;  iy += iyincre;
        look = board[ix][iy];
        
        switch(look) {
        case BLACK:
        case EMPTY:
            return len;
        case WHITE:
            len++;
            continue;
        }
    }
}
void copyboard(char src[17][17], char dest[17][17])
{
    for (int i = 0; i < 17; i++)
        for (int j = 0; j < 17; j++)
            dest[i][j] = src[i][j];
}