#include <stdio.h>
#include <stdlib.h>
// board status
#define BLACK 66
#define WHITE 87
#define EMPTY 43
// test way
#define W_VERT  0
#define W_HORZ  1
#define W_DGNE  2   // diagonal to NE
#define W_DGSE  3   // diagonal to SE
// stretch direction
//    7 0 1
//    6<O>2 
//    5 4 3
//    NW N NE
//     W C E
//    SW S SE
#define DI_N    0
#define DI_NE   1
#define DI_E    2
#define DI_SE   3
#define DI_S    4
#define DI_SW   5
#define DI_W    6
#define DI_NW   7
// BSP blocked_by
#define BY_W    0
#define BY_E    1
// test_return
#define TEST_VIC    0
#define TEST_LEG    1
#define TEST_ILL    2
#define TEST_O3     3
#define TEST_O4     4
#define TEST_O34    5
#define TEST_C      6

int test(char board[15][15], int x, int y);

struct BSP {    // Black Space Partial
    int xco;    // stretched, coordinate that encountered non-black 
    int yco;    // 
    int len;    // space size, 0~5
    int by;     // stretch blocked by { white=border, empty}
};
void findBSP(char board[15][15], int x, int y, int d, struct BSP* bsp)
{
    // find Black Space Partial toward d
    // stretch until first encountering non-black
    // bsp->by : white(==border) or empty
    bsp->len = 0;
    int n, ix = x, iy = y;
    char temp;
    // codes below are same for-format
    switch (d) {
        case DI_N:
            for (n = 0; n < 6; n++) {
                if (ix <= 0) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = WHITE;
                    break;
                }
                temp = board[--ix][iy];
                if (temp == EMPTY || temp == WHITE) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = temp;
                    break;
                }
                bsp->len++;
            }
            bsp->xco = ix;
            bsp->yco = iy;
            bsp->by = WHITE;
            break;
        case DI_NE:
            for (n = 0; n < 6; n++) {
                if (ix <= 0 || iy >= 14) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = WHITE;
                    break;
                }
                temp = board[--ix][++iy];
                if (temp == EMPTY || temp == WHITE) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = temp;
                    break;
                }
                bsp->len++;
            }
            bsp->xco = ix;
            bsp->yco = iy;
            bsp->by = WHITE;
            break;
        case DI_E:
            for (n = 0; n < 6; n++) {
                if (iy >= 14) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = WHITE;
                    break;
                }
                temp = board[ix][++iy];
                if (temp == EMPTY || temp == WHITE) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = temp;
                    break;
                }
                bsp->len++;
            }
            bsp->xco = ix;
            bsp->yco = iy;
            bsp->by = WHITE;
            break;
        case DI_SE:
            for (n = 0; n < 6; n++) {
                if (ix >= 14 || iy >= 14) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = WHITE;
                    break;
                }
                temp = board[++ix][++iy];
                if (temp == EMPTY || temp == WHITE) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = temp;
                    break;
                }
                bsp->len++;
            }
            bsp->xco = ix;
            bsp->yco = iy;
            bsp->by = WHITE;
            break;
        case DI_S:
            for (n = 0; n < 6; n++) {
                if (ix >= 14) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = WHITE;
                    break;
                }
                temp = board[++ix][iy];
                if (temp == EMPTY || temp == WHITE) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = temp;
                    break;
                }
                bsp->len++;
            }
            bsp->xco = ix;
            bsp->yco = iy;
            bsp->by = WHITE;
            break;
        case DI_SW:
            for (n = 0; n < 6; n++) {
                if (ix >= 14 || iy <= 0) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = WHITE;
                    break;
                }
                temp = board[++ix][--iy];
                if (temp == EMPTY || temp == WHITE) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = temp;
                    break;
                }
                bsp->len++;
            }
            bsp->xco = ix;
            bsp->yco = iy;
            bsp->by = WHITE;
            break;
        case DI_W:
            for (n = 0; n < 6; n++) {
                if (iy <= 0) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = WHITE;
                    break;
                }
                temp = board[ix][--iy];
                if (temp == EMPTY || temp == WHITE) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = temp;
                    break;
                }
                bsp->len++;
            }
            bsp->xco = ix;
            bsp->yco = iy;
            bsp->by = WHITE;
            break;
        case DI_NW:
            for (n = 0; n < 6; n++) {
                if (ix <= 0 || iy <= 0) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = WHITE;
                    break;
                }
                temp = board[--ix][--iy];
                if (temp == EMPTY || temp == WHITE) {
                    bsp->xco = ix;
                    bsp->yco = iy;
                    bsp->by = temp;
                    break;
                }
                bsp->len++;
            }
            bsp->xco = ix;
            bsp->yco = iy;
            bsp->by = WHITE;
            break;
    }
}
void copyBoard(char src[15][15], char dest[15][15])
{
    // Just copy
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            dest[i][j] = src[i][j];
}
// margin_return
#define M_BLACK     0
#define M_EMPTY     1
#define M_ILLEG     2
struct MS {
    int m[4];
    int len;
    int by;
    int ex; // x-coord if found empty
    int ey; //
};
void margin(char board[15][15], int x, int y, int d, struct MS* ms)
{
    // lookup 4 Marginal Space from (x, y) with direction d
    // the board is afterboard where caller's board[x][y] is BLACK
    // every EMPTY must checked legal or illegal
    // border, white is changed to M_ILLEG
    int n, ix = x, iy = y;
    char temp;
    int t;
    ms->len = 0;
    int lflag = 0, eflag = 0;
    // codes below are same for-format
    switch (d) {
        case DI_N:
            for (n = 0; n < 4; n++) {
                if (ix <= 0) {
                    ms->ex = ix;
                    ms->ey = iy;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                temp = board[--ix][iy];
                if (temp == EMPTY) {                    
                    if (eflag == 0) {
                        ms->ex = ix;
                        ms->ey = iy;
                        eflag = 1;
                    }
                    ms->m[n] = M_EMPTY;
                    lflag = 1;
                    continue;
                }
                if (temp == WHITE) {
                    lflag = 1;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                ms->m[n] = M_BLACK;
                if (lflag != 1)
                    ms->len++;
            }
            break;
        case DI_NE:
            for (n = 0; n < 4; n++) {
                if (ix <= 0 || iy >= 14) {
                    ms->ex = ix;
                    ms->ey = iy;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                temp = board[--ix][++iy];
                if (temp == EMPTY) {                    
                    if (eflag == 0) {
                        ms->ex = ix;
                        ms->ey = iy;
                        eflag = 1;
                    }
                    ms->m[n] = M_EMPTY;
                    lflag = 1;
                    continue;
                }
                if (temp == WHITE) {
                    lflag = 1;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                ms->m[n] = M_BLACK;
                if (lflag != 1)
                    ms->len++;
            }
            break;
        case DI_E:
            for (n = 0; n < 4; n++) {
                if (iy >= 14) {
                    ms->ex = ix;
                    ms->ey = iy;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                temp = board[ix][++iy];
                if (temp == EMPTY) {                    
                    if (eflag == 0) {
                        ms->ex = ix;
                        ms->ey = iy;
                        eflag = 1;
                    }
                    ms->m[n] = M_EMPTY;
                    lflag = 1;
                    continue;
                }
                if (temp == WHITE) {
                    lflag = 1;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                ms->m[n] = M_BLACK;
                if (lflag != 1)
                    ms->len++;
            }          
            break;
        case DI_SE:
            for (n = 0; n < 4; n++) {
                if (ix >= 14 || iy >= 14) {
                    ms->ex = ix;
                    ms->ey = iy;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                temp = board[++ix][++iy];
                if (temp == EMPTY) {
                    if (eflag == 0) {
                        ms->ex = ix;
                        ms->ey = iy;
                        eflag = 1;
                    }
                    ms->m[n] = M_EMPTY;
                    lflag = 1;
                    continue;
                }
                if (temp == WHITE) {
                    lflag = 1;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                ms->m[n] = M_BLACK;
                if (lflag != 1)
                    ms->len++;
            }
            break;
        case DI_S:
            for (n = 0; n < 4; n++) {
                if (ix >= 14) {
                    ms->ex = ix;
                    ms->ey = iy;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                temp = board[++ix][iy];
                if (temp == EMPTY) {
                    if (eflag == 0) {
                        ms->ex = ix;
                        ms->ey = iy;
                        eflag = 1;
                    }
                    ms->m[n] = M_EMPTY;
                    lflag = 1;
                    continue;
                }
                if (temp == WHITE) {
                    lflag = 1;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                ms->m[n] = M_BLACK;
                if (lflag != 1)
                    ms->len++;
            }
            break;
        case DI_SW:
            for (n = 0; n < 4; n++) {
                if (ix >= 14 || iy <= 0) {
                    ms->ex = ix;
                    ms->ey = iy;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                temp = board[++ix][--iy];
                if (temp == EMPTY) {                    
                    if (eflag == 0) {
                        ms->ex = ix;
                        ms->ey = iy;
                        eflag = 1;
                    }
                    ms->m[n] = M_EMPTY;
                    lflag = 1;
                    continue;
                }
                if (temp == WHITE) {
                    lflag = 1;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                ms->m[n] = M_BLACK;
                if (lflag != 1)
                    ms->len++;
            }
            break;
        case DI_W:
            for (n = 0; n < 4; n++) {
                if (iy <= 0) {
                    ms->ex = ix;
                    ms->ey = iy;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                temp = board[ix][--iy];
                if (temp == EMPTY) {
                    if (eflag == 0) {
                        ms->ex = ix;
                        ms->ey = iy;
                        eflag = 1;
                    }
                    ms->m[n] = M_EMPTY;
                    lflag = 1;
                    continue;
                }
                if (temp == WHITE) {
                    lflag = 1;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                ms->m[n] = M_BLACK;
                if (lflag != 1)
                    ms->len++;
            }
            break;
        case DI_NW:
            for (n = 0; n < 4; n++) {
                if (ix <= 0 || iy <= 0) {
                    ms->ex = ix;
                    ms->ey = iy;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                temp = board[--ix][--iy];
                if (temp == EMPTY) {
                    if (eflag == 0) {
                        ms->ex = ix;
                        ms->ey = iy;
                        eflag = 1;
                    }
                    ms->m[n] = M_EMPTY;
                    lflag = 1;
                    continue;
                }
                if (temp == WHITE) {
                    lflag = 1;
                    ms->m[n] = M_ILLEG;
                    continue;
                }
                ms->m[n] = M_BLACK;
                if (lflag != 1)
                    ms->len++;
            }
            break;
        default:
            fprintf(stderr, "margin wrong direction input\n");
            exit(1);
    }    
}
int testMargin(char board[15][15], int bslen, struct MS* ms)
{
    // directed, check margin validity
    // this board is afterboard
    int long6position = 4 - bslen;
    int blockedposition = ms->len;
    int potentialbs = bslen + ms->len;
    int t;
    if (bslen <= 2) {
        if (ms->m[0] != M_BLACK) // EEB_ or IEB_
            return TEST_C;
        if (ms->m[long6position] == M_BLACK)
            return TEST_C;        
        switch (potentialbs) {
            case 2:
                return TEST_C;
                break;
            case 3:
                if (ms->by == M_ILLEG)
                    return TEST_C;
                /*
                t = test(board, ms->ex, ms->ey);
                printf("testMargin: pbs=%d test=%d\n", potentialbs, t);
                if (t == TEST_LEG || t == TEST_VIC) 
                    return TEST_O3;
                return TEST_C;
                */
                return TEST_O3; //but should be checked again
                break;
            case 4:
                return TEST_O4;                
                break;
        }
    }
    else if (bslen == 3) {      
        if (ms->m[long6position] == M_BLACK)
            return TEST_C;        
        switch (potentialbs) {
            case 3:
                /*
                t = test(board, ms->ex, ms->ey);
                printf("testMargin: pbs=%d test=%d\n", potentialbs, t);
                if (t == TEST_LEG || t == TEST_VIC)
                    return TEST_O3;
                return TEST_C; */
                return TEST_O3;
                break;
            case 4:
                return TEST_O4; // but must checked again
                break;
        }
    }
    else {  //bslen == 4
        if (ms->m[long6position] == M_BLACK)
            return TEST_C;
        return TEST_O4; // but must checked again
    }    
    fprintf(stderr, "testM bslen %d\n", bslen);
    exit(1);
}
int testWay(char board[15][15], int x, int y, int way)
{
    printf("testWay x=%d y=%d way=%d\n", x, y, way);    
    struct BSP bsp1, bsp2;
    int d1, d2;
    switch (way) {  // decide way & direction
    case W_VERT:
        d1 = DI_N;  d2 = DI_S;  break;
    case W_HORZ:
        d1 = DI_W;  d2 = DI_E;  break;
    case W_DGNE:
        d1 = DI_NE; d2 = DI_SW; break;
    case W_DGSE:
        d1 = DI_NW; d2 = DI_SE; break;
    }

    findBSP(board, x, y, d1, &bsp1);
    printf("testWay: findBSP d1=%d bsp1.len=%d\n", d1, bsp1.len);
    findBSP(board, x, y, d2, &bsp2);
    printf("testWay: findBSP d2=%d bsp2.len=%d\n", d2, bsp2.len);
    int len = bsp1.len + bsp2.len + 1; // include the preferred position(x, y)
    char after[15][15];
    copyBoard(board, after);
    after[x][y] = BLACK;

    struct MS ms1, ms2;
    int m1, m2;
    int t1, t2, tmex1, tmex2;
    printf("testWay: len=%d\n", len);
    switch (len) {
    case 1:
    case 2:
    case 3:
            if (bsp1.by == BY_W || bsp2.by == BY_W) {
                return TEST_C;
            }
            margin(board, bsp1.xco, bsp1.yco, d1, &ms1);            
            margin(board, bsp2.xco, bsp2.yco, d2, &ms2);
            m1 = testMargin(after, len, &ms1);
            m2 = testMargin(after, len, &ms2);
            printf("testWay: m1=%d m2=%d\n", m1, m2);
            if (m1 == TEST_C && m2 == TEST_C)   // always C
                return TEST_C;            
            t1 = test(after, bsp1.xco, bsp1.yco);
            if (t1 == TEST_VIC) t1 = TEST_LEG;
            if (t1 == TEST_ILL && m2 == TEST_C)
                return TEST_C;
            t2 = test(after, bsp2.xco, bsp2.yco);
            if (t2 == TEST_VIC) t2 = TEST_LEG;
            if (m1 == TEST_C && t2 == TEST_ILL)
                return TEST_C;
            if (t1 == TEST_ILL) {
                if (t2 == TEST_ILL)
                    return TEST_C;
                tmex2 = test(after, ms2.ex, ms2.ey);
                if (tmex2 == TEST_ILL)
                    return TEST_C;
                return m2;
            }
            if (t2 == TEST_ILL) {
                tmex1 = test(after, ms1.ex, ms1.ey);
                if (tmex1 == TEST_ILL)
                    return TEST_C;
                return m1;
            }                
            // now E(BS)E
            if ((m1 == TEST_O3 && m2 == TEST_O3) ||
                (m1 == TEST_O4 && m2 == TEST_O4))
                return TEST_ILL;
            if ((m1 == TEST_O3 && m2 == TEST_O4) ||
                (m1 == TEST_O4 && m2 == TEST_O3))
                return TEST_O34;
            if (m1 != TEST_C)
                return m1;  // single open(3 or 4)
            return m2;      // single open(3 or 4)
            break;
    case 4:
            if (bsp1.by == BY_W && bsp2.by == BY_W) {
                return TEST_C;  // WBBBBW
            }            
            margin(board, bsp1.xco, bsp1.yco, d1, &ms1);
            m1 = testMargin(after, len, &ms1);
            margin(board, bsp2.xco, bsp2.yco, d2, &ms2);
            m2 = testMargin(after, len, &ms2);
            printf("testWay: m1=%d m2=%d\n", m1, m2);
            if (m1 == TEST_C && m2 == TEST_C)
                return TEST_C;  // always C
            t1 = test(after, bsp1.xco, bsp1.yco);
            t2 = test(after, bsp2.xco, bsp2.yco);
            if (t1 == TEST_ILL && t2 == TEST_ILL)
                return TEST_C;
            if (m1 == TEST_O4 && m2 == TEST_O4)
                return TEST_ILL;
            return TEST_O4;
            break;
    case 5:
            return TEST_VIC;
            break;
    case 6:
            return TEST_ILL;
            break;
    }
}
int test(char board[15][15], int x, int y)
{
    //TODO
    //  test vert, horz, diag+, diag-
    //  if 3-3, return illeg
    // return [victory, legal, illegal]
    int t[4], i;
    int vic = 0, o3 = 0, o4 = 0;
    t[0] = testWay(board, x, y, W_VERT);
    printf("test: t[0]=%d\n", t[0]);
    switch(t[0]) {
    case TEST_ILL:  return TEST_ILL;
    case TEST_VIC:  vic++;  break;     
    case TEST_O3:   o3++;   break;
    case TEST_O4:   o4++;   break;
    case TEST_O34:  o3++;   o4++;   break;
    case TEST_LEG:  break;
    }
    t[1] = testWay(board, x, y, W_HORZ);
    printf("test: t[1]=%d\n", t[1]);
    switch(t[1]) {
    case TEST_ILL:  return TEST_ILL;
    case TEST_VIC:  vic++;  break;     
    case TEST_O3:   o3++;   break;
    case TEST_O4:   o4++;   break;
    case TEST_O34:  o3++;   o4++;   break;
    case TEST_LEG:  break;
    }
    t[2] = testWay(board, x, y, W_DGNE);
    printf("test: t[2]=%d\n", t[2]);
    switch(t[2]) {
    case TEST_ILL:  return TEST_ILL;
    case TEST_VIC:  vic++;  break;     
    case TEST_O3:   o3++;   break;
    case TEST_O4:   o4++;   break;
    case TEST_O34:  o3++;   o4++;   break;
    case TEST_LEG:  break;
    }
    t[3] = testWay(board, x, y, W_DGSE);
    printf("test: t[3]=%d\n", t[3]);
    switch(t[3]) {
    case TEST_ILL:  return TEST_ILL;
    case TEST_VIC:  vic++;  break;     
    case TEST_O3:   o3++;   break;
    case TEST_O4:   o4++;   break;
    case TEST_O34:  o3++;   o4++;   break;
    case TEST_LEG:  break;
    }
    if (o3 >= 2)
        return TEST_ILL;
    if (o4 >= 2)
        return TEST_ILL;
    if (vic >= 1)
        return TEST_VIC;
    return TEST_LEG;    
}