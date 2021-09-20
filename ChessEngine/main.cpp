#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif 
#include "board.h"
#include "movegen.h"
#include "search.h"
#include <chrono>
#include <vector>
#include <iostream>


/**
 * @brief Runs the program
 *
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return int
 */

void perft(Board* b, int depth);
void search(Board* b, int depth);
void split_string(std::vector<std::string>* v, std::string s);
void uci_console();

int main(int argc, char** argv)
{
	uci_console();


	_CrtDumpMemoryLeaks();
	return 0;
}

void perft(Board* b, int depth) {
	std::list<Board*>* moves = b->possible_moves();
	int total = 0;
	for (Board* const& move : *moves) {
		Movegen* gen = new Movegen(move);
		int num_moves = gen->generate_moves(depth - 1);
		total += num_moves;
		std::cout << move->get_last_move() << ": " << num_moves << std::endl;
		delete gen;
	}
	for (Board* const& move : *moves) {
		delete move;
	}
	std::cout << "Nodes searched: " << total << std::endl;
	std::cout << std::endl;
	delete moves;
}

void search(Board* b, int depth) {
	Search* s = new Search();
	s->evaluate_iterative_deepening(b, depth);
	delete s;
}

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
			else if (split->size() >= 2) {
				if ((*split)[0] == "position") {
					if ((*split)[1] == "startpos") {
						delete board;
						board = new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
					}
					else if (split->size() >= 3) {
						if ((*split)[1] == "fen") {
							delete board;
							board = new Board((*split)[2]);
						}
					}
				}
				if ((*split)[0] == "go") {
					if ((*split)[1] == "perft") {
						if (split->size() == 3) {
							int depth = std::stoi((*split)[2]);
							perft(board, depth);
						}
					} else if ((*split)[1] == "depth") {
						if (split->size() == 3) {
							int depth = std::stoi((*split)[2]);
							search(board, depth);
						}
					}
				}
			}
		}


		delete split;
	}

	delete board;
}

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
