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

/**
 * Initialize a new GameState.
 *
 * The player1 and player2 arrays must both be of size pits.
 *
 * @param pits The number of pits per player
 * @param player1 An array pointer representing the pits of player 1
 * @param player2 An array pointer representing the pits of player 2
 * @param store1 The number of stones in player 1's store
 * @param store2 The number of stones in player 2's store
 * @param ply The current ply number
 * @param currentTurn The player allowed to make the next move (0 or 1)
 * @return A pointer to an initialized GameState struct.
 */
extern GameState new_GameState(
    int pits,
    int* player1,
    int* player2,
    int store1,
    int store2,
    int ply,
    int currentTurn);

/**
 * Free the memory used by a GameState, including the player arrays.
 */
extern void GameState_free(GameState state);

/**
 * Start a new game with a certain number of pits, and a
 * certain number of stones per pit.
 *
 * @param pits The number of pits per player
 * @param stonesPerPit The number of stones per pit
 * @return A pointer to an initialized GameState struct.
 */
extern GameState GameState_initCustom(int pits, int stonesPerPit);

/**
 * Start a new classic game (6 pits per player, 4 stones per pit).
 */
extern GameState GameState_initBasic();

/**
 * Returns a pointer to a copy of the given state.
 */
extern GameState GameState_copy(GameState state);

/**
 * Display the game state in a useful manner. Prints a preceding
 * newline if desired.
 */
extern void GameState_print(GameState state, bool newline);

/**
 * Returns whether the game is in a terminal state.
 */
extern bool GameState_isTerminal(GameState state);

/**
 * Returns a LinkedList of valid moves for the current player.
 */
extern LinkedList GameState_getValidMoves(GameState state);

/**
 * Returns the player allowed to make the next move (0 or 1).
 */
extern int GameState_getCurrentTurn(GameState state);

/**
 * Returns the current score of a specified player (0 or 1).
 */
extern int GameState_getScore(GameState state, int player);

/**
 * Apply a move to the current state, given a pit.
 * If the "PIE" rule move is available, the pit input can be -1.
 *
 * Returns a new GameState object with the move applied.
 *
 * @param state The state to apply the move to
 * @param pit The pit number to sow from (integer, current player's perspective)
 * @param autoFree Whether to free the old state before returning the updated state
 * @return A pointer to a new, updated GameState struct.
 */
extern GameState GameState_move(GameState state, int pit, bool autoFree);


#endif //STATE_H
