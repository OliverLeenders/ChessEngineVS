#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif

// includes
#include "board.h"
#include "movegen.h"
#include "search.h"
#include <chrono>
#include <vector>
#include <iostream>

// function definitions

void perft(Board* b, int depth);
void search(Board* b, int depth);
void split_string(std::vector<std::string>* v, std::string s);
void uci_console();

/**
 * @brief Runs the program
 *
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return int
 */
int main(int argc, char** argv)
{
	// run UCI console
	uci_console();

	// dump memory leaks into debug output
	_CrtDumpMemoryLeaks();

	// exit
	return 0;
}

/**
 * Driver function for Perft testing.
 *
 * \param b position to search from
 * \param depth depth to search to
 */
void perft(Board* b, int depth) {
	std::list<Move*>* moves = b->possible_moves();
	Evaluator* e = new Evaluator();
	moves->sort([b, e](Move* m_1, Move* m_2) {return e->compare(b, m_1, m_2); });
	int total = 0;
	for (Move* const& move : *moves) {
		b->make_move(move);
		Movegen* gen = new Movegen(b);
		int num_moves = gen->generate_moves(depth - 1);
		total += num_moves;
		std::cout << move->to_string() << ": " << num_moves << std::endl;
		delete gen;
		b->unmake_move();
	}
	for (Move* const& move : *moves) {
		delete move;
	}
	std::cout << std::endl;
	std::cout << "Nodes searched: " << total << std::endl;
	std::cout << std::endl;
	delete e;
	delete moves;
}

/**
 * Search driving function.
 *
 * \param b position to search
 * \param depth depth to search to
 */
void search(Board* b, int depth) {
	Search* s = new Search();
	s->evaluate_iterative_deepening(b, depth);
	delete s;
}

/**
 * Driver function for UCI console.
 *
 */
void uci_console() {
	Board* board = new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	std::cout << "UCI console -- Ready to take commands..." << std::endl;
	while (true) {
		std::string line = "";
		std::getline(std::cin, line);
		std::vector<std::string>* split = new std::vector<std::string>;
		split_string(split, line);

		if (split->size() != 0) {
			if ((*split)[0] == "quit") {
				split->clear();
				delete split;
				break;
			}
			else if ((*split)[0] == "print") {
				std::cout << board->pos_as_str() << std::endl;
			}
			else if ((*split)[0] == "attacks") {
				std::cout << board->get_attacked_by_white() << std::endl;
				std::cout << board->get_attacked_by_black() << std::endl;
			}
			else if ((*split)[0] == "pins") {
				std::cout << board->get_pins() << std::endl;
			}
			else if ((*split)[0] == "checks") {
				std::cout << board->get_checks() << std::endl;
			}
			else if ((*split)[0] == "castling") {
				std::cout << board->get_castling_rights() << std::endl;
			}
			else if ((*split)[0] == "unmake") {
				board->unmake_move();
			}
			else if ((*split)[0] == "break") {
				std::cout << "triggered breakpoint" << std::endl;
			}
			else if (split->size() >= 2) {
				if ((*split)[0] == "position") {
					if ((*split)[1] == "startpos") {
						delete board;
						board = new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
					}
					else if ((*split)[1] == "kiwipete") {
						delete board;
						board = new Board("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ");
					}
					else if (split->size() >= 3) {
						if ((*split)[1] == "fen") {
							delete board;
							std::string fen = "";
							for (int i = 2; i < split->size(); i++) {
								fen += (*split)[i] + " ";
							}
							// std::cout << fen << std::endl;
							board = new Board(fen);
							std::cout << board->pos_as_str() << std::endl << std::endl;
						}
					}
				}
				else if ((*split)[0] == "go") {
					if ((*split)[1] == "perft") {
						if (split->size() == 3) {
							int depth = std::stoi((*split)[2]);
							perft(board, depth);
						}
					}
					else if ((*split)[1] == "depth") {
						if (split->size() == 3) {
							int depth = std::stoi((*split)[2]);
							search(board, depth);
						}
					}
				}
				else if ((*split)[0] == "move") {
					Move* m = new Move((*split)[1], board->white_to_move, false);
					if ((!board->position[m->target]->is_empty()) || (board->en_passant_target_index == m->target && board->position[m->origin]->get_type() >= 11)) {
						m->is_capture = true;
					}
					board->make_move(m);
					std::cout << board->pos_as_str() << std::endl;
				}
			}
		}
		delete split;
	}
	delete board;
}

/**
 * Utility function to split a string into a vector.
 *
 * \param v vector of strings
 * \param s string to split
 */
void split_string(std::vector<std::string>* v, std::string s) {
	std::string tmp = "";
	for (int i = 0; i < s.length(); i++) {
		if (s[i] != ' ') {
			tmp.push_back(s[i]);
		}
		else {
			v->push_back(tmp);
			tmp = "";
		}
	}
	v->push_back(tmp);
}