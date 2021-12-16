/*
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif
*/
#ifndef Evaluator_H
#define Evaluator_H

#include "board.h"
#include "utility.h"
#include "piece.h"
#include <unordered_map>

class Evaluator
{
public:
	Evaluator();
	static int evaluate(Board* b);
	static bool compare(Board* pos, Move* m_1, Move* m_2, std::vector<Move*>* killer_moves_at_depth);
	static int mirror_vertical(int i);
	static int score_quiet_move(Board* pos, Move *m, std::vector<Move*>* killer_moves_at_depth);
	static int score_move(Board* pos, Move* m, std::vector<Move*>* killer_moves_at_depth);
	static int score_capture(Board* pos, Move* m);
	~Evaluator();
private:
	static int PawnTable[64];
	static int KnightTable[64];
	static int BishopTable[64];
	static int RookTable[64];
	static int QueenTable[64];
	static int KingTable[64];
	static int KingTableEndGame[64];

};

#endif