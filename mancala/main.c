/*
 * project:  Mancalamax
 * file:     main.c
 * author:   Ethan Mentzer
 * modified: 2025-03-08
 */

#include <stdio.h>
#include <stdbool.h>

#include "state.h"
#include "minimax.h"


/**
 * Custom test heuristic (likely worse than the default heuristic).
 */
double h2(GameState state, const int player) {
    if (player == 0)
        return GameState_getScore(state, 0) - 0.65*GameState_getScore(state, 1);
    else
        return GameState_getScore(state, 1) - 1.85*GameState_getScore(state, 0);
}

int main() {
    // Create new initial board state.
    GameState state = GameState_initBasic();
    GameState_print(state, true);

    while (!GameState_isTerminal(state)) {
        int move;

        // Player 0 is the algorithm, player 1 is a user.
        if (GameState_getCurrentTurn(state) == 0) {
            // Collect move from minimax.
            move = minimaxIterDep(state, 1000, 1000, NULL);

            // SOME ALTERNATIVE COMPUTER MOVES BELOW (with and without the custom "h2" heuristic).

            //move = minimaxIterDep(state, 1000, 1000, h2);
            //move = minimaxAlphaBeta(state, 12, NULL);
            //move = minimaxAlphaBeta(state, 12, h2);

            printf("MINIMAX SELECTED: %d\n", move);
        } else {
            // Collect move from user.
            scanf("%d", &move);
            printf("USER SELECTED: %d\n", move);
        }

        // Apply move and print resulting state.
        GameState newState = GameState_move(state, move, true);
        state = newState;

        GameState_print(state, true);
    }

    GameState_free(state);
}
