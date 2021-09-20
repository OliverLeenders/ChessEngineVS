#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif 
#ifndef Search_H
#define Search_H

#include "board.h"
#include "piece.h"
#include "evaluator.h"

class Search
{
private:
    double alpha_beta(Board* pos, double alpha, double beta, unsigned int depth_left, std::list<Board*>* PV, Evaluator* e, bool left_most);
    double alpha_beta_prev_PV(Board* pos, double alpha, double beta, unsigned int depth_left, std::list<Board*>* PV, Evaluator* e, bool left_most, std::list<Board*>* prev_pv);
public:
    Search();
    double evaluate(Board* pos, unsigned int depth);
    double quiescence(Board* pos, double alpha, double beta, Evaluator* e);
    double evaluate_iterative_deepening(Board* pos, unsigned int depth);
    ~Search();
};

#endif