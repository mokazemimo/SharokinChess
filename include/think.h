
#ifndef __think_h__
#define __think_h__

#include <windows.h>

#define MAX(a,b)    ((a) > (b) ? (a) : (b))
#define ABS(x)      ((x) >= 0 ? (x) : -(x))

#define WM_THINKFINISHED (WM_USER + 10)
#define FIFTY         50
#define MAX_DEPTH     32
#define __ALPHA      -4000000L
#define __BETA        4000000L
#define CAPTURED      64

// King, Queen, Knight, Rook, Bishop,
// White pawn, Black pawn, White capture pawn, Black capture pawn
#define SIDE_WHITE      0
#define SIDE_BLACK      1

#define PIECE_KING      0
#define PIECE_QUEEN     1
#define PIECE_KNIGHT    2
#define PIECE_ROOK      3
#define PIECE_BISHOP    4
#define PIECE_WPAWN     5
#define PIECE_BPAWN     6
#define PIECE_WCAPAWN   7
#define PIECE_BCAPAWN   8
#define PIECE_PAWN(x)          ((x) + 5)
#define PIECE_CAPTUREPAWN(x)   ((x) + 7)
#define NUM_PIECES      9
#define NUM_PIECETYPES  6

#define SQUAR(x,y)        ((x) | ((y) << 3))
#define LINE_OF_SQUAR(s)  ((s) & 7)
#define RANK_OF_SQUAR(s)  ((s) >> 3)
#define PIECETYPE(p)      ((p) & 0x3f)
#define PIECETYPE2(p)     (pt2[(p) & 0x3f])
#define IS_NOTEMPTY(s)    (s)
#define SIDE_OF(p)        ((long)(p) & 0xc0)
#define DIR(f, t)          __direction[((long)(f) << 6) | (t)]
#define RAYS2(p, s)       &__pieces[p].Rays2[__pieces[p].Rays2[s]]
#define IRAYS2(p, s, g)   ((__pieces[p].iRays[s] >> (g)) & 1)

typedef unsigned int square_t;

#include <pshpack4.h>
typedef union
{
    struct
    {
        square_t from, to;
    };
    __int64 move;
}
MOVE64;
#define MOVE64_SIZE     2   // sizeof(MOVE64) / sizeof(long)

#include <poppack.h>

typedef struct
{
    long FirstI, LastI, Dir, PawnMove, LastRank, OneBeforeLastRank, KingInitPos;
    long BitSide, BitOppSide, CanCastleBit[2], CastledBit[2];
}
SIDEINFO;
extern SIDEINFO __sides[2];

long __init();
long __generateMoves(long *pos, MOVE64 *moves_buf);
long __makemove(long *pos, MOVE64 move);
long __ischecked(long *pos);
void __prepare(long *pos, long depth, MOVE64 *m, long fifty, long timelimit, HWND hwnd);
DWORD WINAPI __think(LPVOID data);

long __compact(long *);

//long __getbestmove_from();
//long __getbestmove_to();
bool __bestmovefound();
MOVE64 __getbestmove();
long __checkstalemate(long *pos, long *stalemate);
void __zerohasharr();

extern long __lastmoveTime, __generateMovesCount, __makemoveCount, __nodescount;
extern long __depth, __fifty;

extern HANDLE hFinishEvent, hStopEvent, hThinkEvent;

#endif
