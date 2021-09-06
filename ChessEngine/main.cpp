#include "board.h"
#include "movegen.h"
#include "search.h"
#include <chrono>
#include <iostream>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else 
#define DBG_NEW new
#endif

using namespace std;
using namespace std::chrono;
/**
 * @brief Runs the program
 *
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return int
 */
int main(int argc, char** argv)
{
    Board* board = new Board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    cout << board->pos_as_str() << endl;
    cout << board->get_last_move() << endl;
    cout << board->get_attacked_squares() << endl;

    list<Board*>* moves = board->get_legal_moves();
    cout << to_string(moves->size()) + " moves found" << endl;
    Search* s = new Search();
    auto start = high_resolution_clock::now();

    // Call the function, here sort()
    double eval = s->evaluate_iterative_deepening(board, 8);
    

    // Get ending timepoint
    auto stop = high_resolution_clock::now();
    cout.precision(2);
    cout << "eval: " << eval << endl;
    // Get duration. Substart timepoints to 
    // get durarion. To cast it to proper unit
    // use duration cast method
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by function: "
        << duration.count() << " microseconds" << endl;
    
    delete s;
    cout << "running move_gen --" << endl;

    for (Board* const& i : *moves)
    {
        Movegen* m_2 = new Movegen(i);
        //std::cout << i->pos_as_str() << std::endl;
        cout << i->get_last_move() + ": " + to_string(m_2->generate_moves(2)) << endl;
        delete m_2;
        //cout << i->get_last_move() << endl;
    }

    Movegen* m = new Movegen(board);
    int num_moves = m->generate_moves(1);
    cout << to_string(num_moves) + " moves found" << endl;
    num_moves = m->generate_moves(2);
    cout << to_string(num_moves) + " moves found" << endl;



    delete m;

    delete board;
    for (Board* const& i : *moves)
    {
        delete i;
    }

    delete moves;

    _CrtDumpMemoryLeaks();
    return 0;
}