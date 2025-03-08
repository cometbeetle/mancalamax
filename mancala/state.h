/*
 * project:  Mancalamax
 * file:     state.h
 * author:   Ethan Mentzer
 * modified: 2025-03-08
 */

#ifndef STATE_H
#define STATE_H

#include <stdbool.h>
#include "../utils/LinkedList.h"

/**
 * The GameState struct represents a Mancala game state.
 */
typedef struct GameState* GameState;

extern GameState new_GameState(
    int pits,
    int* player1,
    int* player2,
    int store1,
    int store2,
    int ply,
    int currentTurn);

extern void GameState_free(GameState state);
extern GameState GameState_initCustom(int pits, int stonesPerPit);
extern GameState GameState_initBasic();
extern GameState GameState_copy(GameState state);
extern void GameState_print(GameState state, bool newline);
extern bool GameState_isTerminal(GameState state);
extern LinkedList GameState_getValidMoves(GameState state);
extern int GameState_getCurrentTurn(GameState state);
extern int GameState_getScore(GameState state, int player);
extern GameState GameState_move(GameState state, int pit);


#endif //STATE_H
