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

/**
 * Find the optimal move using iterative deepening and minimax / alpha-beta pruning.
 * Starts at depth 2, and increases the depth until time runs out.
 *
 * If time runs out during the search of a given depth, the result found for the
 * previous depth is returned, as it is considered a more complete search.
 *
 * @param state The state to search
 * @param timeLimit The maximum amount of time (in ms) to spend searching
 * @param maxDepth The maximum depth to search before time runs out
 * @param customHeuristic The heuristic function to use. Can be NULL to use the default.
 * @return The optimal move, or a random move if the search failed
 */
extern int minimaxIterDep(GameState state, time_t timeLimit, int maxDepth, Heuristic customHeuristic);

/**
 * Find the optimal move using minimax with alpha-beta pruning.
 *
 * @param state The state to search
 * @param maxDepth The maximum depth to search
 * @param customHeuristic The heuristic function to use. Can be NULL to use the default.
 * @return The optimal move, or a random move if the search failed
 */
extern int minimaxAlphaBeta(GameState state, int maxDepth, Heuristic customHeuristic);

#endif //MINIMAX_H
