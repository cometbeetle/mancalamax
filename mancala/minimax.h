/*
 * project:  Mancalamax
 * file:     minimax.h
 * author:   Ethan Mentzer
 * modified: 2025-03-08
 */

#ifndef MINIMAX_H
#define MINIMAX_H

#include <time.h>
#include "state.h"

/**
 * Typedef representing a heuristic function, which takes a
 * GameState and a player integer.
 */
typedef double (*Heuristic)(GameState, int);

extern int minimaxIterDep(GameState state, time_t timeLimit, int maxDepth, Heuristic customHeuristic);
extern int minimaxAlphaBeta(GameState state, int maxDepth, Heuristic customHeuristic);

#endif //MINIMAX_H
