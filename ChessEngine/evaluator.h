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

#include "piece.h"
#include <unordered_map>

class Evaluator
{
public:
	Evaluator();
	uint64_t random_64_bit_num();
	double evaluate(Board* b);
	uint64_t zobrist_hash(Board* b);
	bool contains_z_hash(uint64_t hash);
	void add_hash(uint64_t hash, double eval);
	static bool compare(Board* pos, Move* m_1, Move* m_2);
	std::unordered_map<uint64_t, double>* zobrist_hashmap = new std::unordered_map<uint64_t, double>;
	std::unordered_map<uint64_t, double>* prev_zobrist_hashmap = new std::unordered_map<uint64_t, double>;
	static int mirror_vertical(int i);
	static double score_move(Board* pos, Move *m);
	~Evaluator();
private:
	static short PawnTable[64];
	static short KnightTable[64];
	static short BishopTable[64];
	static short RookTable[64];
	static short QueenTable[64];
	static short KingTable[64];
	static short KingTableEndGame[64];

	uint64_t* zobrist_base_numbers = new uint64_t[781];
};

#endif