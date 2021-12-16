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
#ifndef Board_H
#define Board_H

#include "Piece.h"
#include <string>
#include <string.h>
#include <vector>
#include <list>
#include "move.h"
#include "utility.h"
#include "zobrist_hashmap.h"
//#include "evaluator.h"

class Board
{
private:
	/* data */
	bool* attacked = new bool[64];
	unsigned int* pins = new unsigned int[64];
	bool* checks = new bool[64];
	unsigned int* check_direction = new unsigned int[2];

	std::string last_move = "";
	/* functions */
	std::string get_coord_str_from_index(int i);
	std::string create_move_str(int from, int to);
	void add_king_moves(std::vector<Move*>* moves, int i);
	void add_queen_moves(std::vector<Move*>* moves, int i);
	void add_rook_moves(std::vector<Move*>* moves, int i);
	void add_bishop_moves(std::vector<Move*>* moves, int i);
	void add_knight_moves(std::vector<Move*>* moves, int i);
	void add_pawn_moves(std::vector<Move*>* moves, int i);
	void promote_with_offset(std::vector<Move*>* moves, int i, int j, unsigned promote_to);
	void move_en_passant(std::vector<Move*>* moves, int i, int j);
	void add_diagonal_attack_rays(int i);
	void add_straight_attack_rays(int i);
	bool en_passant_illegal();


	std::vector<bool*>* stack_castling_rights = new std::vector<bool*>;
	std::vector<int>* stack_en_passant_target_index = new std::vector<int>;
	std::vector<unsigned>* stack_captures = new std::vector<unsigned>;
	std::vector<Move*>* stack_moves = new std::vector<Move*>;
	//std::vector<uint64_t>* stack_hashes = new std::vector<uint64_t>;

public:
	Board();
	int white_king_pos = -1;
	int black_king_pos = -1;
	Board* prev_pos;
	// std::vector<Board*>* get_sorted_legal_moves(Evaluator *e);
	// std::vector<Board*>* get_sorted_legal_captures(Evaluator* e);
	int en_passant_target_index = -1;

	unsigned int num_checks = 0;
	/*
	* Kingside white,
	* Queenside white,
	* Kingside black
	* Queenside black
	*/
	bool* castling_rights = new bool[4];
	Piece** position = new Piece * [64];

	std::list<int>* queen_list = new std::list<int>;
	std::list<int>* rook_list = new std::list<int>;
	std::list<int>* bishop_list = new std::list<int>;
	std::list<int>* knight_list = new std::list<int>;
	std::list<int>* pawn_list = new std::list<int>;

	Board(std::string fen);
	Board(Piece* set_pos[], bool who_to_move, bool* set_castling_rights);
	void switch_move();
	void set_last_move(std::string set_move);
	int set_piece(unsigned type, int pos);
	std::string pos_as_str();
	std::string get_attacked_squares();
	std::string get_checks();
	std::string get_pins();
	std::string get_attacked();

	std::string get_castling_rights();

	std::vector<Move*>* possible_moves();
	std::vector<Move*>* get_legal_captures();
	void compute_pin_rays();
	void compute_other_checks();
	std::string get_last_move();
	void compute_attacked_squares();
	void compute_king_attacked_squares(int pos);
	void compute_knight_attacked_squares(int pos);
	void move_with_offset(std::vector<Move*>* moves, int i, int j);
	Board* clone();
	bool equals(Board* b);
	bool white_to_move;

	void make_move(Move* m);
	void unmake_move();

	~Board();
};

#endif