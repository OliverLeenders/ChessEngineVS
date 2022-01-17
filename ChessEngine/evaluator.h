#pragma once

#include "board.h"
#include "utility.h"
#include "piece.h"
#include <unordered_map>

constexpr int PV_SCORE = 300000;
constexpr int CAPTURE_SCORE = 200000;
constexpr int KILLER_MOVE_ONE = 100000;
constexpr int KILLER_MOVE_TWO = 90000;
constexpr int HISTORY_MOVE_OFFSET = 1000;

class Evaluator
{
public:
	Evaluator();
	static int evaluate(Board* b);
	static bool compare(Board* pos, Move* m_1, Move* m_2, Move* pv_move, Move* prev_best, std::vector<Move*>* killer_moves_at_depth, bool left_most);
	static int mirror_vertical(int i);
	static int score_quiet_move(Board* pos, Move *m);
	static int score_move(Board* pos, Move* m, Move* pv_move, Move* prev_best,  std::vector<Move*>* killer_moves_at_depth, bool left_most);
	static int score_capture(Board* pos, Move* m);
	~Evaluator();
	static void init_tables();
	static void reset_history();
	static int history[12][64];

private:
	static const int INITIAL_MATERIAL_VALUE = 4572;
	
	static int open_files[2][8];
	static int PawnTable[64];
	static int PawnTableEndGame[64];
	static int PawnTableEndGameMO[64];
	static int KnightTable[64];
	static int KnightTableEndGame[64];
	static int BishopTable[64];
	static int BishopTableEndGame[64];
	static int RookTable[64];
	static int RookTableEndGame[64];
	static int QueenTable[64];
	static int QueenTableEndGame[64];
	static int KingTable[64];
	static int KingTableEndGame[64];
	static int game_phase_inc[13];
	static int mg_value[13]; 
	static int eg_value[13];
	static int distances_bonus[64][64];
	static int dist_queen_to_king[64][64];
	static int dist_rook_to_king[64][64];
	static int dist_knight_to_king[64][64];
	static int dist_bishop_to_king[64][64];
	static int north_west_diagonal[64];
	static int north_east_diagonal[64];


};
