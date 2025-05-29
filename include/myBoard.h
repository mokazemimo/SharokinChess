
#ifndef __MYBOARD_H__
#define __MYBOARD_H__

#include <windows.h>
#include <stdlib.h>
#include "think.h"

#define MAX_MOVES   500

#define SCORE_FOUNDINBOOK      0x7fffffffL
#define MAX_BOOKFORK           50
#define MAX_BOOKDEPTH          50

// Game States in Chess
#define GS_NORMAL                      0
#define GS_CHECKMATED                  1
#define GS_DRAWBYSTALEMATE             2
#define GS_DRAWBYTHREEFOLDREPETITION   3
#define GS_DRAWBYFIFTYRULE             4
#define GS_DRAWBYINSUFFICIENTMATERIAL  5
#define GS_CHECKED                     6
#define GS_TOOMANYMOVES                7

#include <pshpack1.h>

typedef struct
{
    MOVE64   Move;
    wchar_t* FarsiComment, * LatinComment;
    DWORD    BookOffs, BookState;
    short    Bonus;
    char     str[8]; // Na4xf3+
    long     Time;
}
COMMOVE;
#include <poppack.h>

class CBoard final
{
public:
    static CBoard &instance();
    ~CBoard();
    CBoard(CBoard&) = delete;
    CBoard(CBoard&&) = delete;
    CBoard& operator= (CBoard&) = delete;
    CBoard& operator= (CBoard&&) = delete;

    int  Analyse(int sx, int sy, int dx, int dy);
    int  MakeMove();
    int  stepBack(short* move = NULL);
    int  stepForward();
    bool canStepBack() { return Head > 0; }
    bool canStepForward() { return Head < NumMoves; }
    int  Open(const char* filename);
    int  Save(const char* filename);
    int  SetupBoard();
    void CountPieces(int side, int* count);
    int  AnalysePosition(__int8* pos, __int8 turn, char* enp, int castle);
    int  getPieceAt(int x, int y);
    int  getSideAt(int x, int y);
    int  getTurn(void)
    {
        return (int)CurPos[66];
    }
    int  getMoveCount()
    {
        return NumMoves;
    }
    int  getMoveHead()
    {
        return Head;
    }
    char* getMoveStr(int i = -1);
    int   isChecked();
    int   isCheckmated();
    int   isStalemated();
    void  setPromotion(__int8 p);
    int   LoadData(HWND h);
    wchar_t* getGameComment(int farsi);
    int      setGameComment(wchar_t* comment, int farsi);
    wchar_t* getMoveComment(int move_index, int farsi);
    int      setMoveComment(int move_index, wchar_t* comment, int farsi);
    int      SetLevel(long depth, long timelimit);
    long     Think();
    int      ThinkFinished(void);
    void     StopThinking(void);
    long getGameState();
    DWORD getLastMoveTime() { return __lastmoveTime; }
    //int  Test(DWORD*, long* gc, long* mc);
    int  ReadOpenBook(char* filename);
    long GetDepth();
    void getKingSquar(int side, int* x, int* y)
    {
        *x = LINE_OF_SQUAR(CurPos[__sides[side].FirstI]);
        *y = RANK_OF_SQUAR(CurPos[__sides[side].FirstI]);
    }
    long getTime(long side);
    long getScore() { return Score; }
    void setScore(long score) { Score = score; }
    int  IsThinking();
    void Continue()
    {
        long gs = getGameState();
        if (gs == GS_NORMAL || gs == GS_CHECKED)
            StartTime = GetTickCount();
    }
    void getLastMove(int* x, int* y)
    {
        *x = -1;
        *y = -1;
        if (Head > 0)
            if (MovesList[Head - 1].Move.to < 100)
            {
                *x = MovesList[Head - 1].Move.from;
                *y = MovesList[Head - 1].Move.to;
            }
            else
            {
                int side = 1 ^ getTurn();
                *x = SQUAR(LINE_OF_SQUAR(MovesList[Head - 1].Move.from), __sides[side].LastRank - __sides[side].Dir);
                *y = SQUAR(RANK_OF_SQUAR(MovesList[Head - 1].Move.from), __sides[side].LastRank);
            }
    }

private:
    CBoard();

    long    generateMoves();
    long    Score;
    long    Head;
    long    NumMoves;
    COMMOVE MovesList[MAX_MOVES], anaMove;
    __int8  StartPos[100], CurPos[100], anaPosition[100];
    long    longPos[100]; // [100];  only for CBoar::Test() is set to 200
    char    tempBuf[8000];
    bool    bAdvanceHead, bAnalysed, bPosAnalysed;
    int     MakeMove(COMMOVE m), MakeMove(MOVE64 m);
    char*   OpenBook;
    DWORD   OpenBookSize;
    int  getMoveStr(COMMOVE* move, MOVE64*, int);
    HWND hWnd;
    long pDepth;
    long TimeLimit, StartTime, TotalTimes[2];
    long Fifty;
    int      CommentsLen;
    wchar_t  CommentsBuf[50000];
    wchar_t* GameFarsiComment, * GameLatinComment;
    long     Depth, CapDepth;
    int      temocolor, xtemp;
    HWND   ThinkThread;
    int  LegalMove(COMMOVE* move);
    short Notation2Move(char* str);
    //int   canMovePieceAt(int x, int y);
    int  BookInitPos;
    int  SearchBook(COMMOVE* pt_move, short* nb);
    void closeOpenBook();
    int  getHint(int* sx, int* sy, int* dx, int* dy);
    long* getLongPos();
    long  getGenMovesCount() { return __generateMovesCount; }
    long  getMakeMoveCount() { return __makemoveCount; }
    long  getNodesCount() { return __nodescount; }
};

#endif
