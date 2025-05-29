
#include <stdlib.h>
#include <stdio.h>
#include "think.h"

#define POSITION_VALUES
#define PAWNSTRUCTURE_VALUES
#define XPOSLIST
#define FOLLOW_PV
//#define __LOOKTABLE
#define ZOBRIST_KEYS

#define NEW_EVALUATION

///---test; check the result; added; 98-07-11; start
// I want {KING_VALUE + (-128 -30 -30 -50 +8*(5-15)) }
// be larger than zero (value[0],[1] be positive in __vantage)
#define KING_VALUE  250 //4291
///---test; check the result; added; 98-07-11; end

#define PROMOTION_BONUS  100000L
#define CAPTURE_BONUS    100000L
#define FOLLOW_BONUS     200000L

HANDLE hWriteEvent, hReadEvent, hFinishEvent, hThinkEvent, hStopEvent;

long __diradv[] =
{1, 9, 8, 7, -1, -9, -8, -7,
10,17,15, 6,-10,-17,-15, -6, 0};
long __dir[64] =
{
    0x0307, 0x071f, 0x0f1f, 0x0f1f,   0x0f1f, 0x0f1f, 0x0e1f, 0x0c1c,
    0x83c7, 0x87ff, 0x9fff, 0x9fff,   0x9fff, 0x9fff, 0x1eff, 0x1c7c,
    0xc3c7, 0xe7ff, 0xffff, 0xffff,   0xffff, 0xffff, 0x7eff, 0x3c7c,
    0xc3c7, 0xe7ff, 0xffff, 0xffff,   0xffff, 0xffff, 0x7eff, 0x3c7c,
    0xc3c7, 0xe7ff, 0xffff, 0xffff,   0xffff, 0xffff, 0x7eff, 0x3c7c,
    0xc3c7, 0xe7ff, 0xffff, 0xffff,   0xffff, 0xffff, 0x7eff, 0x3c7c,
    0xc1c7, 0xe1ff, 0xf9ff, 0xf9ff,   0xf9ff, 0xf9ff, 0x78ff, 0x387c,
    0xc0c1, 0xe0f1, 0xf0f1, 0xf0f1,   0xf0f1, 0xf0f1, 0x70f1, 0x3070
};
long __direction[4096];
long flip[] =
{
   56, 57, 58, 59,   60, 61, 62, 63,
   48, 49, 50, 51,   52, 53, 54, 55,
   40, 41, 42, 43,   44, 45, 46, 47,
   32, 33, 34, 35,   36, 37, 38, 39,
   24, 25, 26, 27,   28, 29, 30, 31,
   16, 17, 18, 19,   20, 21, 22, 23,
    8,  9, 10, 11,   12, 13, 14, 15,
    0,  1,  2,  3,    4,  5,  6,  7,
    64
};
long pt2[16] = {0, 1, 2, 3, 4, 7, 8};

#define ALLOW_MOVE2(_f, _t, _ks) \
    allow_move = 1;              \
    m_dsf = DIR(_ks, _f);        \
    m_ds  = __diradv[m_dsf];     \
    m_dst = DIR(_ks, _t);        \
    if(m_ds && m_dsf != m_dst)   \
    {                            \
        for(m_s = _ks + m_ds; m_s != _f; m_s += m_ds) \
			if(IS_NOTEMPTY(pos[m_s]))           \
                break;                          \
        if(m_s == _f)                           \
        {                                       \
            for(; (1 << m_dsf) & __dir[m_s];)   \
            {                                   \
                m_s += m_ds;                    \
                if(pos[m_s] & bit_side)         \
                    break;                      \
                else if(pos[m_s] & bit_oppside) \
                {                               \
                    if(IRAYS2(PIECETYPE2(pos[m_s]), m_s, _ks)) \
                        allow_move = 0;         \
                    break;                      \
                }   \
            }       \
        }           \
    }

#define ATTACKED2(_s, _t)  attacked = __attacked(pos, _s, _t, &_xa);

#define RETURN \
    long __m = mb - moves_buf, __n;      \
    for(mb = moves_buf, __n = __m; __n > 1; __n--, mb++) \
    {                                    \
        x = rand() % (__n - 1) + 1;      \
        MOVE64 tm = *mb;                 \
        *mb = mb[x];                     \
        mb[x] = tm;                      \
    }                                    \
    return __m;

SIDEINFO  __sides[2];

struct
{
    long* Rays2;
    __int64 iRays[64];
}
__pieces[NUM_PIECES];

long __movesdata2[7777];

typedef struct
{
    MOVE64  move;
    long    score;
}
MOVESCORE;
#define MOVESCORE_SIZE   4   // sizeof(MOVESCORE) / sizeof(long)

MOVESCORE firstLevelMoves[100];

typedef struct
{
    long Material;
    long PosValues;
    long CastleValue;
    long PawnCount, PieceCount;
    long bitPawns, xbitPawns[8], ybitPawns[8];
}
SCORE;

typedef struct
{
    long  pos[100];
    SCORE sco[2];
}
POSSCORE;
#define POSSCORE_SIZE   144   // sizeof(POSSCORE) / sizeof(long)

int firstLevelMoves_Count;

long *__thinkPtr, __thinkBuf[5000000];
long  __depth, __capdepth, __maxdepth, __maxcapdepth, __virtualdepth;
MOVE64 *__bestmovesok, *__bestmoves[MAX_DEPTH],
        *__capbestmoves[MAX_DEPTH], temp_moves[200], *__chain;
long  __chaincount;
long  __followpv, __capfollowpv, __fifty;

HWND __hwnd;

long __chainval[256];
long __bitcount[256];
long __stalematevalue[2];
long __starttime, __timelimit, __lastmoveTime, __generateMovesCount, __makemoveCount, __nodescount;

long __evaluate0(POSSCORE *pos);
long __evaluate(POSSCORE *pos, MOVE64 move);
long __preanalyse(long* pos);
long __attacked(long *pos, long _s, long _t, long *);
long __alphabeta(long depth, long alpha, long beta);
long __alphabetacap(long depth, long alpha, long beta);
long __vantage(POSSCORE *sco);
long __distance(long x, long y);
long __countrepetition();
long __looktable(long *pos), __addtable(long *pos, long score);

void zb__insert(unsigned __int64 key, long score, long depth);
long zb__lookup(unsigned __int64 key, long depth);
unsigned __int64 __boardkey(long *pos);
void zb__initkeys();

long __init()
{
    DWORD offs[NUM_PIECES];

    FILE *fp = fopen("chess2.dat", "rb");
    if(fp == NULL) {
        return -1;
    }
    fread(offs, sizeof(long), NUM_PIECES, fp);
    fread(__movesdata2, 7672, sizeof(long), fp);
    for(int p = 0; p < NUM_PIECES; p++)
        fread(__pieces[p].iRays, 64, sizeof(__int64), fp);
    fread(__direction, 4096L, sizeof(long), fp);
    fclose(fp);

    for(int p = 0; p < NUM_PIECES; p++)  // all pieces, except capture pawns
        __pieces[p].Rays2 = __movesdata2 + offs[p];

    __sides[0].FirstI = 68;
    __sides[0].LastI  = 84;
    __sides[0].Dir = 1;
    __sides[0].PawnMove = 8;
	__sides[0].LastRank = 7;
    __sides[0].OneBeforeLastRank = 6;
    __sides[0].BitSide = 0x40;
    __sides[0].BitOppSide = 0x80;
    __sides[0].CanCastleBit[0] = 0x40;
    __sides[0].CanCastleBit[1] = 0x80;
    __sides[0].CastledBit[0] = 0x4;
    __sides[0].CastledBit[1] = 0x8;
    __sides[0].KingInitPos = 4;

    __sides[1].FirstI = 84;
    __sides[1].LastI  = 100;
    __sides[1].Dir = -1;
    __sides[1].PawnMove = -8;
    __sides[1].LastRank = 0;
    __sides[1].OneBeforeLastRank = 1;
    __sides[1].BitSide = 0x80;
    __sides[1].BitOppSide = 0x40;
    __sides[1].CanCastleBit[0] = 0x10;
    __sides[1].CanCastleBit[1] = 0x20;
    __sides[1].CastledBit[0] = 0x1;
	__sides[1].CastledBit[1] = 0x2;
    __sides[1].KingInitPos = 60;

    long chain_value[] = {0, 10, 25, 50, 75, 100, 125, 150},
         island_value[] = {0, -5, -10, -15, -20, -25, -30, -35};
    for(long i = 0, j, k; i < 256; i++)
    {
        __bitcount[i] = 0;
        for(j = 0; j < 8; j++)
            if(i & (1 << j))
                __bitcount[i]++;
        __chainval[i] = 0;
        for(j = 0; j < 8; j++)
            if(i & (1 << j))
                break;
        for(k = 0; j < 8;)
        {
            __chainval[i] += island_value[k];
            for(k = 0; j < 8; j++, k++)
                if(~i & (1 << j))
                    break;
			__chainval[i] += chain_value[k];
            for(k = 0; j < 8; j++, k++)
                if(i & (1 << j))
                    break;
        }
    }
    __bestmovesok = NULL;
    zb__initkeys();
    return 0;
}

long __generateMoves(long *pos, MOVE64 *moves_buf)
{
    // pos
    // 0..63  describes board
    // 64     always equals 0xc0
    // 65     castle state :
	// 66     turn : 0 white, 1 black
    // 67     enpassant squar, if exist. (pos[67] == 100 no enpassant)
    // 68..83 white(0) pieces positions, [68] is the king position
    // 84..99 black(1) pieces positions, [84] is the king position
    long side = pos[66];
    long first_i = __sides[side].FirstI,
         last_i  = __sides[side].LastI;

    long pawn_move = __sides[side].PawnMove;
    long bit_side = __sides[side].BitSide,
         bit_oppside = __sides[side].BitOppSide;
    long oppfirst_i = __sides[1 ^ side].FirstI;
         //opplast_i  = __sides[1 ^ side].LastI;
    long last_rank = __sides[side].LastRank;
    register long *base, *ptr, adv;
    long king_sq = pos[first_i];
    long attacked, allow_move, _xa;
    register long i, j, s;
    register long m, n, x, p, d, f;
    MOVE64 *mb = moves_buf;

	__generateMovesCount++;
    // King moves
    ptr = RAYS2(PIECE_KING, king_sq);
    n = *ptr++;
    for(i = 0; i < n; i++)
    {
        if(~pos[*ptr] & bit_side)
        {   // There is no any piece of color side
            pos[king_sq] = 0;
            ATTACKED2(*ptr, 1)
            pos[king_sq] = bit_side;
            if(!attacked)
            {
                mb->from = king_sq;
                mb->to = *ptr;
                mb++;
            }
        }
        ptr++;
    }

	long gs, ds;
    long m_dsf, m_dst, m_ds, m_s; // ALLOW_MOVE variables
    long pl[8];
    ATTACKED2(king_sq, 2)
    if(attacked >= 2)
    {
        //RETURN
        return mb - moves_buf;
    }
    if(attacked)
    {
        n = 0;
        if(PIECETYPE(pos[_xa]) == PIECE_KNIGHT)
            pl[n++] = _xa;
        else
        {
            d = __diradv[DIR(f = _xa, king_sq)];
            for(; f != king_sq;)
            {
                pl[n++] = f;
                f += d;
			}
        }
        for(i = 0; i < n; i++)
        {
            gs = pl[i]; // pl[0] equals the gs calculated above
            for(s = first_i + 1; s < last_i; s++)
            if((x = pos[s]) != CAPTURED)
            {
                p = PIECETYPE(pos[x]);
                if(p == PIECE_PAWN(side))
                {
                    if(LINE_OF_SQUAR(x) == LINE_OF_SQUAR(gs))
                    {
                        if(i > 0)
                        if(IRAYS2(PIECE_PAWN(side), x, gs))
                        if(ABS(gs - x) != 16 || !IS_NOTEMPTY(pos[x + gs >> 1]))
                        {
                            ALLOW_MOVE2(x, gs, king_sq)
                            if(allow_move)
                            if(RANK_OF_SQUAR(gs) != last_rank)
                            {
								mb->from = x;
                                mb->to   = gs;
                                mb++;
                            }
                            else
                                for(p = PIECE_QUEEN; p <= PIECE_BISHOP; p++)
                                {
                                    mb->from = SQUAR(LINE_OF_SQUAR(x), LINE_OF_SQUAR(gs));
                                    mb->to   = 100 + p;
                                    mb++;
                                }
                        }
                    }
                    else if(IRAYS2(PIECE_CAPTUREPAWN(side), x, gs))
                    {
                        if(i == 0)
                        {
                            ALLOW_MOVE2(x, gs, king_sq)
                            if(allow_move)
                            if(RANK_OF_SQUAR(gs) != last_rank)
                            {
								mb->from = x;
                                mb->to   = gs;
                                mb++;
                            }
                            else
                                for(p = PIECE_QUEEN; p <= PIECE_BISHOP; p++)
                                {
                                    mb->from = SQUAR(LINE_OF_SQUAR(x), LINE_OF_SQUAR(gs));
                                    mb->to   = 100 + p;
                                    mb++;
                                }
                        }
                    }
                    else if(gs == pos[67] - pawn_move)
                        if(IRAYS2(PIECE_CAPTUREPAWN(side), x, pos[67]))
                        {
                            ALLOW_MOVE2(x, pos[67], king_sq)
                            if(allow_move)
                            {
                                ALLOW_MOVE2(gs, x, king_sq)
                                if(allow_move)
								{
                                    mb->from = x;
                                    mb->to   = pos[67];
                                    mb++;
                                }
                            }
                        }
                }
                else if(IRAYS2(p, x, gs))
                {
                    if(p != PIECE_KNIGHT)
                    {
						m = MAX(ABS(LINE_OF_SQUAR(gs) - LINE_OF_SQUAR(x)),
                                ABS(RANK_OF_SQUAR(gs) - RANK_OF_SQUAR(x)));
                        //m = DISTANCE(x, gs);   // DISTANCE is a pre-calculated array
                        d = __diradv[DIR(x, gs)];
                        f = x + d;
                        for(j = 1; j < m; j++)
                        {
                            if(IS_NOTEMPTY(pos[f]))
                                break;
							f += d;
                        }
                        if(j < m)
                            continue;
                    }
                    ALLOW_MOVE2(x, gs, king_sq)
                    if(allow_move)
                    {
                        mb->from = x;
                        mb->to   = gs;
                        mb++;
                    }
                }
            }
        }
        //RETURN
        return mb - moves_buf;
    }

    // Queen, Knight, Rook and Bishop moves
    for(s = first_i + 1; s < last_i; s++)
	if((x = pos[s]) != CAPTURED)
    if((p = PIECETYPE(pos[x])) == PIECE_KNIGHT)
    {
        ptr = RAYS2(PIECE_KNIGHT, x);
        n = *ptr++;
        for(i = 0; i < n; i++)
        {
            if(~pos[*ptr] & bit_side)
            { // There is no any piece of color side
                ALLOW_MOVE2(x, *ptr, king_sq);
                if(allow_move)
                {
                    mb->from = x;
                    mb->to = *ptr;
                    mb++;
                }
            }
            ptr++;
        }
    }
    else if(p <= PIECE_BISHOP) // queen, rook & bishop moves
	{
        ptr = base = RAYS2(p, x);
        m = *ptr++;
        for(j = 0; j < m; j++)
        {
            adv = *ptr++;
            n = *ptr++;
            for(i = 0; i < n; i++)
            {
                if(pos[*ptr] & bit_side)
                { // There is a piece of color side
                    ptr = base + adv;
                    break;
                }
                else
                {
                    ALLOW_MOVE2(x, *ptr, king_sq);
                    if(allow_move)
                    {
                        mb->from = x;
                        mb->to = *ptr;
						mb++;
                    }
                    if(pos[*ptr] & bit_oppside)
                    { // There is a piece of color oppside
                        ptr = base + adv;
                        break;
                    }
                }
                ptr++;
            }
        }
    }

    // Pawn moves
    for(s = first_i; s < last_i; s++)
    if((x = pos[s]) != CAPTURED)
    if((p = PIECETYPE(pos[x])) == PIECE_PAWN(side))
    {
        ptr = RAYS2(p, x);
        n = *ptr++;
        for(i = 0; i < n; i++)
		{
            if(IS_NOTEMPTY(pos[*ptr]))
                break;
            ALLOW_MOVE2(x, *ptr, king_sq);
            if(allow_move)
            {
                ptr++;
                mb->from = *ptr++;
                mb->to   = *ptr++;
                mb++;
            }
            else
                ptr += 3;
        }
        ptr = RAYS2(p = PIECE_CAPTUREPAWN(side), x);
        n = *ptr++;
        for(i = 0; i < n; i++)
        {
            d = 0;
            if(pos[*ptr] & bit_oppside)
            {
				ALLOW_MOVE2(x, *ptr, king_sq);
                if(allow_move)
                    d = 1;
            }
            else if(*ptr == pos[67])
            {
                gs = pos[pos[67] - pawn_move];
                pos[pos[67] - pawn_move] = 0;
                pos[pos[67]] = pos[x];
                pos[x] = 0;
                ATTACKED2(king_sq, 1)
                if(!attacked)
                    d = 1;
                pos[pos[67] - pawn_move] = gs;
                pos[x] = pos[pos[67]];
                pos[pos[67]] = 0;
            }
            if(d)
            {
                ptr++;
                mb->from = *ptr++;
				mb->to   = *ptr++;
                mb++;
            }
            else
                ptr += 3;
        }
    }

    // short castle move
    if(pos[65] & __sides[side].CanCastleBit[0])
    {
        p = king_sq + 2;
        for(x = king_sq + 1; x <= p; x++)
        {
            if(IS_NOTEMPTY(pos[x]))
                break;
            ATTACKED2(x, 1)
            if(attacked)
                break;
        }
        if(x > p)
		{
            mb->from = king_sq;
            mb->to   = king_sq + 2;
            mb++;
        }
    }
    // long castle move
    if(pos[65] & __sides[side].CanCastleBit[1])
    {
        p = king_sq - 1;
        for(x = king_sq - 3; x <= p; x++)
        {
            if(IS_NOTEMPTY(pos[x]))
                break;
            ATTACKED2(x, 1)
            if(attacked)
                break;
        }
        if(x > p)
        {
            mb->from = king_sq;
			mb->to   = king_sq - 2;
            mb++;
        }
    }
    //RETURN
    return mb - moves_buf;
}

long
base_piece_values[] =
///---test; check the result; changed; 98-07-11; start
//{ 4291, 900, 300, 500, 300, 100, 100, 100, 100},
{ KING_VALUE, 900, 300, 500, 300, 100, 100, 100, 100},
///---test; check the result; changed; 98-07-11; end
//  {0, 0x1000, 0x900, 0x4c0, 0x300, 0x300, 0x100},
piece_values[2][9],
pawn_on_rank[] =
  {0, 0, 10, 13, 20, 40, 70, 0},
kpv_surface[15][15],
center_attack_val[64] =
{
 -5,  -2,   0,   0,   0,   0,  -2,  -5,
 -2,   4,   8,  10,  10,   8,   4,  -2,
  0,   8,  14,  20,  20,  14,   8,   0,
  0,  10,  20,  30,  30,  20,  10,   0,
  0,  10,  20,  30,  30,  20,  10,   0,
  0,   8,  14,  20,  20,  14,   8,   0,
 -2,   4,   8,  10,  10,   8,   4,  -2,
 -5,  -2,   0,   0,   0,   0,  -2,  -5
},
forward_attack_val[64] =
{
  5,   5,   5,   5,   5,   5,   5,   5,
  5,   5,   5,   5,   5,   5,   5,   5,
  4,   4,   4,   4,   4,   4,   4,   4,
  2,   2,   2,   2,   2,   2,   2,   2,
  1,   1,   1,   1,   1,   1,   1,   1,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
  0,   0,   0,   0,   0,   0,   0,   0,
},
attack_val[64],
king_pos_v1[] =
{
   50,  40,  30,  10, -10, -30, -40, -60,
   40,  50,  40,  20,  10,   0, -30, -40,
   30,  40,  50,  45,  40,  20,   0, -30,
   10,  20,  45,  60,  50,  40,  10, -10,
  -10,  10,  40,  50,  60,  45,  20,  10,
  -30,   0,  20,  40,  45,  50,  40,  30,
  -40, -30,   0,  10,  20,  40,  50,  40,
  -60, -40, -30, -10,  10,  30,  40,  50
},
base_piece_pos_values[][64] =
{
 { // begin_king_lv[64] =
 /*
  -50, -50, -50, -50, -50, -50, -50, -50,
  -50, -50, -50, -50, -50, -50, -50, -50,
  -50, -50, -50, -50, -50, -50, -50, -50,
  -50, -50, -50, -50, -50, -50, -50, -50,
  -30, -30, -30, -30, -30, -30, -30, -30,
  -25, -25, -25, -25, -25, -25, -25, -25,
  -15, -15, -15, -15, -15, -15, -15, -15,
	0,   0,  10, -15, -15,   0,  20,   0			// fix later by giving bonus for
  */
  -40, -40, -40, -40, -40, -40, -40, -40,
  -40, -40, -40, -40, -40, -40, -40, -40,
  -40, -40, -40, -40, -40, -40, -40, -40,
  -40, -40, -40, -40, -40, -40, -40, -40,
  -30, -30, -30, -30, -30, -30, -30, -30,
  -25, -25, -25, -25, -25, -25, -25, -25,
  -15, -15, -15, -15, -15, -15, -15, -15,
   10,  20,  30,   0,   0,   0,  30,  20
 },
 { // queen_lv[64] =
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0
 },
 { // knight_lv[64] =
/*
  -10, -10, -10, -10, -10, -10, -10, -10,
  -10,   5,   5,   7,   7,   5,   5, -10,
    4,   5,  10,  12,  12,  10,   5,   4,
  -10,   5,  12,  15,  15,  12,   5, -10,
  -10,   5,  12,  15,  15,  12,   5, -10,
    4,   5,  10,  12,  12,  10,   5,   4,
  -10,   5,   5,   7,   7,   5,   5, -10,
  -10, -10, -10, -10, -10, -10, -10, -10
*/
  -10, -10, -10, -10, -10, -10, -10, -10,
  -10,   0,   0,   0,   0,   0,   0, -10,
  -10,   0,   5,   5,   5,   5,   0, -10,
  -10,   0,   5,  10,  10,   5,   0, -10,
  -10,   0,   5,  10,  10,   5,   0, -10,
  -10,   0,   5,   5,   5,   5,   0, -10,
  -10,   0,   0,   0,   0,   0,   0, -10,
  -10, -30, -10, -10, -10, -10, -30, -10
 },
 { // rook_lv[64] =
    0,   0,   0,   0,   0,   0,   0,   0,
   30,  30,  30,  30,  30,  30,  30,  30,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,  10,  10,  10,  10,   0,   0
 },
 { // bishop_lv[64] =
  -10,  -5,  -5,  -5,  -5,  -5,  -5, -10,
   -5,   0,   0,   0,   0,   0,   0,  -5,
   -5,   4,   4,   5,   5,   4,   4,  -5,
   -5,   8,   7,  10,  10,   7,   8,  -5,
   -5,   5,   7,  10,  10,   7,   5,  -5,
   -5,  10,   4,   5,   5,   4,  10,  -5,
   -5,  20,   0,   0,   0,   0,  20,  -5,
  -10,  -5, -10,  -5,  -5, -10,  -5, -10
 },
 {   // pawn_lv
/*
	0,   0,   0,   0,   0,   0,   0,   0,	// WHITE
   50,  50,  50,  50,  50,  50,  50,  50,
   10,  10,  13,  15,  15,  13,  10,  10,
	7,   7,  11,  10,  10,  11,   7,   7,
	5,   3,  12,  15,  15,  12,   3,   5,
    8,   2,   8,  -5,  -5,   8,   2,   8,
	0,   0,   0,  -5,  -5,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0
*/
	0,   0,   0,   0,   0,   0,   0,   0,
	5,  10,  15,  20,  20,  15,  10,   5,
	4,   8,  12,  16,  16,  12,   8,   4,
	3,   6,   9,  12,  12,   9,   6,   3,
	2,   4,   6,   8,   8,   6,   4,   2,
	1,   2,   3, -10, -10,   3,   2,   1,
	0,   0,   0, -40, -40,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0
 },
 {   // pawn_lv
 /*
	0,   0,   0,   0,   0,   0,   0,   0,	// BLACK
   50,  50,  50,  50,  50,  50,  50,  50,
   10,  10,  13,  15,  15,  13,  10,  10,
	7,   7,  11,  10,  10,  11,   7,   7,
	5,   3,  12,  15,  15,  12,   3,   5,
    8,   2,   8,  -5,  -5,   8,   2,   8,
	0,   0,   0,  -5,  -5,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0
 */
	0,   0,   0,   0,   0,   0,   0,   0,
	5,  10,  15,  20,  20,  15,  10,   5,
	4,   8,  12,  16,  16,  12,   8,   4,
	3,   6,   9,  12,  12,   9,   6,   3,
	2,   4,   6,   8,   8,   6,   4,   2,
	1,   2,   3, -10, -10,   3,   2,   1,
	0,   0,   0, -40, -40,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0
 }
},
pawn_prom_value[] =
{//mat_level=
//  0    1    2    3    4    5    6    7
	0,   0,   0,   0,   0,   0,   0,   0,
   25,  20,  15,  15,  10,  10,   4,   0,
   20,  16,  12,  12,   8,   8,   3,   0,
   15,  12,   9,   9,   6,   6,   2,   0,
   10,   8,   6,   6,   4,   4,   2,   0,
	5,   4,   3,   3,   2,   2,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,   0,   0
},
piece_pos_values[2][7][64],
multi_pawn[] =
  {0, -50, -75, -90, -100, -100, -100},
*__heuristic;
#define    SCORE_CANSHORTCASTLE   50
#define    SCORE_CANLONGCASTLE    35
#define    SCORE_SHORTCASTLE      90
#define    SCORE_LONGCASTLE       70
/*
#define    SCORE_CANSHORTCASTLE   100
#define    SCORE_CANLONGCASTLE    80
#define    SCORE_SHORTCASTLE      230
#define    SCORE_LONGCASTLE       200
*/
/*
#define     TOLERANCE           8  //  Tolerance width
#define     EXCHANGEVALUE      32
     //  Value for exchanging pieces when ahead (not pawns)
#define     ISOLATEDPAWN       20
    //  Isolated pawn.  Double isolated pawn is 3 * 20
#define     DOUBLEPAWN          8   //  Double pawn
#define     SIDEPAWN            6   //  Having a pawn on the side
#define     CHAINPAWN           3   //  Being covered by a pawn
#define     COVERPAWN           3   //  covering a pawn
#define     NOTMOVEPAWN         2   //  Penalty for moving pawn
#define     BISHOPBLOCKVALUE   20
    //  Penalty for bishop blocking d2/e2 pawn
#define     ROOKBEHINDPASSPAWN 16   //  Bonus for Rook behind passed pawn

#define     SIDEMATERIAL      0x2000  // = sum of one side pieces + 1
#define     SIDEMATERIAL1     0x1fff  // = sum of one side pieces
#define     TOTALMATERIAL     0x4000  // = sum of all pieces + 2
#define     TOTALMATERIAL1    0x3fff  // = sum of all pieces + 1
*/
long distan[8] = { 3, 2, 1, 0, 0, 1, 2, 3 };
    /*  The value of a pawn is the sum of Rank and file values.
        The file value is equal to PawnFileFactor * (Rank Number + 2) */
long pawnrank[8] = {0, 0, 0, 2, 4, 8, 30, 0};
long passpawnrank[8] = {0, 0, 10, 20, 40, 60, 70, 0};
long pawnfilefactor[8] = {0, 0, 2, 5, 6, 2, 0, 0};
long filebittab[8] = {1, 2, 4, 8, 0x10, 0x20, 0x40, 0x80};
long totalmaterial, pawntotalmaterial, material;
//  Material level of the game (early middlegame = 43 - 32, endgame = 0)
long materiallevel;
long squarerankvalue[8] = { 0, 0, 0, 0, 1, 2, 4, 4};
long pv_table[2][6][64];

long __attacked(long *pos, long _s, long _t, long *_xa)
{
    long _i, _x, _p, _d, _e;
    long attacked = 0, side = pos[66];
    long oppfirst_i = __sides[1 ^ side].FirstI,
         opplast_i  = __sides[1 ^ side].LastI;
    for(_i = oppfirst_i; _i < opplast_i; _i++)
        if((_x = pos[_i]) != CAPTURED)
        if(IRAYS2(_p = PIECETYPE2(pos[_x]), _x, _s))
        {
            if(_p == PIECE_KING ||
                _p == PIECE_KNIGHT || _p == PIECE_CAPTUREPAWN(side))
            {
                if(_xa)
                    *_xa = pos[_i];
                attacked++;
            }
            else
            {
                _d = DIR(_x, _s);
                _x += (_e = __diradv[_d]);
                for(; _x != _s; _x += _e)
                    if(IS_NOTEMPTY(pos[_x]))
                        break;
                if(_x == _s)
                {
                    if(_xa)
                        *_xa = pos[_i];
                    attacked++;
                }
            }
            if(attacked >= _t)
                break;
        }
    return attacked;
}

long __makemove(long *pos, MOVE64 move)
{
    long enp, side = pos[66];
    long from = 100, to, i, j,
        last_rank = __sides[side].LastRank,
        last_rank_1 = __sides[side].OneBeforeLastRank,
        bit_side = __sides[side].BitSide;

    __makemoveCount++;
    __fifty++;
    __evaluate((POSSCORE *)pos, move);
    if(move.to > 100)
    {   // pawn promotion
        i = LINE_OF_SQUAR(move.from);
        j = RANK_OF_SQUAR(move.from);
        move.from = SQUAR(i, last_rank_1);
        pos[move.from] = bit_side | (move.to - 100);
        move.to = SQUAR(j, last_rank);
        __fifty = 0;
    }
    else if(pos[65] & 0xf0)
    {
        if(move.from == 0 || move.to == 0)
            pos[65] &= ~__sides[SIDE_WHITE].CanCastleBit[1];//0x80;
        if(move.from == 7 || move.to == 7)
            pos[65] &= ~__sides[SIDE_WHITE].CanCastleBit[0];
        if(move.from == 4)
        {
            if(move.to == 2)
                (pos[65] & __sides[SIDE_WHITE].CanCastleBit[1]) ?
                    (from = 0, to = 3, pos[65] |= __sides[SIDE_WHITE].CastledBit[1]) : 0;
            else if(move.to == 6)
                (pos[65] & __sides[SIDE_WHITE].CanCastleBit[0]) ?
                    (from = 7, to = 5, pos[65] |= __sides[SIDE_WHITE].CastledBit[0]) : 0;
            pos[65] &= 0x3f;
        }
        if(move.from == 56 || move.to == 56)
            pos[65] &= ~__sides[SIDE_BLACK].CanCastleBit[1];
        if(move.from == 63 || move.to == 63)
            pos[65] &= ~__sides[SIDE_BLACK].CanCastleBit[0];
        if(move.from == 60)
        {
            if(move.to == 58)
                (pos[65] & __sides[SIDE_BLACK].CanCastleBit[1]) ?
                    (from = 56, to = 59, pos[65] |= __sides[SIDE_BLACK].CastledBit[1]) : 0;
            else if(move.to == 62)
                (pos[65] & __sides[SIDE_BLACK].CanCastleBit[0]) ?
                    (from = 63, to = 61, pos[65] |= __sides[SIDE_BLACK].CastledBit[0]) : 0;
            pos[65] &= 0xcf;
        }
    }
    if((enp = pos[67]) != 100) // enpassant
    {
        long b = __sides[side].PawnMove;
        if(__dir[enp] & 1)
        if((pos[enp + __diradv[0] - b] & bit_side) &&
                PIECETYPE(pos[enp + __diradv[0] - b]) == PIECE_PAWN(side))
            if(move.from == enp + __diradv[0] - b && move.to == enp)
            {
                from = enp - b;
                to = CAPTURED;   // Capture this pawn
            }
        if(__dir[enp] & 0x10)
        if((pos[enp + __diradv[4] - b] & bit_side) &&
                PIECETYPE(pos[enp + __diradv[4] - b]) == PIECE_PAWN(side))
            if(move.from == enp + __diradv[4] - b && move.to == enp)
            {
                from = enp - b;
                to = CAPTURED;   // Capture this pawn
            }
    }
    if(from != 100)
        __fifty = 0;
    pos[67] = 100;
    if(ABS(move.to - move.from) == 16)
    if(PIECETYPE(pos[move.from]) == PIECE_PAWN(side))
        pos[67] = (move.from + move.to) >> 1;
    if(IS_NOTEMPTY(pos[move.to]))
        __fifty = 0;
    pos[move.to] = pos[move.from];
    pos[move.from] = 0;

    j = __sides[SIDE_BLACK].LastI;
    for(i = __sides[SIDE_WHITE].FirstI; i < j; i++)
        (pos[i] == move.from) ? pos[i] = move.to :
                (pos[i] == move.to) ? pos[i] = CAPTURED : 0;
    if(from != 100)
    {
        if(to != CAPTURED)
            pos[to] = pos[from];
        pos[from] = 0;
        for(i = __sides[SIDE_WHITE].FirstI; i < j; i++)
            (pos[i] == from) ? pos[i] = to :
                    (pos[i] == to) ? pos[i] = CAPTURED : 0;
    }
    pos[66] ^= 1;  // switch turn
    return __fifty;
}

#ifdef __LOOKTABLE
#define MAX_ARRMOVE4  600
__int64 __arrmove4[4 * MAX_ARRMOVE4];
long __move4count, __score4[MAX_ARRMOVE4];
#endif

void __prepare(long *pos, long maxdepth,
				MOVE64 *mv64, long fifty, long thinktime, HWND hwnd)
{
    memset(__thinkBuf, 0, sizeof(__thinkBuf));
    __thinkPtr = __thinkBuf;
    __maxdepth = maxdepth;
    __capdepth = 8;
    __fifty = fifty;
#ifdef XPOSLIST
    __chaincount = __fifty;
    __chain = (MOVE64 *)__thinkPtr;
    __thinkPtr += 100 * MOVE64_SIZE;
    for(long i = 0; i < __chaincount; i++)
        __chain[i] = mv64[i];
#endif
	__timelimit = thinktime;
    __hwnd = hwnd;
    __generateMovesCount = __makemoveCount = 0;
    __nodescount = 0;
#ifdef __LOOKTABLE
    __move4count = 0;
#endif
    __starttime = (long)GetTickCount();
    __bestmovesok = (MOVE64 *)__thinkPtr;
    __thinkPtr += MAX_DEPTH * MOVE64_SIZE;
    for(long i = 0; i < MAX_DEPTH; i++)
    {
        __bestmoves[i] = (MOVE64 *)__thinkPtr;
        __thinkPtr += (MAX_DEPTH - i) * MOVE64_SIZE;
    }
    for(long i = 0; i < MAX_DEPTH; i++)
    {
        __capbestmoves[i] = (MOVE64 *)__thinkPtr;
        __thinkPtr += (MAX_DEPTH - i) * MOVE64_SIZE;
    }
    __heuristic = __thinkPtr;
    __thinkPtr += 128 * 128;
    memcpy(__thinkPtr, pos, 100 * sizeof(long));
    //__compact(__thinkPtr);
    //__zerohasharr();
    __preanalyse(__thinkPtr);
    ResetEvent(hFinishEvent);
    SetEvent(hThinkEvent);
}

long __bonus[100], __timefact[] = {0, 1, 1, 1, 1,  1, 1, 4, 10, 16,  25, 35, 30};
DWORD WINAPI __think(LPVOID data)
{
    __evaluate0((POSSCORE *)__thinkPtr);

    __followpv = 0;
	memset(__bonus, 0, 100 * sizeof(long));
	long d, count = 0;
    MOVE64 mv64 = {0, 0};
	firstLevelMoves_Count = 0;
	for(d = 1; d <= __maxdepth; d++)
	{
		__depth = d;
        __virtualdepth = 0;
        __followpv = 1;
#ifdef __LOOKTABLE
        __move4count = 0;
#endif
        if(__alphabeta(0, __ALPHA, __BETA) >= __BETA - __depth)
			break;
		if(__depth >= 6)
		{
			if(mv64.move == __bestmoves[0][0].move)
			{
				if(++count >= 3)
					break;
			}
			else
			{
				mv64 = __bestmoves[0][0];
				count = 1;
			}
		}
		__lastmoveTime = (long)GetTickCount() - __starttime;
		if(__timefact[__depth] * __lastmoveTime >= __timelimit)
			break;
	}
	if(__bestmovesok[0].move == 0)
        for(long i = 0; i < MAX_DEPTH; i++)
            __bestmovesok[i] = __bestmoves[0][i];

    if(__bestmovesok[0].move == 0)
    {
        int num_moves = __generateMoves(__thinkPtr, temp_moves);

        POSSCORE *here_pos = (POSSCORE *)__thinkPtr;
        __thinkPtr += POSSCORE_SIZE;
        MOVESCORE *here_moves = (MOVESCORE *)__thinkPtr;
        __thinkPtr += num_moves * MOVESCORE_SIZE;
        int side = here_pos->pos[66];
        for(int i = 0; i < num_moves; i++)
        {
			here_moves[i].move = temp_moves[i];
            if(here_moves[i].move.to >= 100) // pawn promotion
                here_moves[i].score = 10 * (piece_values[side][here_moves[i].move.to - 100]);
            else if(here_moves[i].move.to == here_pos->pos[67])
                here_moves[i].score = piece_values[1 ^ side][PIECE_WPAWN];
            else if(IS_NOTEMPTY(here_pos->pos[here_moves[i].move.to]))
                here_moves[i].score =
				   10 * (piece_values[1 ^ side][PIECETYPE(here_pos->pos[here_moves[i].move.to])]
					   - piece_values[    side][PIECETYPE(here_pos->pos[here_moves[i].move.from])]);
			else
                here_moves[i].score =
                        __heuristic[here_moves[i].move.from * 128 + here_moves[i].move.to];
        }
        long score = 0;
        for(int i = 0, j, m; i < num_moves; i++)
        {
            score = here_moves[m = i].score;
            for(j = i + 1; j < num_moves; j++)
                if(score < here_moves[j].score)
                    score = here_moves[m = j].score;
            if(m > i)
            {
                MOVESCORE ms = here_moves[i];
                here_moves[i] = here_moves[m];
                here_moves[m] = ms;
            }
        }
        __bestmovesok[0] = here_moves[0].move;
    }
    
    SetEvent(hFinishEvent);
    PostMessage(__hwnd, WM_THINKFINISHED, 0, 0);
    ResetEvent(hThinkEvent);

    return 0;
}

long __alphabeta(long depth, long alpha, long beta)
{
	if(depth <= 1)
    if(WaitForMultipleObjects(1, &hStopEvent, TRUE, 0) == WAIT_OBJECT_0)
    {
        if(__bestmovesok[0].move == 0)
            for(long i = 0; i < MAX_DEPTH; i++)
                __bestmovesok[i] = __bestmoves[0][i];
        return __ALPHA - 100;
    }
    long side = __thinkPtr[66], score;
#ifdef XPOSLIST
    if(__countrepetition() >= 3)
    {
        __nodescount++;
        return __stalematevalue[side] - __stalematevalue[1 ^ side];
    }
#endif
#ifdef VIRTUALDEPTH
	if(__ischecked(__thinkPtr))
		if(__depth + __virtualdepth + 2 < MAX_DEPTH)
            __virtualdepth += 2;
#endif
	if(depth >= __depth + __virtualdepth)
    {
        __nodescount++;
#ifdef __LOOKTABLE
        if((score = __looktable(__thinkPtr + 68)) <= 0)
        {
#endif
#ifdef ZOBRIST_KEYS
        unsigned __int64 key64 = __boardkey(__thinkPtr);
        if((score = zb__lookup(key64, depth)) <= __ALPHA - 100)
        {
#endif
            __capfollowpv = 1;
            for(__capdepth = 2; __capdepth <= __maxcapdepth; __capdepth += 2)
                if((score = __alphabetacap(0, __ALPHA, __BETA)) > 0)
                    break;
#ifdef ZOBRIST_KEYS
            zb__insert(key64, score, depth);
        }
#endif
#ifdef __LOOKTABLE
            if(score > 0)
                __addtable(__thinkPtr + 68, score);
        }
#endif
		return score > 0 ? score : __vantage((POSSCORE *)__thinkPtr);
	}
    long i, j, m, num_plays, ff,
            num_moves = __generateMoves(__thinkPtr, temp_moves);
    if(depth == 0 && num_moves == 1)
    {
        __bestmoves[0][0] = temp_moves[0];
        return __BETA;
    }
	POSSCORE *here_pos = (POSSCORE *)__thinkPtr;
    __thinkPtr += POSSCORE_SIZE;
    MOVESCORE *here_moves = (MOVESCORE *)__thinkPtr;
    __thinkPtr += num_moves * MOVESCORE_SIZE;
    score = 0;

//--- ordering first level moves; start
	for(i = 0; i < num_moves; i++)
		here_moves[i].move = temp_moves[i];
	if(depth == 0 && firstLevelMoves_Count > 0)
	{
		//for(j = 0; j < num_moves; j++)
        //    here_moves[j].score = 0;
        for (i = 0; i < num_moves; i++) {
            for (j = 0; j < firstLevelMoves_Count; ++j)
            {
                if (here_moves[i].move.move == firstLevelMoves[j].move.move)
                {
                    here_moves[i].score = firstLevelMoves[j].score;
                    break;
                }
            }
        }
		firstLevelMoves_Count = 0;
	}
    else {
        //--- ordering first level moves; end

        for (i = 0; i < num_moves; i++)
        {
            ///---test; ordering first level moves; commented; 98-07-18; start
            //		here_moves[i].move = temp_moves[i];
            ///---test; ordering first level moves; commented; 98-07-18; end
            if (here_moves[i].move.to >= 100) // pawn promotion
                here_moves[i].score = PROMOTION_BONUS +
                10 * (piece_values[side][here_moves[i].move.to - 100]);
            else if (here_moves[i].move.to == here_pos->pos[67])
                here_moves[i].score = CAPTURE_BONUS;
            else if (IS_NOTEMPTY(here_pos->pos[here_moves[i].move.to]))
                here_moves[i].score = CAPTURE_BONUS +
                10 * (piece_values[1 ^ side][PIECETYPE(here_pos->pos[here_moves[i].move.to])]
                    - piece_values[side][PIECETYPE(here_pos->pos[here_moves[i].move.from])]);
            else
                here_moves[i].score =
                __heuristic[here_moves[i].move.from * 128 + here_moves[i].move.to];
            if (__followpv)
                if (here_moves[i].move.move == __bestmoves[0][depth].move)
                    here_moves[i].score += (score = FOLLOW_BONUS);
        }
    }
	if(score <= 0)
		__followpv = 0;
	ff = __fifty;
	for(i = 0; i < num_moves; i++) {
		score = here_moves[m = i].score;
		for(j = i + 1; j < num_moves; j++)
			if(score < here_moves[j].score)
				score = here_moves[m = j].score;
		if(m > i) {
			MOVESCORE ms = here_moves[i];
			here_moves[i] = here_moves[m];
			here_moves[m] = ms;
		}
		*(POSSCORE *)__thinkPtr = *here_pos;
		__fifty = ff;
		__chain[__chaincount++] = here_moves[i].move;
		__makemove(__thinkPtr, here_moves[i].move);
		score = -__alphabeta(depth + 1, -beta, -alpha);
///---test; ordering first level moves; added; 98-07-18; start
		if(depth == 0) {
			firstLevelMoves[firstLevelMoves_Count].move = here_moves[i].move;
			firstLevelMoves[firstLevelMoves_Count].score = score;
			//++firstLevelMoves_Count;
		}
///---test; ordering first level moves; added; 98-07-18; end
		__chaincount--;
		if(score > alpha) {
			__heuristic[here_moves[i].move.from * 128 + here_moves[i].move.to] += MAX_DEPTH - depth;
			__bestmoves[depth][0] = here_moves[i].move;
			for(j = 0; j < __depth + __virtualdepth - depth - 1; j++)
				__bestmoves[depth][j + 1] = __bestmoves[depth + 1][j];
			alpha = score;
		}
		if(score >= beta) {
///---test; ordering first level moves; added; 98-07-18; start
			if(depth == 0)
			for(i++; i < num_moves; i++)
			{
				firstLevelMoves[firstLevelMoves_Count].move = here_moves[i].move;
				firstLevelMoves[firstLevelMoves_Count].score = alpha;
				//++firstLevelMoves_Count;
			}
///---test; ordering first level moves; added; 98-07-18; end
			break;
		}
    }
    __thinkPtr = here_pos->pos;
    __fifty = ff;
    if(num_moves == 0)
    {
		if(__ischecked(here_pos->pos))  // checkmate
			return __ALPHA - 10000 + depth;
		else // stalemate
            return __vantage(here_pos) > 0 ? __ALPHA + depth : __BETA - depth + 1000;
    }

    return i >= num_moves ? alpha : beta;
}

long __alphabetacap(long depth, long alpha, long beta)
{
    long side = __thinkPtr[66];
    POSSCORE *here_pos = (POSSCORE *)__thinkPtr;
    __thinkPtr += POSSCORE_SIZE;
    if(depth >= __capdepth)
    {
        __nodescount++;
        return __vantage(here_pos);
    }
    long i, j, m, num_plays, ff,
            num_moves = __generateMoves(here_pos->pos, temp_moves);
	MOVESCORE *here_moves = (MOVESCORE *)__thinkPtr;
    __thinkPtr += num_moves * MOVESCORE_SIZE;
    long score = 0;
    for(i = 0; i < num_moves; i++)
    {
        here_moves[i].move = temp_moves[i];
        if(here_moves[i].move.to >= 100) // pawn promotion
            here_moves[i].score = PROMOTION_BONUS +
                    10 * (piece_values[side][here_moves[i].move.to - 100]);
        else if(here_moves[i].move.to == here_pos->pos[67])
            here_moves[i].score = CAPTURE_BONUS;
        else if(IS_NOTEMPTY(here_pos->pos[here_moves[i].move.to]))
            here_moves[i].score = CAPTURE_BONUS +
			   10 * (piece_values[1 ^ side][PIECETYPE(here_pos->pos[here_moves[i].move.to])]
				   - piece_values[    side][PIECETYPE(here_pos->pos[here_moves[i].move.from])]);
		else
            here_moves[i].score = __ALPHA - 100;
#ifdef FOLLOW_PV
        if(__capfollowpv)
        if(here_moves[i].move.move == __capbestmoves[0][depth].move)
            here_moves[i].score += (score = FOLLOW_BONUS);
#endif
    }
#ifdef FOLLOW_PV
    if(score <= 0)
        __capfollowpv = 0;
#endif
    for(i = 0; i < num_moves; i++)
    {
        score = here_moves[m = i].score;
        for(j = i + 1; j < num_moves; j++)
            if(score < here_moves[j].score)
                score = here_moves[m = j].score;
        if(m > i)
        {
            MOVESCORE ms = here_moves[i];
            here_moves[i] = here_moves[m];
            here_moves[m] = ms;
        }
        if(here_moves[i].score <= __ALPHA - 100)
            break;
        *(POSSCORE *)__thinkPtr = *here_pos;
		__makemove(__thinkPtr, here_moves[i].move);
		score = -__alphabetacap(depth + 1, -beta, -alpha);
		if(score > alpha)
        {
            __capbestmoves[depth][0] = here_moves[i].move;
            for(j = 0; j < __capdepth - depth - 1; j++)
                __capbestmoves[depth][j + 1] = __capbestmoves[depth + 1][j];
            alpha = score;
        }
        if(score >= beta)
            break;
    }
    __thinkPtr = here_pos->pos;
    if(num_moves == 0)
    {
        if(__ischecked(here_pos->pos))  // checkmate
			return __ALPHA - 10000 + depth;
		else // stalemate
            return __vantage(here_pos) > 0
                        ? __ALPHA + depth : __BETA - depth + 1000;
    }
    else if(here_moves[i].score <= __ALPHA - 100)
        return i > 0 ? alpha : __vantage(here_pos);
    return beta;
}

long __evaluate0(POSSCORE *possco)
{
    long i, side, x, *pos = possco->pos;

    SCORE *sc = possco->sco;
    for(side = SIDE_WHITE; side <= SIDE_BLACK; side++)
    {
        memset(&sc[side], 0, sizeof(SCORE));
        for(i = __sides[side].FirstI; i < __sides[side].LastI; i++)
            if((x = pos[i]) != CAPTURED) // The piece is not captured yet
            {
                sc[side].Material += piece_values[side][PIECETYPE(pos[x])];
#ifdef PAWNSTRUCTURE_VALUES
                if(PIECETYPE(pos[x]) == PIECE_PAWN(side))
                {
                    sc[side].PawnCount++;
                    sc[side].xbitPawns[LINE_OF_SQUAR(x)] |= 1 << RANK_OF_SQUAR(x);
                    sc[side].ybitPawns[RANK_OF_SQUAR(x)] |= 1 << LINE_OF_SQUAR(x);
                    sc[side].bitPawns |= 1 << LINE_OF_SQUAR(x);
                }
                else
                    sc[side].PieceCount++;
#endif
#ifdef POSITION_VALUES
                sc[side].PosValues +=
                    piece_pos_values[side][PIECETYPE(pos[x])][x];
#endif
            }
        // castle values
        if(pos[65] & __sides[side].CanCastleBit[0])
            sc[side].CastleValue += SCORE_CANSHORTCASTLE;
        if(pos[65] & __sides[side].CanCastleBit[1])
            sc[side].CastleValue += SCORE_CANLONGCASTLE;
        if(pos[65] & __sides[side].CastledBit[0])
            sc[side].CastleValue += SCORE_SHORTCASTLE;
        if(pos[65] & __sides[side].CastledBit[1])
            sc[side].CastleValue += SCORE_LONGCASTLE;
        // Add here King safety
    }
    return 0;
}

long __evaluate(POSSCORE *possco, MOVE64 move)
{
    long *pos = possco->pos;
    long side = pos[66], oppside,
        castle = pos[65], from = move.from, mat[2],
            to = move.to, pro, to1, from1, fromx, fromy, tox, toy;

    oppside = 1 ^ side;
    SCORE *score = possco->sco;
    mat[0] = score[0].Material;
    mat[1] = score[1].Material;

    fromx = LINE_OF_SQUAR(from);
    fromy = RANK_OF_SQUAR(from);
    tox   = LINE_OF_SQUAR(to);
    toy   = RANK_OF_SQUAR(to);

    if(to >= 100) // pawn promotion
    {
        score[side].Material +=
            piece_values[side][pro = to - 100] - piece_values[side][PIECE_PAWN(side)];
        to = SQUAR(tox = fromy, toy = __sides[side].LastRank);
        from = SQUAR(fromx, fromy = __sides[side].OneBeforeLastRank);
        score[side].PieceCount++;
#ifdef POSITION_VALUES
        score[side].PosValues +=
            piece_pos_values[side][pro][to] -
            piece_pos_values[side][PIECETYPE(pos[from])][from];
#endif
#ifdef PAWNSTRUCTURE_VALUES
        score[side].xbitPawns[fromx] ^= 1 << fromy;
        score[side].ybitPawns[fromy] ^= 1 << fromx;
        if(score[side].xbitPawns[fromx] == 0)
            score[side].bitPawns ^= 1 << fromx;
        score[side].PawnCount--;
#endif
    }
    else
    {
#ifdef POSITION_VALUES
        score[side].PosValues +=
            piece_pos_values[side][PIECETYPE(pos[from])][to] -
            piece_pos_values[side][PIECETYPE(pos[from])][from];
#endif
#ifdef PAWNSTRUCTURE_VALUES
        if(PIECETYPE(pos[from]) == PIECE_PAWN(side))
        {
            score[side].xbitPawns[fromx] ^= 1 << fromy;
            score[side].ybitPawns[fromy] ^= 1 << fromx;
            score[side].xbitPawns[tox  ] |= 1 << toy;
            score[side].ybitPawns[toy  ] |= 1 << tox;
            if(score[side].xbitPawns[fromx] == 0)
                score[side].bitPawns ^= 1 << fromx;
            score[side].bitPawns |= 1 << tox;
        }
#endif
        // castle moves
        to1 = from1 = 100;
        if(castle & __sides[side].CanCastleBit[0])   // short castle move
            if(from == __sides[side].KingInitPos && to == from + 2)
            {
                from1 = move.from + 3;
                to1 = from1 - 2;
                score[side].CastleValue += SCORE_SHORTCASTLE;
            }
        if(castle & __sides[side].CanCastleBit[1])   // long castle move
            if(from == __sides[side].KingInitPos && to == from - 2)
            {
                from1 = from - 4;
                to1 = from1 + 3;
                score[side].CastleValue += SCORE_LONGCASTLE;
            }
        if(from == __sides[side].KingInitPos)
        {
            if(castle & __sides[side].CanCastleBit[0])   // short castle move
                score[side].CastleValue -= SCORE_CANSHORTCASTLE;
            if(castle & __sides[side].CanCastleBit[1])   // long castle move
                score[side].CastleValue -= SCORE_CANLONGCASTLE;
        }

        if(from == __sides[side].KingInitPos + 3)
            if(castle & __sides[side].CanCastleBit[0])   // short castle move
                score[side].CastleValue -= SCORE_CANSHORTCASTLE;
        if(to == __sides[oppside].KingInitPos + 3)
            if(castle & __sides[oppside].CanCastleBit[0])   // short castle move
                score[oppside].CastleValue -= SCORE_CANSHORTCASTLE;
        if(from == __sides[side].KingInitPos - 4)
            if(castle & __sides[side].CanCastleBit[1])   // long castle move
                score[side].CastleValue -= SCORE_CANLONGCASTLE;
        if(to == __sides[oppside].KingInitPos - 4)
            if(castle & __sides[oppside].CanCastleBit[1])   // long castle move
                score[oppside].CastleValue -= SCORE_CANLONGCASTLE;
#ifdef POSITION_VALUES
        if(from1 != 100)
        {
            score[side].PosValues +=
                    piece_pos_values[side][PIECETYPE(pos[from1])][to1] -
                    piece_pos_values[side][PIECETYPE(pos[from1])][from1];
        }
#endif
    }
    if(!IS_NOTEMPTY(pos[to]))
    {
        if(to == pos[67])  // enpassant capture
        if(PIECETYPE(pos[from]) == PIECE_PAWN(side))
        {
            to += __sides[oppside].Dir * 8;
            toy += __sides[oppside].Dir;
#ifdef PAWNSTRUCTURE_VALUES
            score[oppside].xbitPawns[tox] ^= 1 << toy;
            score[oppside].ybitPawns[toy] ^= 1 << tox;
            if(score[oppside].xbitPawns[tox] == 0)
                score[oppside].bitPawns ^= 1 << tox;
            score[oppside].PawnCount--;
#endif
            score[oppside].Material -= piece_values[oppside][PIECE_PAWN(oppside)];
#ifdef POSITION_VALUES
            score[oppside].PosValues -=
                piece_pos_values[oppside][PIECE_PAWN(oppside)][to];
#endif
        }
    }
    else
    {
        score[oppside].Material -= piece_values[oppside][PIECETYPE(pos[to])];
#ifdef POSITION_VALUES
        score[oppside].PosValues -=
            piece_pos_values[oppside][PIECETYPE(pos[to])][to];
#endif
#ifdef PAWNSTRUCTURE_VALUES
        if(PIECETYPE(pos[to]) == PIECE_PAWN(oppside))
        {
            score[oppside].xbitPawns[tox] ^= 1 << toy;
            score[oppside].ybitPawns[toy] ^= 1 << tox;
            if(score[oppside].xbitPawns[tox] == 0)
                score[oppside].bitPawns ^= 1 << tox;
            score[oppside].PawnCount--;
        }
        else
            score[oppside].PieceCount--;
#endif
    }
    return 0;
}

long attackval[2][64],
    v_material,
    v_pawnstructure,
    v_castle,
    v_posvalues,
    v_attack,
    v_kingsafety;

long bits, bits1, bitside;

long __vantage(POSSCORE *ps)
{
    long value[2], n, i, capmat, totmat, king_x, king_sq, *pos, *ptr, bit_side;

    long eva_side = ps->pos[66];
    SCORE *sco = ps->sco;
    pos = ps->pos;

    //mat[0] = sco[0].Material;
    //mat[1] = sco[1].Material;
    for(long side = SIDE_WHITE; side <= SIDE_BLACK; side++)
    {
        v_material = 0;
        v_pawnstructure = 0;
        v_castle = 0;
        v_posvalues = 0;
        v_attack = 0;
        v_kingsafety = 0;

        king_x = LINE_OF_SQUAR(king_sq = ps->pos[__sides[side].FirstI]);
        //king_y = RANK_OF_SQUAR(ps->pos[__sides[side].FirstI]);
        bitside = __sides[side].BitSide;
        v_material += sco[side].Material;
        v_castle += sco[side].CastleValue;
#ifdef POSITION_VALUES
        v_posvalues += sco[side].PosValues;
#endif
#ifdef PAWNSTRUCTURE_VALUES
        for(i = 0; i < 8; i++)
        {
            if(__bitcount[sco[side].xbitPawns[i]] > 1)
                v_pawnstructure += multi_pawn[__bitcount[sco[side].xbitPawns[i]] - 1];
            //v_pawnstructure += __bitcount[sco[side].ybitPawns[i]] *
            //    (side == SIDE_WHITE ? pawn_on_rank[i] : pawn_on_rank[7 - i]);
        }
        bits = sco[side].bitPawns;
        //v_pawnstructure += __chainval[bits];
        v_pawnstructure -= __bitcount[bits & ((bits << 1) | (bits >> 1))] << 4;
        bits = sco[side].ybitPawns[1];
        for(i = 2; i < 8; i++)
        {
            bits1 = bits;
            bits = sco[side].ybitPawns[i];
            v_pawnstructure += __bitcount[((bits  << 1) | (bits  >> 1)) & bits ] << 2;
            v_pawnstructure += __bitcount[((bits  << 1) | (bits  >> 1)) & bits1] << 1;
            v_pawnstructure += __bitcount[((bits1 << 1) | (bits1 >> 1)) & bits ] << 1;
        }
#endif
        if(king_x < 7)
            if(sco[side].xbitPawns[king_x + 1] == 0)
                v_pawnstructure -= 30;
        if(sco[side].xbitPawns[king_x] == 0)
            v_pawnstructure -= 50;
        if(king_x > 0)
            if(sco[side].xbitPawns[king_x - 1] == 0)
                v_pawnstructure -= 30;

        //v_kingsafety;
        ptr = RAYS2(PIECE_KING, king_sq);
        n = *ptr++;
        v_kingsafety += n * 5;
        for(i = 0; i < n; i++)
        {
            if(pos[*ptr] & bit_side)
                v_kingsafety += 10;
            if(__attacked(pos, king_sq, 1, NULL))
                v_kingsafety -= 15;
            ptr++;
        }
        value[side] = v_material + v_castle +
                        v_posvalues + v_pawnstructure + v_attack + v_kingsafety;
	}

#ifdef NEW_EVALUATION
	double ratio = 1.0;
	if(value[eva_side] > value[1 ^ eva_side]) {
		ratio = (double)value[eva_side] / value[1 ^ eva_side];
	}
	else if(value[1 ^ eva_side] > value[eva_side]) {
		ratio = (double)value[1 ^ eva_side] / value[eva_side];
	}
	long diff = (value[eva_side] - value[1 ^ eva_side]);

	return diff * ratio;
#else
    return value[eva_side] - value[1 ^ eva_side];
#endif
}

long __ischecked(long *pos)
{
    long side = pos[66];
    long king_sq = pos[__sides[side].FirstI];
    return __attacked(pos, king_sq, 1, NULL);
}

long __preanalyse(long *pos)
{
    long material[2], pwndst[2][8],
            king_sq[2], totalcount[2], pieces[2][6];
    long side, i, j, x, y, s, p, m, n, v;
    for(side = SIDE_WHITE; side <= SIDE_BLACK; side++)
    {
        king_sq[side] = pos[__sides[side].FirstI];
        material[side] = 0;
        totalcount[side] = 0;
        for(i = 0; i < 6; i++)
            pieces[side][i] = 0;
        for(i = __sides[side].FirstI; i < __sides[side].LastI; i++)
        if((x = pos[i]) != CAPTURED)
        {
            p = PIECETYPE(pos[x]);
            if(p == PIECE_BPAWN)
                p = PIECE_WPAWN;
            pieces[side][p]++;
            material[side] += base_piece_values[p];
            totalcount[side]++;
        }
        material[side] -= base_piece_values[PIECE_KING];
    }

    __maxcapdepth = (totalcount[SIDE_WHITE] + totalcount[SIDE_BLACK] - 2) / 2 * 2;

    // piece_values calculation
    for(side = SIDE_WHITE; side <= SIDE_BLACK; side++)
    {
        memcpy(piece_values[side],
            base_piece_values, sizeof(base_piece_values));
        double fac, r1 = ((double)1.0 + material[1 ^ side]) / (1 + material[side]);
        if(pieces[side][PIECE_WPAWN] > 0)
        {
            fac = 1.0;
            if(totalcount[side] - pieces[side][PIECE_WPAWN] <= 2 &&
                (pieces[side][PIECE_BISHOP] + pieces[side][PIECE_KNIGHT] <= 1))
                fac = 2.0;
            if(totalcount[side] - pieces[side][PIECE_WPAWN] == 3 && pieces[side][PIECE_KNIGHT] == 2)
                fac = 2.0;
            piece_values[side][PIECE_WPAWN  ] =
            piece_values[side][PIECE_BPAWN  ] =
            piece_values[side][PIECE_WCAPAWN] =
            piece_values[side][PIECE_BCAPAWN] =
                base_piece_values[PIECE_WPAWN] * fac;
        }
        fac = 1.0;
        fac = (10.0 + 1.2 * r1 * r1) / (10.0 + 1.2 * r1);
        for(i = PIECE_QUEEN; i <= PIECE_BISHOP; i++)
            piece_values[side][i] *= fac;
    }

    __checkstalemate(pos, __stalematevalue);
    for(i = 0; i < 15; i++)
    for(j = 0; j < 15; j++)
    {
		kpv_surface[i][j] = 10 - 7 * MAX(ABS(7 - i), ABS(7 - j));
        if(kpv_surface[i][j] < 0)
            kpv_surface[i][j] = 0;
    }

    long x_arr[100], *ptr, *base, adv, mat_level, coef;
    mat_level =
        (material[SIDE_WHITE] + material[SIDE_BLACK]) / 1000;
    //mat_level <= 7[= (3900 + 3900) / 1000] and mat_level >= 0

    // __depth calculation
    long max_depth = __depth - (__depth <= 1000 ? 0 : 1000);
    /*
    long min_dist = 20;
    for(side = 0; side < 2; side++)
    for(i = 0; i < 8; i++)
        if(pwndst[side][i] != -1)
        if(min_dist > pwndst[side][i])
            min_dist = pwndst[side][i];
    if(min_dist < 20 && mat_level < 6)
    {
        if(max_depth < min_dist * 2 + 1)
            max_depth = min_dist * 2 + 1;
    }
    */
    if(__depth > 1000)
        __depth = max_depth;

    double f_arr[100];
    for(side = SIDE_WHITE; side <= SIDE_BLACK; side++)
    {
        for(x = 0; x < 64; x++)
            attack_val[(side == SIDE_WHITE) ? flip[x] : x] =
                    center_attack_val[x] + mat_level * forward_attack_val[x];
        memcpy(piece_pos_values[side],
            base_piece_pos_values, sizeof(piece_pos_values[side]));
        if(side == SIDE_WHITE)
        for(x = 0; x < 32; x++)
            for(p = PIECE_KING; p <= PIECE_BPAWN; p++)
            {
                long temp = piece_pos_values[side][p][x];
                piece_pos_values[side][p][x] =
                    piece_pos_values[side][p][flip[x]];
                piece_pos_values[side][p][flip[x]] = temp;
            }
        ptr = RAYS2(PIECE_KING, king_sq[1 ^ side]);
        n = *ptr++;
        for(i = 0; i < n; i++)
            attack_val[*ptr++] += mat_level * 5;
        n = 0;
        if(__stalematevalue[side] >= __BETA / 4)
        {
            if(__stalematevalue[1 ^ side] <= __ALPHA / 4)
            {
                for(i = 3; i <= 4; i++)
                for(j = 3; j <= 4; j++)
                {
                    f_arr[n]   = 70.0;
                    x_arr[n++] = SQUAR(i, j);
                }
                for(i = 0; i < 8; i++)
                {
                    f_arr[n]   = -100.0;
                    x_arr[n++] = SQUAR(i, 7);
                    f_arr[n]   = -100.0;
                    x_arr[n++] = SQUAR(i, 0);
                    f_arr[n]   = -100.0;
                    x_arr[n++] = SQUAR(0, i);
                    f_arr[n]   = -100.0;
                    x_arr[n++] = SQUAR(7, i);
                }
                f_arr[n]   = -50.0;
                x_arr[n++] = king_sq[1 - side];
            }
        }
        if(__stalematevalue[side] <= __ALPHA / 4)
        {
            if(__stalematevalue[1 ^ side] >= __BETA / 4)
            {
                for(i = 0; i < 8; i++)
                for(j = 0; j < 8; j++)
                {
					x = MAX(ABS(i - king_sq[1 ^ side] % 8), ABS(j - king_sq[1 ^ side] / 8));
					f_arr[n]   = 80 - 8 * x;
                    x_arr[n++] = SQUAR(i, j);
                }
            }
        }
        if(n > 0)
        {
            for(x = 0; x < 64; x++)
            {
                double dbl = 0;
                    for(i = 0; i < n; i++)
                    dbl += f_arr[i] *
                        kpv_surface[7 + x / 8 - x_arr[i] / 8][7 + x % 8 - x_arr[i] % 8];
                    piece_pos_values[side][PIECE_KING][x] = dbl / n;
            }
        }
        if(__stalematevalue[side] >= __BETA / 4)
        {
            if(totalcount[1 ^ side] == 3 &&
                pieces[1 ^ side][PIECE_BISHOP] == 1 && pieces[1 ^ side][PIECE_KNIGHT] == 1)
            {
                for(x = 0; x < 64; x++)
                    if(PIECETYPE(pos[x]) == PIECE_BISHOP)
                    if((pos[x] & 0xc0) == __sides[1 ^ side].BitSide)
                        break;
                if(LINE_OF_SQUAR(king_sq[side]) == 0 ||
                   LINE_OF_SQUAR(king_sq[side]) == 7 ||
                   RANK_OF_SQUAR(king_sq[side]) == 0 ||
                   RANK_OF_SQUAR(king_sq[side]) == 7 )
                {
                    if((LINE_OF_SQUAR(x) + RANK_OF_SQUAR(x)) % 2 == 1)
                        x = (__distance(king_sq[side], 0) <=
                                __distance(king_sq[side], 63)) ? 0 : 63;
                    else
                        x = (__distance(king_sq[side], 0) <=
                                __distance(king_sq[side], 56)) ? 7 : 56;
                    for(i = 0; i < 64; i++)
                        piece_pos_values[side][PIECE_KING][i] = 100;
                    if(x != king_sq[side])
                    {
                        p = __diradv[DIR(x, king_sq[side])];
                        for(i = 0; i < 8; i++)
                            piece_pos_values[side][PIECE_KING][x + i * p] =
                                25 * (__distance(king_sq[side], x) - __distance(x + i * p, x));
                    }
                    else
                    {
                        for(i = 0; i < 8; i += 2)
                        if(__dir[x] & (1 << i))
                        {
                            for(j = 0; j < 8; j++)
                                piece_pos_values[side]
                                        [PIECE_KING][x + j * __diradv[i]] = -25 * j;
                        }
                    }
                    //__depth = 9;
                }
            }
        }
        for(x = 0; x < 64; x++)
        {
            ptr = RAYS2(PIECE_KNIGHT, x);
            n = *ptr++;
            v = 0;
            for(i = 0; i < n; i++)
            {
                v += attack_val[*ptr];
                ptr++;
            }
            piece_pos_values[side][PIECE_KNIGHT][x] += (v >> 1);
            for(p = PIECE_ROOK; p <= PIECE_BISHOP; p++)
            {
                v = 0;
                ptr = base = RAYS2(p, x);
                m = *ptr++;
                for(j = 0; j < m; j++)
                {
                    adv = *ptr++;
                    n = *ptr++;
                    coef = 1;
                    for(i = 0; i < n; i++)
                    {
                        v += attack_val[*ptr] << coef;
                        if(IS_NOTEMPTY(pos[*ptr]))
                        if(PIECETYPE(pos[*ptr]) != p &&
                           PIECETYPE(pos[*ptr]) != PIECE_QUEEN)
                            coef = 0;
                        if(PIECETYPE(pos[*ptr]) == PIECE_WPAWN ||
                           PIECETYPE(pos[*ptr]) == PIECE_BPAWN )
                        {
                            ptr = base + adv;
                            break;
                        }
                        ptr++;
                    }
                }
                piece_pos_values[side][p][x] += v >> 3;
            }
            piece_pos_values[side][PIECE_QUEEN][x] =
                (piece_pos_values[side][PIECE_ROOK][x] +
                    piece_pos_values[side][PIECE_BISHOP][x]) >> 2;
        }
        double line_fact[] = {1.0, 1.0, 1.1, 1.2,   1.2, 1.1, 1.0, 1.0},
                    iso_fact[] = {0, 0.3, 0.3, 1.0};
        for(x = 0; x < 8; x++)
        for(y = 0; y < 8; y++)
        {
            s = SQUAR(x, side == SIDE_BLACK ? y : 7 - y);
            if(IS_NOTEMPTY(pos[s]))
                if(PIECETYPE(pos[s]) == PIECE_PAWN(side))
                {
                    long yy, flag = 0;
                    for(yy = 0; yy < y; yy++)
                        piece_pos_values[side][PIECE_PAWN(side)]
                                [SQUAR(x, side == SIDE_BLACK ? yy : 7 - yy)] +=
                            line_fact[x] * pawn_prom_value[mat_level + 8 * yy];
                    if(x > 0)
                    {
                        for(yy = 0; yy < y; yy++)
                        {
                            s = SQUAR(x - 1, side == SIDE_BLACK ? yy : 7 - yy);
                            if(PIECETYPE(pos[s]) == PIECE_PAWN(1 ^ side))
                                break;
                        }
                        if(yy >= y)
                            flag += 1;
                    }
                    if(x < 7)
                    {
                        for(yy = 0; yy < y; yy++)
                        {
                            s = SQUAR(x + 1, side == SIDE_BLACK ? yy : 7 - yy);
                            if(PIECETYPE(pos[s]) == PIECE_PAWN(1 ^ side))
                                break;
                        }
                        if(yy >= y)
                            flag += 2;
                    }
                    for(yy = 0; yy < y; yy++)
                        piece_pos_values[side][PIECE_PAWN(side)]
                                [SQUAR(x, side == SIDE_BLACK ? yy : 7 - yy)] +=
                            line_fact[x] * pawn_prom_value[mat_level + 8 * yy] * iso_fact[flag];
                }
                else
                    break;
        }
        /*
        for(x = 0; x < 8; x++)
        if(pwndst[side][x] != -1)
        for(long yy = pwndst[side][x] + __sides[side].PawnMove;
                yy >= 0 && yy < 64; yy += __sides[side].PawnMove)
        {
            piece_pos_values[side][PIECE_PAWN(side)][yy] +=
                      //  [RANK_OF_SQUAR(yy)][mat_level] * line_fact[x];
                (8 - ABS(__sides[side].LastRank - RANK_OF_SQUAR(yy))) * (2 - mat_level / 2);//20;
            if(x > 0)
            if(pwndst[1 ^ side][x - 1] != -1)
            if(__sides[side].Dir *
                    (pwndst[side][x] - 1 - pwndst[1 ^ side][x - 1]) >= 0)
                piece_pos_values[side][PIECE_PAWN(side)][yy] += 8 - 2 * mat_level; // 6;
            if(x < 7)
            if(pwndst[1 ^ side][x + 1] != -1)
            if(__sides[side].Dir *
                    (pwndst[side][x] + 1 - pwndst[1 ^ side][x + 1]) >= 0)
                piece_pos_values[side][PIECE_PAWN(side)][yy] += 8 - 2 * mat_level; // 6;
        }
        */
#ifndef POSITION_VALUES
        memset(piece_pos_values[side],
                    0, sizeof(piece_pos_values[side]));
#endif
    }

    return 0;
}

long __getbestmove_from()
{
    return __bestmovesok ? __bestmovesok->from : 0;
}

long __getbestmove_to()
{
    return __bestmovesok ? __bestmovesok->to : 0;
}

bool __bestmovefound() { return __bestmovesok != NULL; }
MOVE64 __getbestmove() { return __bestmovesok[0]; }

long __compact(long *pos)
{
    __sides[SIDE_WHITE].FirstI = 68;
    long i, j, s;
    for(s = 0; s < 2; s++)
    {
        for(i = 0, j = __sides[s].FirstI + 1; i < 64; i++)
            if(pos[i] == (__sides[s].BitSide | PIECE_KING))
                pos[__sides[s].FirstI] = i;
            else if(pos[i] & __sides[s].BitSide)
                pos[j++] = i;
        __sides[s].LastI = j;
        if(s == SIDE_WHITE)
            __sides[SIDE_BLACK].FirstI = j;
    }
    //__sides[SIDE_BLACK].LastI = j;
    for(; j < 100; j++)
        pos[j] = CAPTURED;
    return 0;
}

long __checkstalemate(long *pos, long *stalemate)
{
    long total, pieces[NUM_PIECES], value[2], side, x, i, p;
    for(side = 0; side < 2; side++)
    {
        total = 0;
        memset(pieces, 0, sizeof(pieces));
        value[side] = 0;
        stalemate[side] = __ALPHA / 4;
        for(i = __sides[side].FirstI; i < __sides[side].LastI; i++)
        if((x = pos[i]) != CAPTURED)
        {
            p = PIECETYPE(pos[x]);
            if(p == PIECE_BPAWN)
                p = PIECE_WPAWN;
            total++;
            pieces[p]++;
            value[side] += base_piece_values[p];
        }
        if(total == 1)
            stalemate[side] = __BETA / 4;
        if(total == 2 &&
            (pieces[PIECE_BISHOP] + pieces[PIECE_KNIGHT] == 1))
            stalemate[side] = __BETA / 4;
        if(total == 3 && pieces[PIECE_KNIGHT] == 2)
            stalemate[side] = __BETA / 4;
    }
    for(side = 0; side < 2; side++)
        if(stalemate[side] < __BETA / 4)
            stalemate[side] -= value[side] - value[1 ^ side];
    return 0;
}

long __distance(long x, long y)
{
    return
        ABS(LINE_OF_SQUAR(x) - LINE_OF_SQUAR(y)) +
        ABS(RANK_OF_SQUAR(x) - RANK_OF_SQUAR(y));
}

long __countrepetition()
{
    long a[64], b = 0, c = 1;
    memset(a, 0, 64 * sizeof(long));
    for(long i = __chaincount - 1; i >= __chaincount - __fifty; i--)
    {
        (++a[__chain[i].from] == 0) ? --b : ++b;
        (--a[__chain[i].to  ] == 0) ? --b : ++b;
        b == 0 ? ++c : 0;
    }
    return c;
}

#ifdef __LOOKTABLE
long __looktable(long *pos)
{
/*
    __int64 *pt64 = (__int64 *)pos, *pt_table64 = __arrmove4;
    for(long i = 0; i < __move4count; i++)
    {
        if(*pt64 == *pt_table64)
        {
            if(pt64[1] == pt_table64[1])
            if(pt64[2] == pt_table64[2])
            if(pt64[3] == pt_table64[3])
                return __score4[i];
        }
        pt_table64 += 4;
    }
    return -1L;
*/
    __int64 a64[4], *pt_table64 = __arrmove4;
    unsigned char *ptchar = (unsigned char *)a64;
    for(long i = 0; i < 32; i++)
        *ptchar++ = (unsigned char)*pos++;

    for(long i = 0, j; i < __move4count; i++)
    {
        for(j = 0; j < 4; j++)
            if(a64[j] != *pt_table64++)
                break;
        if(j >= 4) // found in table
            return __score4[i];
    }
    return -1L;

}

long __addtable(long *pos, long score)
{
    if(__move4count < MAX_ARRMOVE4)
    {
        unsigned char *pt = (unsigned char *)&__arrmove4[__move4count << 2];
        for(long i = 0; i < 32; i++)
            *pt++ = (unsigned char)*pos++;
        __score4[__move4count++] = score;
    }
    return 0;
}
#endif

#define MAX_HASHS   500000
unsigned __int64 zb__keys[2][7][64], zb__epkeys[2][8],
        zb__noepkey, zb__castlekeys[16], zb__turnkeys[2];
typedef struct
{
    unsigned __int64 key;
    long score, depth;
}
HASH;
HASH __hasharr[MAX_HASHS];

void zb__insert(unsigned __int64 key, long score, long depth)
{
    __hasharr[key % MAX_HASHS].key = key;
    __hasharr[key % MAX_HASHS].score = score;
    __hasharr[key % MAX_HASHS].depth = depth;
}

long zb__lookup(unsigned __int64 key, long depth)
{
    HASH hash = __hasharr[key % MAX_HASHS];
    if(hash.key == key)
        if(hash.depth >= depth)
            return hash.score;
    return __ALPHA - 100;
}

unsigned __int64 __boardkey(long *pos)
{
    unsigned __int64 key = 0;

    for(long i = 0; i < 64; i++)
        if(IS_NOTEMPTY(pos[i]))
            key ^= zb__keys[pos[i] >> 7][PIECETYPE(pos[i])][i];
    key ^= (pos[67] >= 100 ? zb__noepkey
            : zb__epkeys[pos[66]][LINE_OF_SQUAR(pos[67])]);
    key ^= zb__castlekeys[pos[65] >> 4];
    key ^= zb__turnkeys[pos[66]];

    return key;
}

void zb__initkeys()
{
    unsigned __int64 rand64();
    srand(12345);
    long i, j, k;
    for(i = 0; i < 2; i++)
        for(j = 0; j <= PIECE_BPAWN; j++)
            for(k = 0; k < 64; k++)
                zb__keys[i][j][k] = rand64();
    for(i = 0; i < 2; i++)
        for(j = 0; j < 8; j++)
            zb__epkeys[i][j] = rand64();
    zb__noepkey = rand64();
    for(i = 0; i < 16; i++)
        zb__castlekeys[i] = rand64();
    zb__turnkeys[SIDE_WHITE] = rand64();
    zb__turnkeys[SIDE_BLACK] = rand64();
    __zerohasharr();
}

void __zerohasharr()
{
    memset(__hasharr, 0, MAX_HASHS * sizeof(HASH));
}

unsigned __int64 rand64()
{
	unsigned __int64 temp = rand();
	temp ^= ((unsigned __int64)rand() << 10);
	temp ^= ((unsigned __int64)rand() << 20);
	temp ^= ((unsigned __int64)rand() << 30);
	temp ^= ((unsigned __int64)rand() << 40);
	temp ^= ((unsigned __int64)rand() << 50);
	temp ^= ((unsigned __int64)rand() << 60);
	return temp;
}

