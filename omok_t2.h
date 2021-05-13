#include <stdio.h>
#include <stdlib.h>
// board data
#define BLACK   66
#define WHITE   87
#define EMPTY   45
// test return
#define TEST_ILL    0
#define TEST_LEG    1
#define TEST_WIN    2
// test way
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
#define TW_WIN  0   // safe return
#define TW_CLO  1   // safe return
#define TW_ILL  2   // safe return
#define TW_UN1  3   // unsafe return found 1
#define TW_UN2  4   // unsafe return found both d
#define TW_OP3  5   // unsafe
#define TW_OP4  6   // unsafe
// abstract board data
#define A_BLACK 0
#define A_ILLEG 1
#define A_EMPTY 2

struct BSP {
    int by;     // BLACK finished by what, abstract board
    int len;    // BLACK length from start, try up to 5    
    int xco;    // xco, yco of finished point
    int yco;
};
struct MSP {
    int len;    // BLACK length found immediately
    int by;     // BLACK finished by what
    int abs[4]; // translate board into abstract board
    int xco;    // if by==A_EMPTY, that xco, yco
    int yco;
};
struct WDATA {
    struct BSP bs[2];
    struct MSP ms[2];
    int tw[2];  // testway result
};
int test(char board[15][15], int x, int y);
int testM(int bslen, struct MSP* ms);
int testWay(char board[15][15], int x, int y, int w, struct BSP bs[2], struct MSP ms[2], int tw[2]);
void findBS(char board[15][15], int x, int y, int d, struct BSP* bs);
void findMS(char board[15][15], int x, int y, int d, struct MSP* ms);
void copyBoard(char src[15][15], char dest[15][15]);
 
int test(char board[15][15], int x, int y)
{
    printf("test called x=%d y=%d\n", x, y);
    if (x < 0 || x > 14 || y < 0 || y > 14)
        return TEST_ILL;
   // findBS
    struct WDATA wd[4];
    int wr[4];
    int win = 0, un = 0;
    int i;
    //
    //
    // look every way
    for (i = 0; i < 4; i++) {
        //
        //
        // test one way
        wr[i] = testWay(board, x, y, i, wd[i].bs, wd[i].ms, wd[i].tw);
        printf("test: wr[%d]=%d\n", i, wr[i]);
        //
        //
        // record result
        switch (wr[i]) {
            case TW_WIN: win++;   break;
            case TW_CLO:          break;
            case TW_ILL: return TEST_ILL;
            case TW_UN1: un += 1; break;
            case TW_UN2: un += 2; break;
            case TW_OP3: un += 1; break;
            case TW_OP4: un += 1; break;
        }
    }
    //
    //
    //
    if (un <= 1) {
        //
        if (win >= 1)
            return TEST_WIN;
        //
        return TEST_LEG;
    }
    //
    //
    // now only unsafe
    int o3 = 0, o4 = 0;
    int j, wresult, tresult;
    char rb[15][15];
    rb[x][y] = BLACK;
    //
    //
    // review every way that returned UNsafe
    for (i = 0; i < 4; i++) {
        wresult = wr[i];
        //
        if (wresult == TW_UN1 || wresult == TW_UN2) {
            //
            for (j = 0; j < 2; j++) {
                tresult = wd[i].tw[i];
                //                
                switch (tresult) {
                    case TW_CLO:
                    case TW_WIN:
                        break;
                    //
                    //
                    //
                    case TW_OP3:
                        if ((test(rb, wd[i].bs[j].xco, wd[i].bs[j].yco) == TEST_ILL) ||
                        (test(rb, wd[i].ms[j].xco, wd[i].ms[j].yco) == TEST_ILL)) {
                            //
                            // unsafe to closed
                            un--;
                            //
                            // safe now?
                            if (un <= 1) {
                                if (win >= 1)
                                    return TEST_WIN;
                                return TEST_LEG;
                            }
                        }
                        else {
                            o3++;
                            if (o3 >= 2)
                                return TEST_ILL;
                        }
                        break;
                    //
                    //
                    //
                    case TW_OP4:
                        if (test(rb, wd[i].bs[j].xco, wd[i].bs[j].yco) == TEST_ILL) {
                            //
                            // changed to closed
                            un--;
                            //
                            // safe now?
                            if (un <= 1) {
                                if (win >= 1)
                                    return TEST_WIN;
                                return TEST_LEG;
                            }
                            break;
                        }
                        else {
                            o4++;
                            if (o4 >= 2)
                                return TEST_ILL;
                        }
                        break;
                        //
                        //
                        //
                }                
            }
        }
        else if (wresult == TW_OP3) {
            if ((test(rb, wd[i].bs[0].xco, wd[i].bs[0].yco) == TEST_ILL) ||
                (test(rb, wd[i].bs[1].xco, wd[i].bs[1].yco) == TEST_ILL)) {
                //
                //
                //
                un--;
                if (un <= 1) {
                    if (win >= 1)
                        return TEST_WIN;
                    return TEST_LEG;
                }
            }
            else {
                o3++;
                if (o3 >= 2)
                    return TEST_ILL;
                }
            }
        else if (wresult == TW_OP4) {
            if ((test(rb, wd[i].bs[0].xco, wd[i].bs[0].yco) == TEST_ILL) ||
                (test(rb, wd[i].bs[1].xco, wd[i].bs[1].yco) == TEST_ILL)) {
                //
                //
                //
                un--;
                if (un <= 1) {
                    if (win >= 1)
                        return TEST_WIN;
                    return TEST_LEG;
                }
            }
            else {
                o4++;
                if (o4 >= 2)
                    return TEST_ILL;
            }
        }
    }
    return TEST_LEG;
}
int testM(int bslen, struct MSP* ms)
{
    // decide testway return safely
    // decide only open|closed
    // 1 <= bslen <= 4
    int mslen = bslen + ms->len;
    int l6position = 4 - bslen;
    printf("\t\ttestM: mslen=%d by=%d\n", mslen, ms->by);
    switch(mslen) {
        //
        //
        // looking for O3 || O4
        case 1:
        case 2:
            return TW_CLO;
        //
        //
        // more check
        case 3:
            if (ms->abs[l6position] == A_BLACK)
                return TW_CLO;
            if (ms->by == A_ILLEG)
                return TW_CLO;
            //
            //
            //
            return TW_OP3;
        //
        //
        //
        case 4:
            if (ms->abs[l6position] == A_BLACK)
                return TW_CLO;
            //
            //
            //
            return TW_OP4;
        //
        //
        // ridiculuous input
        default:
            return TW_CLO;
    }
}
int testWay(char board[15][15], int x, int y, int w, struct BSP bs[2], struct MSP ms[2], int tw[2])
{
    // return 2 test_result of both direction
    //
    //
    // decide direction by way
    int d[2];
    switch (w) {
    case W_VERT:    d[0] = D_N;   d[1] = D_S;   break;
    case W_HORZ:    d[0] = D_W;   d[1] = D_E;   break;
    case W_DGNE:    d[0] = D_NE;  d[1] = D_SW;  break;
    case W_DGSE:    d[0] = D_NW;  d[1] = D_SE;  break;
    }
    //
    //
    // find BS both direction
    findBS(board, x, y, d[0], &(bs[0]));
    findBS(board, x, y, d[1], &(bs[1]));
    int bslen = bs[0].len + bs[1].len + 1;
    int i;
    


    printf("\ttway: %d = %d + %d (+1), by1=%d by2=%d\n", bslen, bs[0].len, bs[1].len, bs[0].by, bs[1].by);
    switch (bslen) {
        case 1:
        case 2:
            if (bs[0].by == A_ILLEG && bs[1].by == A_ILLEG)
                return TW_CLO;
            


            for (i = 0; i < 2; i++) {
                if (bs[i].by == A_EMPTY) {
                    //
                    findMS(board, bs[i].xco, bs[i].yco, d[i], &(ms[i]));
                    //
                    tw[i] = testM(bslen, &(ms[i]));
                }
                else
                    tw[i] = TW_CLO;
            }
            printf("\ttway: tw[0]=%d tw[1]=%d\n", tw[0], tw[1]);
            


            if (tw[0] == TW_CLO && tw[1] == TW_CLO)
                return TW_CLO;
            //
            if (tw[0] != TW_CLO && tw[1] != TW_CLO)
                return TW_UN2;
            //
            return TW_UN1;
        case 3:
            if (bs[0].by == A_ILLEG && bs[1].by == A_ILLEG)
                return TW_CLO;
            


            for (i = 0; i < 2; i++) {
                if (bs[i].by == A_EMPTY) {
                    //
                    findMS(board, bs[i].xco, bs[i].yco, d[i], &(ms[i]));
                    //
                    tw[i] = testM(bslen, &(ms[i]));
                }
                else
                    tw[i] = TW_CLO;
            }
            printf("\ttway: tw[0]=%d tw[1]=%d\n", tw[0], tw[1]);
            


            if (tw[0] == TW_CLO && tw[1] == TW_CLO)
                return TW_CLO;
            if (tw[0] == TW_OP3 && tw[1] == TW_OP3)
                return TW_OP3;
            if (tw[0] != TW_CLO && tw[1] != TW_CLO)
                return TW_UN2;
            //
            return TW_UN1;
            


        //
        //
        //
        case 4:
            if (bs[0].by == A_ILLEG && bs[1].by == A_ILLEG)
                return TW_CLO;
                


            for (i = 0; i < 2; i++) {
                if (bs[i].by == A_EMPTY) {
                    //
                    findMS(board, bs[i].xco, bs[i].yco, d[i], &(ms[i]));
                    //
                    tw[i] = testM(bslen, &(ms[i]));
                }
                else
                    tw[i] = TW_CLO;
            }
            printf("\ttway: tw[0]=%d tw[1]=%d\n", tw[0], tw[1]);
            


            if (tw[0] == TW_CLO && tw[1] == TW_CLO)
                return TW_CLO;
            //
            if (tw[0] == TW_OP4 && tw[1] == TW_OP4)
                return TW_OP4;
            //
            return TW_UN1;
            


        //
        //
        //
        case 5:
            return TW_WIN;
            


        //
        //
        //
        case 6:
            return TW_ILL;
    }
}
void findBS(char board[15][15], int x, int y, int d, struct BSP* bsp)
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
        case +1: ixbound = 14; break;
        case 0:  ixbound = -1; break;
        case -1: ixbound = 0;  break;
    }
    switch (iyincre) {
        case +1: iybound = 14; break;
        case 0:  iybound = -1; break;
        case -1: iybound = 0;  break;
    }



    bsp->len = 0;
    bsp->xco = ix;
    bsp->yco = iy;

    for (n = 0; n < 6; n++) {
        if (ix == ixbound || iy == iybound){
            bsp->by = A_ILLEG;
            break;
        }        
        
        ix += ixincre; iy += iyincre;
        bsp->xco = ix;
        bsp->yco = iy;
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
void findMS(char board[15][15], int ex, int ey, int d, struct MSP* msp)
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
        case +1: ixbound = 14; break;
        case 0:  ixbound = -1; break;
        case -1: ixbound = 0;  break;
    }
    switch (iyincre) {
        case +1: iybound = 14; break;
        case 0:  iybound = -1; break;
        case -1: iybound = 0;  break;
    }
    


    int lflag = 0;
    msp->xco = ix;
    msp->yco = iy;
    msp->len = 0;
    for (n = 0; n < 4; n++) {



        // check boundary
        if (ix == ixbound || iy == iybound) {
            msp->abs[n] = A_ILLEG;

            if (lflag == 1)
                continue;
            msp->xco = ix;
            msp->yco = iy;
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
                msp->xco = ix;
                msp->yco = iy;
                lflag = 1;
                break;
                


            case WHITE:
                msp->abs[n] = A_ILLEG;
                if (lflag == 1) break;
                //
                msp->by = A_ILLEG;
                msp->xco = ix;
                msp->yco = iy;
                lflag = 1;
                break;
                


            case BLACK:
                msp->abs[n] = A_BLACK;
                if (lflag == 1) break;
                //
                msp->xco = ix;
                msp->yco = iy;
                msp->len++;
                break;
                


            default:
                exit(1);
        }
    }
}
void copyBoard(char src[15][15], char dest[15][15])
{
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            dest[i][j] = src[i][j];
}