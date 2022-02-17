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
	if (depth == 0) {
		std::cout << "Nodes searched: 1" << std::endl;
		return;
	}
	std::vector<Move*>* moves = b->possible_moves();
	std::cout << "number of moves " << moves->size() << std::endl;
	Evaluator* e = new Evaluator();
	//moves->sort([b, e](Move* m_1, Move* m_2) {return e->compare(b, m_1, m_2); });
	int total = 0;
	std::sort(moves->begin(), moves->end(), [b](Move* m_1, Move* m_2) -> bool {return Evaluator::compare(b, m_1, m_2, nullptr, nullptr, nullptr, false); });
	for (Move* const& move : *moves) {
		int score = Evaluator::score_move(b, move, nullptr, nullptr, nullptr, false);
		b->make_move(move);
		Movegen* gen = new Movegen(b);
		int num_moves = gen->generate_moves(depth - 1);
		total += num_moves;
		std::cout << move->to_string() << ": " << num_moves << " score " << score << std::endl;
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
void search(Board* b, int depth, std::thread* search_thread, Search* s) {
	search_thread = new std::thread(&Search::evaluate_iterative_deepening, s, b, depth);
	search_thread->detach();
}



void search_time(Board* b, int time_ms, std::thread* search_thread, Search* s) {
	search_thread = new std::thread(&Search::evaluate_iterative_deepening_time, s, b, time_ms);
	search_thread->detach();
}

void ni_search(Board* b, int depth, Search* s) {
	s->evaluate(b, depth);
}

/**
 * Driver function for UCI console.
 *
 */
void uci_console() {
	Evaluator::init_tables();
	zobrist_hashmap::init_bases();

	srand((unsigned)time(0));
	int move_overhead = 25;
	Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	board.set_hash_size(256);
	std::cout << "UCI console -- Ready to take commands..." << std::endl;
	bool is_calculating = false;
	bool has_just_finished = false;
	std::thread* search_thread = new std::thread();
	Search* s = new Search();
	while (true) {
		std::string line = "";
		std::getline(std::cin, line);
		std::vector<std::string>* split = new std::vector<std::string>;
		Utility::split_string(split, line);

		if (split->size() != 0) {
			if (is_calculating) {
				if ((*split)[0] == "stop") {
					s->stop_now = true;
				}
				is_calculating = false;
			}
			if ((*split)[0] == "quit") {
				split->clear();
				delete split;
				break;
			}
			else if ((*split)[0] == "print" || (*split)[0] == "d") {
				std::cout << board.pos_as_str() << "Key: " << std::uppercase << std::hex << board.pos_hash << std::dec << std::nouppercase << "\n" << std::endl;
			}
			else if ((*split)[0] == "attacks") {
				std::cout << board.get_attacked() << std::endl;
			}
			else if ((*split)[0] == "pins") {
				std::cout << board.get_pins() << std::endl;
			}
			else if ((*split)[0] == "checks") {
				std::cout << board.get_checks() << std::endl;
			}
			else if ((*split)[0] == "castling") {
				std::cout << board.get_castling_rights() << std::endl;
			}
			else if ((*split)[0] == "clearhash") {
				board.transposition_table->clear();
			}
			else if ((*split)[0] == "uci") {
				std::cout << "id name Leandor v1.1" << std::endl;
				std::cout << "id author Oliver Leenders" << std::endl;
				std::cout << "option name Move Overhead type spin default 25 min 0 max 5000" << std::endl;
				std::cout << "option name Threads type spin default 1 min 1 max 1" << std::endl;
				std::cout << "option name Hash type spin default 256 min 16 max 2048" << std::endl;
				std::cout << "uciok" << std::endl;
			}
			else if ((*split)[0] == "isready") {
				std::cout << "readyok" << std::endl;
			}
			else if ((*split).size() >= 6 && ((*split)[0] == "setoption" && (*split)[1] == "name" && (*split)[2] == "Move" && (*split)[3] == "Overhead" && (*split)[4] == "value")) {
				int new_overhead = std::stoi((*split)[5]);
				if (new_overhead >= 0 && new_overhead <= 5000) {
					move_overhead = new_overhead;
				}

			} else if ((*split).size() >= 5 && (*split)[0] == "setoption" && (*split)[1] == "name" && (*split)[2] == "Hash" && (*split)[3] == "value") {
				int new_size = std::max(16, std::min(2048, std::stoi((*split)[4])));
				std::cout << new_size << std::endl;
				board.set_hash_size(new_size);
			}
			else if ((*split)[0] == "unmake") {
				board.unmake_move();
			}
			else if ((*split)[0] == "break") {
				std::cout << "triggered breakpoint" << std::endl;
			}
			else if ((*split)[0] == "eval") {
				std::cout << Evaluator::evaluate(&board) << std::endl;
			}
			else if (split->size() >= 2) {
				if ((*split)[0] == "position") {
					//std::cout << "pos set" << std::endl;
					if ((*split)[1] == "startpos") {
						//std::cout << "initial pos" << std::endl;
						board.set_pos_from_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
						if (split->size() > 3) {
							if ((*split)[2] == "moves") {
								for (int i = 3; i < split->size(); i++) {
									//std::cout << (*split)[i] << std::endl;
									Move* m = new Move((*split)[i], board.white_to_move, false, false);
									if ((!board.position[m->target].is_empty()) || (board.en_passant_target_index == m->target && board.position[m->origin].get_type() >= 11)) {
										m->is_capture = true;
									}
									if (board.position[m->origin].get_type() >= 11) {
										m->is_pawn_push = true;
									}
									board.make_move(m);
								}
							}
						}
					}
					else if ((*split)[1] == "kiwipete") {
						board.set_pos_from_fen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
					}
					else if ((*split)[1] == "pos3") {
						board.set_pos_from_fen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");
					}
					else if (split->size() >= 3) {
						if ((*split)[1] == "fen") {
							std::string fen = "";
							for (int i = 2; i < 8; i++) {
								fen += (*split)[i] + " ";
							}
							board.set_pos_from_fen(fen);
							if (split->size() > 8) {
								if ((*split)[8] == "moves") {
									for (int i = 9; i < split->size(); i++) {
										Move* m = new Move((*split)[i], board.white_to_move, false, false);
										if ((!board.position[m->target].is_empty()) || (board.en_passant_target_index == m->target && board.position[m->origin].get_type() >= 11)) {
											m->is_capture = true;
										}
										if (board.position[m->origin].get_type() >= 11) {
											m->is_pawn_push = true;
										}
										board.make_move(m);
									}
								}
							}
							std::cout << board.pos_as_str() << std::endl << std::endl;
						}
					}
				}
				else if ((*split)[0] == "go") {
					if ((*split)[1] == "perft") {
						if (split->size() == 3) {
							int depth = std::stoi((*split)[2]);
							perft(&board, depth);
						}
					}
					else if ((*split)[1] == "depth") {
						if (split->size() == 3) {
							int depth = std::stoi((*split)[2]);
							is_calculating = true;
							search(&board, depth, search_thread, s);
						}
					}
					else if ((*split)[1] == "infinite") {
						is_calculating = true;
						search(&board, 256, search_thread, s);
					}
					else if ((*split)[1] == "nidepth") {
						if (split->size() == 3) {
							int depth = std::stoi((*split)[2]);
							ni_search(&board, depth, s);
						}
					}
					else if ((*split).size() >= 9 && (*split)[1] == "wtime" && (*split)[3] == "btime" && (*split)[5] == "winc" && (*split)[7] == "binc") {
						int w_time = std::stoi((*split)[2]);
						int b_time = std::stoi((*split)[4]);
						int w_inc = std::stoi((*split)[6]);
						int b_inc = std::stoi((*split)[8]);
						if (board.white_to_move) {
							search_time(&board, std::min(w_time, (w_time + 25 * w_inc) / 25) - move_overhead, search_thread, s);
						}
						else {
							search_time(&board, std::min(b_time, (b_time + 25 * b_inc) / 25) - move_overhead, search_thread, s);
						}
					}
					else if ((*split)[1] == "movetime") {
						if (split->size() == 3) {
							int time_ms = std::stoi((*split)[2]);
							search_time(&board, time_ms - move_overhead, search_thread, s);
						}
					}

				}
				else if ((*split)[0] == "move") {
					Move* m = new Move((*split)[1], board.white_to_move, false, false);
					if ((!board.position[m->target].is_empty()) || (board.en_passant_target_index == m->target && board.position[m->origin].get_type() >= 11)) {
						m->is_capture = true;
					}
					if (board.position[m->origin].get_type() >= 11) {
						m->is_pawn_push = true;
					}
					board.make_move(m);
					std::cout << board.pos_as_str() << std::endl;
				}
			}
		}
		delete split;
	}
	delete s;
	delete search_thread;
}

