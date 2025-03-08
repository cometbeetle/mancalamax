/*
 * project:  Mancalamax
 * file:     main.c
 * author:   Ethan Mentzer
 * modified: 2025-03-08
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "state.h"
#include "minimax.h"


#define MAIN1


#ifdef MAIN0
/**
 * Main program entrypoint.
 *
 * Receives a string from standard input of the form:
 *
 *     STATE <N> <p11> ... <p1N> <p21> ... <p2N> <p1S> <p2S> <turn> <player>
 *
 * and prints out the optimal move for the player found via minimax / alpha-beta pruning,
 * at a fixed search depth of 12. This depth can be reached well within one second,
 * so for better moves, it can be increased until time becomes a problem.
 *
 * Alternatively, iterative deepening can be used with a fixed time limit.
 */
int main() {
    char header[32];
    scanf("%31s", header);

    if (strcmp(header, "STATE") != 0) {
        fprintf(stderr, "ERROR: Invalid header (expected \"STATE\")\n");
        exit(1);
    }

    int pits, store1, store2, ply, currentTurn;
    scanf("%d", &pits);

    int* player1 = (int*)calloc(sizeof(int), pits);
    int* player2 = (int*)calloc(sizeof(int), pits);

    for (int i = 0; i < pits; i++) {
        int stones;
        scanf("%d", &stones);
        player1[i] = stones;
    }

    for (int i = 0; i < pits; i++) {
        int stones;
        scanf("%d", &stones);
        player2[i] = stones;
    }

    scanf("%d %d %d %d", &store1, &store2, &ply, &currentTurn);

    GameState state = new_GameState(pits, player1, player2, store1, store2, ply, currentTurn-1);

    //const int move = minimaxIterDep(state, 290, 100, NULL);
    const int move = minimaxAlphaBeta(state, 12, NULL);

    if (move == -1)
        printf("PIE\n");
    else
        printf("%d\n", move);

    GameState_free(state);
}
#endif


#ifdef MAIN1
/**
 * Custom test heuristic.
 */
double h2(GameState state, const int player) {
    if (player == 0)
        return GameState_getScore(state, 0) - 0.65*GameState_getScore(state, 1);
    else
        return GameState_getScore(state, 1) - 1.85*GameState_getScore(state, 0);
}

int main() {
    GameState state = GameState_initBasic();

    GameState_print(state, true);

    while (!GameState_isTerminal(state)) {
        int move;
        if (GameState_getCurrentTurn(state) == 0) {
            move = minimaxIterDep(state, 1000, 1000, NULL);
            //move = minimaxAlphaBeta(state, 12, NULL);
        } else {
            move = minimaxIterDep(state, 100, 1000, NULL);
            //scanf("%d", &move);
            //srand(time(NULL));
            //LinkedList validMoves = GameState_getValidMoves(state);
            //const int randIndex = rand() % LinkedList_size(validMoves);
            //move = LinkedList_get(validMoves, randIndex);
            //LinkedList_free(validMoves);
        }

        printf("SELECTED: %d\n", move);

        GameState newState = GameState_move(state, move);
        GameState_free(state);
        state = newState;

        GameState_print(state, true);

    }

    printf("SCORE DIFF: %d\n", GameState_getScore(state, 1) - GameState_getScore(state, 0));

    GameState_free(state);
}
#endif
