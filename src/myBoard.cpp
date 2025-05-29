
#include <string.h>
#include <stdio.h>
#include <fstream>

#include "think.h"
#include "myBoard.h"

char Char2Piece(char ch);

CBoard& CBoard::instance()
{
    static CBoard board;
    return board;
}

CBoard::CBoard()
{
    memset(CommentsBuf, 0, sizeof(CommentsBuf));
    CommentsLen = 0;
    wchar_t *temp =
        &CommentsBuf[sizeof(CommentsBuf) / sizeof(wchar_t) - 1];
    GameFarsiComment = temp;
    GameLatinComment = temp;
    for(int i = 0; i < MAX_MOVES; i++)
    {
        MovesList[i].FarsiComment = temp;
        MovesList[i].LatinComment = temp;
    }
    OpenBook = NULL;
    OpenBookSize = 0;

    ThinkThread = NULL;
    hFinishEvent = CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)"FinishEvent");
    if(!hFinishEvent)
        return;
    hStopEvent = CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)"StopEvent");
    if(!hStopEvent)
        return;
    hThinkEvent = CreateEvent(NULL, TRUE, FALSE, (LPCWSTR)"ThinkEvent");
    if(!hThinkEvent)
        return;
    pDepth = 0;

    srand(time(nullptr));
}

CBoard::~CBoard()
{
    if(hFinishEvent)
        CloseHandle(hFinishEvent);
    if(hThinkEvent)
        CloseHandle(hThinkEvent);
    if(hStopEvent)
        CloseHandle(hStopEvent);
    if(ThinkThread)
    {
        TerminateThread(ThinkThread, 0);
        CloseHandle(ThinkThread);
    }
    closeOpenBook();
}

int CBoard::getPieceAt(int x, int y)
{
    if(!(x >= 0 && x < 8) || !(y >= 0 && y < 8))
        return -1;
    __int8 p = CurPos[SQUAR(x, y)];
    return (int)(p & 0xc0 ? PIECETYPE(p) : -1);
}

int CBoard::getSideAt(int x, int y)
{
    int ret =
        (CurPos[SQUAR(x, y)] & 0x40) ? SIDE_WHITE :
            (CurPos[SQUAR(x, y)] & 0x80) ? SIDE_BLACK : -1;
    return ret;
}

long CBoard::generateMoves()
{
    long i, n;
    MOVE64 *mb64 = (MOVE64 *)tempBuf;
    ///MOVE64 *mb16 = (MOVE64 *)tempBuf;
    n = __generateMoves(getLongPos(), mb64);
    /*
    for(i = 0; i < n; i++, mb64++, mb16++)
    {
        mb16->from = mb64->from;
        mb16->to   = mb64->to;
    }
    */
    return n;
}

int CBoard::Analyse(int sx, int sy, int dx, int dy)
{
    bAnalysed = 0;
    char side = getSideAt(sx, sy);
    if(getPieceAt(sx, sy) == PIECE_PAWN(side) &&
            sy + __sides[side].Dir == dy &&
                    dy == __sides[side].LastRank)
    {
        anaMove.Move.from = (char)SQUAR(sx, dx);
        anaMove.Move.to   = PIECE_QUEEN + 100;
        bAnalysed = 2;
    }
    else
    {
        anaMove.Move.from = (char)SQUAR(sx, sy);
        anaMove.Move.to   = (char)SQUAR(dx, dy);
        bAnalysed = 1;
    }
    if(!LegalMove(&anaMove))
        bAnalysed = 0;
    if(bAnalysed)
    {
        anaMove.BookOffs = 0;
        short i, n, nb;
        COMMOVE *cmv = (COMMOVE *)tempBuf;
        if((n = SearchBook(cmv, &nb)) >= 0)
            for(i = 0; i < n; i++)
            if(cmv[i].Move.move == anaMove.Move.move)
            {
                anaMove.BookOffs = cmv[i].BookOffs;
                bAnalysed |= 4;
                break;
            }
    }

    return bAnalysed;
}

int CBoard::MakeMove()
{
    if(!bAnalysed)
        return 1;
    long gs = getGameState();
    if(gs != GS_NORMAL && gs != GS_CHECKED)
        return gs;

    long time = GetTickCount();
    if(Head < NumMoves)
        StartTime = time;
    anaMove.Time = (Head > 0 ? time - StartTime : 0);
    TotalTimes[getTurn()] += anaMove.Time;
    StartTime = time;

    return MakeMove(anaMove);
}

int CBoard::MakeMove(MOVE64 move)
{
    COMMOVE cm;
    cm.Move = move;
    return MakeMove(cm);
}

int CBoard::MakeMove(COMMOVE commove)
{
    __fifty = Fifty;
    Fifty = __makemove(getLongPos(), commove.Move);
    for(long i = 0; i < 100; i++)
        CurPos[i] = longPos[i];
    if(bAdvanceHead)
    {
        long gs;
        if((gs = getGameState()) == GS_CHECKED)
            strcat(commove.str, "+");
        else if(gs == GS_CHECKMATED)
                strcat(commove.str, "#");
        MovesList[Head] = commove;
        MovesList[Head].FarsiComment =
            &CommentsBuf[sizeof(CommentsBuf) / sizeof(wchar_t) - 1];
        MovesList[Head].LatinComment =
            &CommentsBuf[sizeof(CommentsBuf) / sizeof(wchar_t) - 1];

        NumMoves = ++Head;
    }
    bAnalysed = 0;
    return 0;
}

int CBoard::SetupBoard()
{
    const char __stdpos[100] = {
    //   a     b     c     d       e     f     g     h
        0x43, 0x42, 0x44, 0x41,   0x40, 0x44, 0x42, 0x43, // 1
        0x45, 0x45, 0x45, 0x45,   0x45, 0x45, 0x45, 0x45, // 2
        0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 3
        0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 4

        0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 5
        0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 6
        0x86, 0x86, 0x86, 0x86,   0x86, 0x86, 0x86, 0x86, // 7
        0x83, 0x82, 0x84, 0x81,   0x80, 0x84, 0x82, 0x83, // 8
    //   a     b     c     d       e     f     g     h

        // 64
        0xc0, 0xf0, 0, 100,
        // 68
         4,  0,  1,  2,  3,  5,  6,  7,
         8,  9, 10, 11, 12, 13, 14, 15,
         // 84
         60, 56, 57, 58, 59, 61, 62, 63,
         48, 49, 50, 51, 52, 53, 54, 55
    };

    memcpy(StartPos, bPosAnalysed ? anaPosition : __stdpos, 100);
    memcpy(CurPos, StartPos, 100);
    BookInitPos = 12;
    if(memcmp(CurPos, __stdpos, 100))
        BookInitPos = 0;

    NumMoves = 0;
    Head = 0;
    bPosAnalysed = 0;
    Fifty = 0;
    StartTime = 0;
    TotalTimes[SIDE_WHITE] = 0;
    TotalTimes[SIDE_BLACK] = 0;
    __zerohasharr();

    return 0;
}

void CBoard::CountPieces(int side, int *p_count)
{
    int i, j;
    for(i = 0; i < 6; i++)
        p_count[i] = 0;

	int cv[] = {0, 1, 2, 3, 4, 5, 5};

	for(i = 0; i < 8; i++)
		for(j = 0; j < 8; j++)
			if(side == getSideAt(i, j))
			{
				p_count[cv[getPieceAt(i, j)]]++;
			}
}

int CBoard::LegalMove(COMMOVE *move)
{
    int i = 0, num = generateMoves();
    for(MOVE64 *mb = (MOVE64 *)tempBuf; i < num; i++, mb++)
        if(move->Move.from == mb->from && move->Move.to == mb->to)
        {
            getMoveStr(move, (MOVE64 *)tempBuf, num);
            return 1;
        }
    return 0;
}

int CBoard::stepBack(short *move)
{
    int res = 1;
    if(Head > 0)
    {
        if(move)
            *move = *(short *)&MovesList[Head - 1].Move;
        Head--;
        Fifty = 0;
        bAdvanceHead = false;
        memcpy(CurPos, StartPos, 100);
        TotalTimes[SIDE_WHITE] = TotalTimes[SIDE_BLACK] = 0;
        for(int i = 0; i < Head; i++)
        {
            TotalTimes[i % 2] += MovesList[i].Time;
            MakeMove(MovesList[i]);
        }
        StartTime = 0;
        bAdvanceHead = true;
        res = 0;
    }
    return res;
}

int CBoard::stepForward()
{
    int res = 1;
    if(Head < NumMoves)
    {
        bAdvanceHead = false;
        TotalTimes[getTurn()] += MovesList[Head].Time;
        MakeMove(MovesList[Head++]);
        bAdvanceHead = true;
        res = 0;
        StartTime = 0;
    }
    return res;
}

int CBoard::Open(const char *filename)
{
    std::fstream fp(filename, std::ios::binary | std::ios::in);
    if (!fp.is_open()) {
        return -1;
    }

    fp.seekg(0, std::ios::end);
    int size = fp.tellg();
    fp.seekg(0, std::ios::beg);

    char* buf = (size > 0 ? new char[size] : NULL);
    if (!buf)
        return -2;
    fp.read(buf, size);

    int i, j, k;
    for (i = size - 1; i > 0; --i) {
        if (buf[i] == ']')
            break;
    }

    SetupBoard();

    MOVE64 mv;
    for (; i < size; i++) {
        if (buf[i] == '.')
        {
            for (k = 0; k < 2; k++)
            {
                for (i++; i < size; i++)
                    if (buf[i] != ' ' && buf[i] != '\n')
                        break;
                j = i;
                mv.move = -1;
                for (; j < size; j++)
                    if (buf[j] == '+' || buf[j] == '#')
                        buf[j] = 0;
                    else if (buf[j] == ' ' || buf[j] == '\n')
                    {
                        buf[j] = 0;
                        break;
                    }
                if (!strcmp(buf + i, "1-0")) {
                    break;
                }
                if (!strcmp(buf + i, "0-1")) {
                    break;
                }
                if (!strcmp(buf + i, "1/2-1/2")) {
                    break;
                }
                else {
                    mv.move = Notation2Move(buf + i);
                    Analyse(LINE_OF_SQUAR(mv.from), RANK_OF_SQUAR(mv.from), LINE_OF_SQUAR(mv.to), RANK_OF_SQUAR(mv.to));
                    if (mv.move == -1)
                        return -3;  // Invalid move notation
                    else if (MakeMove())
                        return -4;
                }
                i = j;
            }
        }
    }
    delete[] buf;

    //while (!Undo());

    fp.close();

    return 0;
}

int CBoard::Save(const char *filename)
{
    std::ofstream fp(filename);
/*
| Tag                   | Description                                                           |
| --------------------- | --------------------------------------------------------------------- |
| `[Event "..."]`       | Name of the event (e.g., "SharokinChess")                             |
| `[Site "..."]`        | Location of the game (e.g., "Gothenburg, SWE")                        |
| `[Date "YYYY.MM.DD"]` | Date of the game. Use `????.??.??` if unknown                         |
| `[Round "..."]`       | Round number or identifier (e.g., "1", "-")                           |
| `[White "..."]`       | Name of the player with the white pieces                              |
| `[Black "..."]`       | Name of the player with the black pieces                              |
| `[Result "..."]`      | Game result: `"1-0"`, `"0-1"`, `"1/2-1/2"`, or `"*"` (for unfinished) |
*/
    fp << "[Event \"SharokinChess\"]" << std::endl;
    fp << "[Site \"\"]" << std::endl;
    fp << "[Date \"\"]" << std::endl;
    fp << "[Round \"-\"]" << std::endl;
    fp << "[White \"\"]" << std::endl;
    fp << "[Black \"\"]" << std::endl;

    fp << "[Result \"";
    int gs = getGameState();
    if (gs == GS_CHECKMATED) {
        getTurn() == SIDE_BLACK ? (fp << "  1-0\"]\n") : (fp << "  0-1\"]\n");
    }
    else {
        fp << "*\"]\n";
    }

    int i = 0;
    char line[50], *str;
    for (; 2 * i < getMoveCount(); i++)
    {
        str = line;
        str += sprintf(str, "%d.%s ", i + 1, getMoveStr(2 * i));
        if(2 * i + 1 < getMoveCount())
            str += sprintf(str, "%s ", getMoveStr(2 * i + 1));
        if (str[-1] == ' ' && str[-2] == '#') {
            str[-2] = '\0';
        }
        else if ((i + 1) % 4 == 0) {
            *str++ = '\n';
            *str++ = '\0';
        }
        fp << line;
    }
    switch (getGameState()) {
    case GS_NORMAL :
        break;
    case GS_CHECKMATED :
        getTurn() == SIDE_BLACK ? (fp << "  1-0\n") : (fp << "  0-1\n");
        break;
    case GS_DRAWBYSTALEMATE :
    case GS_DRAWBYTHREEFOLDREPETITION :
    case GS_DRAWBYFIFTYRULE :
    case GS_DRAWBYINSUFFICIENTMATERIAL :
    case GS_TOOMANYMOVES :
        fp << "  1/2-1/2\n";
        break;
    default:
        break;
    }

    fp.close();

    return 0;
}

//-----------------------------------------------------------------------------
int CBoard::LoadData(HWND h)
{
    hWnd = h;
	__init();
    bAdvanceHead = true;
    bAnalysed = 0;
    bPosAnalysed = 0;
    return 0;
}

int CBoard::AnalysePosition(__int8 *pos, __int8 turn, char *enp, int castle)
{
    short kings[] = {0, 0},
          queens[] = {0, 0},
          rooks[] = {0, 0},
          knights[] = {0, 0},
          bishops_w[] = {0, 0},
          bishops_b[] = {0, 0},
          pawns[] = {0, 0};
    long s[2], t[2];

    s[0] = __sides[0].FirstI + 1;
    s[1] = __sides[1].FirstI + 1;
    t[0] = __sides[0].FirstI + 15;
    t[1] = __sides[1].FirstI + 15;

    char p;
    memset(anaPosition, 0, 100);
    for(int i = s[0]; i <= t[0]; i++)
        anaPosition[i] = CAPTURED;
    for(int i = s[1]; i <= t[1]; i++)
        anaPosition[i] = CAPTURED;
    for(int i = 0; i < 64; i++)
    {
        int x = -1, side =
            (pos[i] >= 'B' && pos[i] <= 'R') ? SIDE_WHITE : SIDE_BLACK;
        switch(pos[i])
        {
        case 'K' :
        case 'k' :
            kings[side]++;
            x = __sides[side].FirstI;
            p = 0;
            break;
        case 'Q' :
        case 'q' :
            queens[side]++;
            x = s[side]++;
            p = PIECE_QUEEN;
            break;
        case 'R' :
        case 'r' :
            rooks[side]++;
            x = s[side]++;
            p = PIECE_ROOK;
            break;
        case 'N' :
        case 'n' :
            knights[side]++;
            x = s[side]++;
            p = PIECE_KNIGHT;
            break;
        case 'B' :
        case 'b' :
            (((i >> 3) + (i & 7)) & 1) ? bishops_w[side]++ : bishops_b[side]++;
            x = s[side]++;
            p = PIECE_BISHOP;
            break;
        case 'P' :
        case 'p' :
            pawns[side]++;
            if(i / 8 == 0 || i / 8 == 7)
                return 10;
            x = t[side]--;
            p = PIECE_PAWN(side);
            break;
        }
        if(x != -1)
        {
            anaPosition[x] = i;
            anaPosition[i] = (1 << (6 + side)) | p;
        }
    }
    if(kings[0] != 1 || kings[1] != 1)
        return 1;
    for(int i = 0; i < 2; i++)
    {
        if(queens[i] > 1)
            pawns[i] += queens[i] - 1;
        if(rooks[i] > 2)
            pawns[i] += rooks[i] - 2;
        if(knights[i] > 2)
            pawns[i] += knights[i] - 2;
        if(bishops_w[i] > 1)
            pawns[i] += bishops_w[i] - 1;
        if(bishops_b[i] > 1)
            pawns[i] += bishops_b[i] - 1;
    }
    if(pawns[0] > 8 || pawns[1] > 8)
        return 2;
    anaPosition[64] = 0xC0;
    if(anaPosition[4] == 0x40)
	{
		if(castle & 1)  // white's short castle
		if(anaPosition[7] == 0x40 | PIECE_ROOK)
			anaPosition[65] |= 0x40;
		if(castle & 2)
		if(anaPosition[0] == 0x40 | PIECE_ROOK)
			anaPosition[65] |= 0x80;
	}
    if((unsigned char)anaPosition[60] == 0x80)
    {
		if(castle & 4)  // black's short castle
		if((unsigned char)anaPosition[63] == 0x80 | PIECE_ROOK)
			anaPosition[65] |= 0x10;
		if(castle & 8)  // black's long castle
		if((unsigned char)anaPosition[56] == 0x80 | PIECE_ROOK)
			anaPosition[65] |= 0x20;
	}
    if(turn != SIDE_WHITE && turn != SIDE_BLACK)
        return -1;
    anaPosition[66] = 1 - turn;
    if(*enp != 0)
    {
        if(enp[0] < 'a' || enp[0] > 'h' || enp[1] < '1' || enp[1] > '8')
            return 3;
        anaPosition[67] = SQUAR(*enp - 'a', enp[1] - '1');
        long ee = anaPosition[67] + __sides[1 - turn].Dir * 8;
        if(~anaPosition[ee] & __sides[1 - turn].BitSide)
            return 3;
        if(PIECETYPE(anaPosition[ee]) != PIECE_PAWN(1 - turn))
            return 3;
    }
    char temp_pos[100];
    memcpy(temp_pos, CurPos, 100);
    memcpy(CurPos, anaPosition, 100);
    if(isChecked())
    {
        memcpy(CurPos, temp_pos, 100);
        return 4;
    }
    anaPosition[66] = turn;
    bPosAnalysed = 1;
    return 0;
}

wchar_t *CBoard::getGameComment(int farsi)
{
    return farsi ? GameFarsiComment : GameLatinComment;
}

int CBoard::setGameComment(wchar_t *comment, int farsi)
{
    wchar_t *temp =
        farsi ? GameFarsiComment
              : GameLatinComment;
    //if(!temp)
    //    temp = &CommentsBuf[CommentsLen];
    if(wcslen(comment) > wcslen(temp))
        temp = &CommentsBuf[CommentsLen];
    wcscpy(temp, comment);
    CommentsLen += wcslen(temp) + 1;
    farsi ? GameFarsiComment = temp
          : GameLatinComment = temp;
    return 0;
}

wchar_t *CBoard::getMoveComment(int move_index, int farsi)
{
    if(move_index <= -1)
        move_index = Head - 1;
    if(move_index <= -1)
        return &CommentsBuf[sizeof(CommentsBuf) / sizeof(wchar_t) - 1];
    return (farsi ? MovesList[move_index].FarsiComment
                  : MovesList[move_index].LatinComment);
}

int CBoard::setMoveComment(int move_index, wchar_t *comment, int farsi)
{
    if(move_index <= -1)
        move_index = Head - 1;
    if(move_index <= -1)
        return 1;
    wchar_t *temp =
        farsi ? MovesList[move_index].FarsiComment
              : MovesList[move_index].LatinComment;
    //if(!temp)
    //    temp = &CommentsBuf[CommentsLen];
    if(wcslen(comment) > wcslen(temp))
        temp = &CommentsBuf[CommentsLen];
    wcscpy(temp, comment);
    CommentsLen += wcslen(temp) + 1;
    farsi ? MovesList[move_index].FarsiComment = temp
          : MovesList[move_index].LatinComment = temp;
    return 0;
}

long *CBoard::getLongPos()
{
    for(long i = 0; i < 100; i++)
        longPos[i] = (unsigned char)CurPos[i];
    return longPos;
}

long CBoard::Think()
{
    short i, m, n, nb;
    COMMOVE *cmv = (COMMOVE *)tempBuf;
    if((n = SearchBook(cmv, &nb)) > 0)
    {
        m = rand() % nb;
        for(i = 0; i < n; i++)
        {
            m -= cmv[i].Bonus;
            if(m < 0)
                break;
        }
        anaMove = cmv[i];
        LegalMove(&anaMove);
        bAnalysed = 4;  // Found in book
        SetEvent(hFinishEvent);
        PostMessage(hWnd, WM_THINKFINISHED, 0, 0);
        return SCORE_FOUNDINBOOK;
    }
    ResetEvent(hStopEvent);
    for(n = 50 - Head / 2; n <= 0; )
        n += 10;
    n = (16 - Head / 8) * n;
    n <= 0 ? n = 1 : 0;
    long time = 2 * (TimeLimit - TotalTimes[getTurn()]) / n;
    if(time < 10000)
        time = 10000;

    MOVE64 *mv64 = new MOVE64[Fifty];
    for(long i = 0; i < Fifty; i++)
        mv64[i] = MovesList[Head - Fifty + i].Move;
    __prepare(getLongPos(), Depth, mv64, Fifty, time, hWnd);
    delete[] mv64;
    
	DWORD temp;
	CreateThread( (SECURITY_ATTRIBUTES *)NULL, 200000,
		(unsigned long(__stdcall *)(void *))__think, (void *)NULL, 0, &temp);
	return 0;
}

int CBoard::IsThinking()
{
    return (int)(WaitForMultipleObjects(1, &hThinkEvent, TRUE, 0) == WAIT_OBJECT_0);
}

int CBoard::ThinkFinished(void)
{
    if(WaitForMultipleObjects(1, &hFinishEvent, TRUE, 0) == WAIT_OBJECT_0)
    {
        if(!bAnalysed)
        {
            if(__bestmovefound())
            {
                anaMove.Move = __getbestmove();
            }
            else {
                anaMove.Move.from = 0;
                anaMove.Move.to = 0;
            }
            LegalMove(&anaMove);
            anaMove.BookOffs = 0;
            bAnalysed = 1;
        }
        return 1;
    }
    return 0;
}

void CBoard::StopThinking(void)
{
    SetEvent(hStopEvent);
    bAnalysed = 0;
}

typedef union
{
    struct
    {
        unsigned __int8 from;
        unsigned __int8 to;
    };
    short move;
}
MOVE16;
MOVE16 m16;

int CBoard::SearchBook(COMMOVE *pt_move, short *nb)
{
    if(!OpenBook)
        return 0;
    long off, n, m, arr_nest[MAX_BOOKDEPTH], nest, bit;

    off = Head > 0 ? MovesList[Head - 1].BookOffs : BookInitPos;
    if(off == 0)
        return 0;

    short bonus[] = {1, 50, 1000, 1400};
    m = 0;
    *nb = 0;
    arr_nest[m++] = 0;
    for(n = 0, nest = 0, bit = 0; off < (long)OpenBookSize; off += 2)
    {
        m16.move = *(short*)(OpenBook + off);
        bit = m16.from >> 6;
        if(nest <= 0)
        {
            pt_move[n].Bonus = bonus[m16.to >> 6];
            //pt_move[n].Move.move &= 0x0000003f0000003f;
            pt_move[n].Move.from = m16.from & 0x3f;
            pt_move[n].Move.to   = m16.to & 0x3f;
            pt_move[n].BookOffs = (bit & 2) ? 0 : off + 2;
            *nb += pt_move[n].Bonus;
            n++;
            if(bit & 1)
                break;
        }
        if(bit == 0)
            arr_nest[m++] = nest;
        if(~bit & 2)
            nest++;
        if(bit == 3)
            nest = arr_nest[--m];
    }
    return (int)n;
}

int CBoard::ReadOpenBook(char *filename)
{
    FILE* fp = fopen(filename, "rb");
    if (fp == NULL) {
        return -1;
    }
    closeOpenBook();

    fseek(fp, 0, SEEK_END);
    fpos_t pos;
    fgetpos(fp, &pos);
    fseek(fp, 0, SEEK_SET);

    OpenBookSize = pos;
    OpenBook = OpenBookSize > 0 ? new char[OpenBookSize] : NULL;
    fread(OpenBook, 1, OpenBookSize, fp);
    fclose(fp);
    return (int)OpenBookSize;
}

short CBoard::Notation2Move(char *str)
{
    if(*str == ' ' || *str == '\n')
        return -1;

    short len = strlen(str);
    char side = CurPos[66], piece,
        from = 100, to = 100, extra = '-';
    if(*str == 'O')
    {
        from = 4 + side * 56;
        if(len == 3) // short castle
            to = 6 + 56 * side;
        else if(len == 5) // long castle
            to = 2 + 56 * side;
    }
    else
    {
        piece = Char2Piece(*str);
        if(piece == '-')
            piece = PIECE_PAWN(side);
        if(len > 3)
            if(str[2] == '=')   // pawn promotion
            {
                str[2] = str[3] = 0;
                len = 2;
            }
        if(piece == PIECE_PAWN(side))
        {
            if(str[0] != 'x' && len > 2)
                extra = str[0];
        }
        else if(str[1] != 'x' && len > 3)
        {
            extra = str[1];
            if(str[2] != 'x' && len > 4)
                from = str[1] - 'a' + 8 * (str[2] - '1');
        }
        to = (str[len - 2] - 'a') + 8 * (str[len - 1] - '1');
    }
    short i, n = generateMoves(), move = -1;
    MOVE64 *mv = (MOVE64 *)tempBuf;
    for(i = 0; i < n; i++)
    {
        if(mv[i].to == to)
        {
            if(from != 100)
            {
                if(mv[i].from == from)
                    break;
            }
            else
            {
                if(PIECETYPE(CurPos[mv[i].from]) == piece &&
                        SIDE_OF(CurPos[mv[i].from]) == __sides[side].BitSide)
                {
                    if(extra == '-')
                        break;
                    if(extra >= '1' && extra <= '8')
                        if(extra == RANK_OF_SQUAR(mv[i].from) + '1')
                            break;
                    if(extra >= 'a' && extra <= 'h')
                        if(extra == LINE_OF_SQUAR(mv[i].from) + 'a')
                            break;
                }
            }
        }
    }
    if(i < n)
        move = mv[i].move;
    return move;
}

char Char2Piece(char ch)
{
    switch(ch)
    {
    case 'K' : return PIECE_KING;
    case 'Q' : return PIECE_QUEEN;
    case 'R' : return PIECE_ROOK;
    case 'B' : return PIECE_BISHOP;
    case 'N' : return PIECE_KNIGHT;
    }
    return '-';
}

int CBoard::SetLevel(long depth, long timelimit)
{
    Depth = (depth <= 0 || depth >= MAX_DEPTH ? MAX_DEPTH : depth);
    TimeLimit = (timelimit > 0 ? timelimit : 6000000L);
    return 0;
}

char *CBoard::getMoveStr(int i)
{
    if(i <= -1)
        i = Head - 1;
    static char str[8];
    str[0] = 0;
    if(i >= 0 && i < NumMoves)
        strcpy(str, MovesList[i].str);
    return str;
}

void CBoard::closeOpenBook()
{
    if(OpenBook)
    {
        delete[] OpenBook;
        OpenBook = NULL;
        OpenBookSize = 0;
    }
}

int CBoard::getHint(int *sx, int *sy, int *dx, int *dy)
{
    *sx = -1;
    if(bAnalysed)
    {
        *sx = anaMove.Move.from >> 3;
        *sy = anaMove.Move.from &  7;
        *dx = anaMove.Move.to   >> 3;
        *dy = anaMove.Move.to   &  7;
        return 0;
    }
    return 1;
}
/*
int CBoard::canMovePieceAt(int x, int y)
{
    int num = generateMoves() - 1;
    for(MOVE64 *mb = (MOVE64 *)tempBuf; num >= 0; num--, mb++)
        if(mb->from == SQUAR(x, y))
            return 1;
        else if(mb->to >= 100)
        if(SQUAR(LINE_OF_SQUAR(mb->from),
            __sides[getTurn()].LastRank - __sides[getTurn()].Dir) == SQUAR(x, y))
            return 1;
    return 0;
}
*/
void CBoard::setPromotion(__int8 p)
{
    if(bAnalysed == 2)
        anaMove.Move.to = p + 100;
}

int CBoard::isStalemated()
{
    return generateMoves() == 0 && !isChecked();
}

int CBoard::isCheckmated()
{
    return generateMoves() == 0 && isChecked();
}

int CBoard::getMoveStr(COMMOVE *move, MOVE64 *mlist, int num)
{
    MOVE64 m, mm;
    m.move = move->Move.move;
    memset(move->str, 0, sizeof(move->str));

    int i, j, p, pro = -1, s = CurPos[66];
    char piece_names[] = "KQNRBPPPP",
            short_castle[] = "O-O", long_castle[] = "O-O-O", *str = move->str;
    if(m.to < 100)
        p = PIECETYPE(CurPos[m.from]);
    else
    {
        p = PIECE_WPAWN;
        pro = m.to - 100;
        i = LINE_OF_SQUAR(m.from);
        j = RANK_OF_SQUAR(m.from);
        m.from = SQUAR(i, __sides[s].LastRank - __sides[s].Dir);
        m.to = SQUAR(j, __sides[s].LastRank);
    }
    // castle moves
    if(p == PIECE_KING && m.from == __sides[getTurn()].KingInitPos)
        if(m.to == __sides[getTurn()].KingInitPos + 2)
        {
            strcpy(str, short_castle);
            return 0;
        }
        else if(m.to == __sides[getTurn()].KingInitPos - 2)
        {
            strcpy(str, long_castle);
            return 0;
        }
    if(*str == 'O')
        return 0;

    *str = piece_names[p];
    if(*str != 'P')
        str++;
    for(i = j = 0; i < num; i++)
    {
        mm = mlist[i];
        if(mm.from != m.from)
        if(PIECETYPE(CurPos[mm.from]) == p)
        if(mm.to == m.to)
            j |= (LINE_OF_SQUAR(mm.from) == LINE_OF_SQUAR(m.from)) ? 2 : 1;
    }
    if(j & 1)
        *str++ = 'a' + LINE_OF_SQUAR(m.from);
    if(j & 2)
        *str++ = '1' + RANK_OF_SQUAR(m.from);
    if(IS_NOTEMPTY(CurPos[m.to]))
    {
        if(p >= PIECE_WPAWN)
            if(j == 0)
                *str++ = 'a' + LINE_OF_SQUAR(m.from);
        *str++ = 'x';
    }
    *str++ = 'a' + LINE_OF_SQUAR(m.to);
    *str++ = '1' + RANK_OF_SQUAR(m.to);
    if(p >= PIECE_WPAWN)
        if(pro != -1)
        {
            *str++ = '=';
            *str++ = piece_names[pro];
        }

    return 0;
}

int CBoard::isChecked()
{
    return __ischecked(getLongPos());
}

/*
int CBoard::Test(DWORD *t, long *gc, long *mc)
{
    char tempBuf2[8000];
    MOVE64 *mb64  = (MOVE64 *)tempBuf2;
    DWORD t1 = GetTickCount(), t2, i, j, m, n, mm;
    __generateMovesCount = 0;
    __makemoveCount = 0;
    __compact(getLongPos());
    POSSCORE *ps = (POSSCORE *)longPos;
    __evaluate0(ps);
    mm = 0;
    for(i = 0; i < 50000; i++)
    {
        __compact(getLongPos());
        n = __generateMoves(longPos, mb64);
        for(j = 0; j < n; j++)
        {
            __evaluate(ps, mb64[j]);
        }
        //if(n > 0)
        //{
        //    for(m = 0; m < n - 1; m++)
        //    for(j = n - 1; j > m; j--)
        //        if(movscor[j].score > movscor[j - 1].score)
        //        {
        //            movscor[99] = movscor[j];
        //            movscor[j] = movscor[j - 1];
        //            movscor[j - 1] = movscor[99];
        //        }
        //}
        //for(j = 0; j < n; j++)
        //{
        //    __compact(getLongPos());
        //    __makemove(longPos, mb64[j]);
        //    mm++;
        //}
    }
    t2 = GetTickCount();
    *t = t2 - t1;
    *gc = __generateMovesCount;
    *mc = (long)mm;//__makemoveCount;
    return 0;
}
*/
long CBoard::getGameState()
{
    long ret = GS_NORMAL, a[64], b = 0, c = 1;
    memset(a, 0, 64 * sizeof(long));
    ///---test; temporarily for Fifty bug; 98-06-25; commented; start
    /*
    for(long i = Head - 1; i >= Head - Fifty; i--)
    {
        (++a[MovesList[i].Move.from] == 0) ? --b : ++b;
        (--a[MovesList[i].Move.to  ] == 0) ? --b : ++b;
        b == 0 ? ++c : 0;
    }
    */
    ///---test; temporarily for Fifty bug; 98-06-25; commented; end
    if(NumMoves >= MAX_MOVES)
        ret = GS_TOOMANYMOVES;
    else if(generateMoves() == 0)
        ret = (isChecked() ? GS_CHECKMATED : GS_DRAWBYSTALEMATE);
    else if(c >= 3)
        ret = GS_DRAWBYTHREEFOLDREPETITION;
    else if(Fifty >= FIFTY)
        ret = GS_DRAWBYFIFTYRULE;
    else if(isChecked())
        ret = GS_CHECKED;
    else
    {
        long st[2];
        __checkstalemate(getLongPos(), st);
        if(st[0] == __BETA / 4 && st[1] == __BETA / 4)
            ret = GS_DRAWBYINSUFFICIENTMATERIAL;
    }
    if(ret != GS_CHECKED && ret != GS_NORMAL)
        StartTime = 0;
    return ret;
}

long CBoard::GetDepth()
{
    return __depth;
}

long CBoard::getTime(long side)
{
    if(Head <= 0)
        return 0;
    long time;
    if(Head >= NumMoves)
    {
        time = TotalTimes[side] +
                (side == getTurn() && StartTime > 0 ? GetTickCount() - StartTime : 0);
    }
    else
    {
        time = 0;
        for(long i = side; i < Head; i += 2)
            time += MovesList[i].Time;
    }
    return time;
}

