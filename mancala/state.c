/*
 * project:  Mancalamax
 * file:     state.c
 * author:   Ethan Mentzer
 * modified: 2025-03-08
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "state.h"
#include "../utils/LinkedList.h"


/**
 * The GameState struct represents a Mancala game state.
 */
struct GameState {
    int* players[2];
    int stores[2];
    int pits;
    int ply;
    int currentTurn;
};

static void switchTurn(GameState state) {
    if (state == NULL) return;
    state->currentTurn = state->currentTurn == 0 ? 1 : 0;
}

/**
 * Rotates the Mancala board, per the "PIE" rule (players swap).
 */
static void rotateBoard(GameState state) {
    if (state == NULL) return;

    int* oldPlayer1 = state->players[0];
    state->players[0] = state->players[1];
    state->players[1] = oldPlayer1;

    const int oldStore1 = state->stores[0];
    state->stores[0] = state->stores[1];
    state->stores[1] = oldStore1;
}

/**
 * Helper function to sum up the elements in an array of ints.
 */
static int arraySum(const int* array, const int size) {
    if (array == NULL) return 0;
    int sum = 0;
    for (int i = 0; i < size; i++) sum += array[i];
    return sum;
}


GameState new_GameState(
    const int pits,
    int* player1,
    int* player2,
    const int store1,
    const int store2,
    const int ply,
    const int currentTurn)
{
    struct GameState* const newState = (GameState)malloc(sizeof(struct GameState));

    newState->pits = pits;
    newState->players[0] = player1;
    newState->players[1] = player2;
    newState->stores[0] = store1;
    newState->stores[1] = store2;
    newState->ply = ply;
    newState->currentTurn = currentTurn;

    return newState;
}

void GameState_free(GameState state) {
    if (state == NULL) return;

    for (int player = 0; player < 2; player++) {
        free(state->players[player]);
    }

    free(state);
}

GameState GameState_initCustom(const int pits, const int stonesPerPit) {
    if (pits < 1 || stonesPerPit < 1) return NULL;

    int* player1 = (int*)calloc(sizeof(int), pits);
    int* player2 = (int*)calloc(sizeof(int), pits);

    for (int i = 0; i < pits; i++) {
        player1[i] = stonesPerPit;
        player2[i] = stonesPerPit;
    }

    return new_GameState(pits, player1, player2, 0, 0, 1, 0);
}

GameState GameState_initBasic() {
    return GameState_initCustom(6, 4);
}

GameState GameState_copy(GameState state) {
    if (state == NULL) return NULL;

    int *player1 = (int*)calloc(sizeof(int), state->pits);
    int *player2 = (int*)calloc(sizeof(int), state->pits);
    memcpy(player1, state->players[0], sizeof(int)*state->pits);
    memcpy(player2, state->players[1], sizeof(int)*state->pits);

    return new_GameState(
        state->pits,
        player1,
        player2,
        state->stores[0],
        state->stores[1],
        state->ply,
        state->currentTurn);
}

void GameState_print(GameState state, const bool newline) {
    if (state == NULL) return;

    if (newline) printf("\n");

    printf("Bird's-Eye View of Game State %p\n", state);
    printf("==============================================\n");

    printf("%c P1:  (%2d)  [ ", state->currentTurn == 0 ? '*' : ' ', state->stores[0]);
    for (int i = state->pits-1; i >= 0; i--) printf("%2d ", state->players[0][i]);
    printf("]\n");

    printf("%c P2:        [ ", state->currentTurn == 1 ? '*' : ' ');
    for (int i = 0; i < state->pits; i++) printf("%2d ", state->players[1][i]);
    printf("]  (%2d)\n", state->stores[1]);

    printf("Turn: %d\n", state->ply);
}

bool GameState_isTerminal(GameState state) {
    if (state == NULL) return false;

    for (int player = 0; player < 2; player++) {
        for (int i = 0; i < state->pits; i++) {
            if (state->players[player][i] != 0) return false;
        }
    }

    return true;
}

LinkedList GameState_getValidMoves(GameState state) {
    if (state == NULL) return NULL;

    LinkedList newList = new_LinkedList();
    const int player = state->currentTurn;

    // If the "PIE" move is available for player 2.
    if (player == 1 && state->ply == 2)  // NOTE: was "|| state->ply == 3"
        LinkedList_prepend(newList, -1);

    // List all pits where the number of stones != 0.
    for (int pit = 0; pit < state->pits; pit++) {
        if (state->players[player][pit] != 0)
            LinkedList_prepend(newList, pit+1);
    }

    return newList;
}

int GameState_getCurrentTurn(GameState state) {
    if (state == NULL) return -1;
    return state->currentTurn;
}

int GameState_getScore(GameState state, const int player) {
    if (state == NULL) return -1;
    return state->stores[player];
}

GameState GameState_move(GameState state, int pit, const bool autoFree) {
    if (state == NULL) return NULL;

    // Make a copy of the current state.
    GameState newState = GameState_copy(state);

    // Free the old state, if requested by the user.
    if (autoFree) GameState_free(state);

    // Handle "PIE" input.
    if (pit == -1) {
        rotateBoard(newState);
        switchTurn(newState);
        newState->ply++;
        return newState;
    }

    // Get current player, find adjusted pit index, and collect number of stones to distribute.
    int* player = newState->players[newState->currentTurn];
    pit--;
    const int stones = player[pit];
    player[pit] = 0;
    pit++;

    // Initialize turn variables.
    int side = newState->currentTurn;  // used to determine whether to place stones in store
    bool goAgain = false;  // used if last stone ends up in a store

    // Distribute the stones of the selected pit.
    for (int i = 0; i < stones; i++) {
        const bool lastStone = i == stones - 1;

        if (pit != newState->pits) {
            // Add stone to pit.
            player[pit]++;
        } else {
            // Only add stones to the current player's store.
            const bool addToStore = side == newState->currentTurn;
            if (addToStore) {
                newState->stores[side]++;
                if (lastStone) goAgain = true;
            }

            // Switch board sides.
            side = side == 0 ? 1 : 0;
            player = newState->players[side];
            pit = 0;

            // If we did not add to the store, make sure to add one to the next player's store.
            // If we DID add to the store, and if that wasn't the last stone, add one to the
            // next player's store, and increment i to avoid adding two stones for the same i.
            if (!addToStore) {
                player[pit]++;
            } else if (!lastStone) {
                player[pit]++;
                i++;
            }
        }

        // Determine which stones to capture (if any).
        if (lastStone && side == newState->currentTurn && player[pit] == 1) {
            int toCapture[2] = {-1, -1};

            if (side == 0) {
                toCapture[0] = pit;
                toCapture[1] = newState->pits - pit - 1;
            } else {
                toCapture[0] = newState->pits - pit - 1;
                toCapture[1] = pit;
            }

            newState->stores[side] += newState->players[0][toCapture[0]];
            newState->stores[side] += newState->players[1][toCapture[1]];
            newState->players[0][toCapture[0]] = 0;
            newState->players[1][toCapture[1]] = 0;
        }

        pit++;
    }

    // Detect completed game.
    int finalStoneRecipient = -1;
    if (arraySum(newState->players[0], newState->pits) == 0)
        finalStoneRecipient = 1;
    else if (arraySum(newState->players[1], newState->pits) == 0)
        finalStoneRecipient = 0;

    // If game is finished, player with stones on their side captures them all.
    if (finalStoneRecipient != -1) {
        player = newState->players[finalStoneRecipient];
        for (int i = 0; i < newState->pits; i++) {
            newState->stores[finalStoneRecipient] += player[i];
            player[i] = 0;
        }
    }

    // Don't switch players if player goes again.
    if (!goAgain)
        switchTurn(newState);

    newState->ply++;

    return newState;
}
