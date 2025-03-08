/*
 * project:  Mancalamax
 * file:     minimax.c
 * author:   Ethan Mentzer
 * modified: 2025-03-08
 */

#include <stdlib.h>
#include <math.h>

#include "minimax.h"
#include "state.h"
#include "../utils/LinkedList.h"


// Keep track of start time / time limits / heuristic.
static time_t start, limit;
static Heuristic h;

// Declare static functions.
static void minValue(
    double* util,
    int* bestMove,
    GameState state,
    double alpha,
    double beta,
    int optimizeFor,
    int depth);

static void maxValue(
    double* util,
    int* bestMove,
    GameState state,
    double alpha,
    double beta,
    int optimizeFor,
    int depth);

/**
 * The utility of a state is defined by the difference in player scores.
 * Depending on who we are maximizing for, subtraction is performed differently.
 * The utility function should be equivalent for both players.
 */
static double utility(GameState state, const int player) {
    if (player == 0)
        return GameState_getScore(state, 0) - GameState_getScore(state, 1);
    else
        return GameState_getScore(state, 1) - GameState_getScore(state, 0);
}

/**
 * Heuristic function for minimax. Currently, it evaluates states the
 * same as our utility function. The heuristic should be equivalent
 * for both players.
 */
static double heuristic(GameState state, const int player) {
    if (player == 0)
        return GameState_getScore(state, 0) - GameState_getScore(state, 1);
    else
        return GameState_getScore(state, 1) - GameState_getScore(state, 0);
}


/**
 * Helper function to convert a timespec struct to milliseconds.
 */
static time_t timespecToMs(const struct timespec ts) {
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}


int minimaxIterDep(GameState state, const time_t timeLimit, const int maxDepth, Heuristic customHeuristic) {
    if (state == NULL) return -2;

    struct timespec timer;
    clock_gettime(CLOCK_MONOTONIC, &timer);

    // Set start and end times.
    start = timespecToMs(timer);
    limit = timeLimit;
    time_t now = start;

    // Set heuristic.
    h = customHeuristic == NULL ? heuristic : customHeuristic;

    LinkedList bestMoves = new_LinkedList();
    int depth = 2;

    // Search until time runs out, or until the max depth has been reached.
    while (now < start + timeLimit && depth <= maxDepth) {
        double util;
        int bestMove;
        maxValue(
            &util, &bestMove,
            state,
            -INFINITY, INFINITY,
            GameState_getCurrentTurn(state),
            depth);

        if (bestMove != -2)
            LinkedList_prepend(bestMoves, bestMove);

        depth++;

        clock_gettime(CLOCK_MONOTONIC, &timer);
        now = timespecToMs(timer);
    }

    // Return a random move if nothing found.
    if (LinkedList_size(bestMoves) < 2) {
        srand(time(NULL));
        LinkedList validMoves = GameState_getValidMoves(state);
        const int randIndex = rand() % LinkedList_size(validMoves);
        const int result = LinkedList_get(validMoves, randIndex);
        LinkedList_free(bestMoves);
        LinkedList_free(validMoves);
        return result;
    }

    // Always get the last fully completed search depth result.
    const int result = LinkedList_get(bestMoves, 1);
    LinkedList_free(bestMoves);
    return result;
}


int minimaxAlphaBeta(GameState state, const int maxDepth, Heuristic customHeuristic) {
    if (state == NULL) return -2;

    double util;
    int bestMove;
    start = limit = 0;

    // Set heuristic.
    h = customHeuristic == NULL ? heuristic : customHeuristic;

    maxValue(
        &util, &bestMove,
        state,
        -INFINITY, INFINITY,
        GameState_getCurrentTurn(state),
        maxDepth);

    // Return a random move if nothing found.
    if (bestMove == -2) {
        srand(time(NULL));
        LinkedList validMoves = GameState_getValidMoves(state);
        const int randIndex = rand() % LinkedList_size(validMoves);
        const int result = LinkedList_get(validMoves, randIndex);
        LinkedList_free(validMoves);
        return result;
    }

    return bestMove;
}


static void maxValue(
    double* util,
    int* bestMove,
    GameState state,
    double alpha,
    const double beta,
    const int optimizeFor,
    int depth)
{
    // If we are in a terminal state, evaluate utility.
    if (GameState_isTerminal(state)) {
        *util = utility(state, optimizeFor);
        *bestMove = -2;
        return;
    }

    struct timespec timer;
    clock_gettime(CLOCK_MONOTONIC, &timer);
    const time_t now = timespecToMs(timer);

    // If we have reached artificial limit, use the heuristic.
    if (depth <= 0 || (limit > 0 && now > start + limit)) {
        *util = h(state, optimizeFor);
        *bestMove = -2;
        return;
    }

    depth--;
    double v = -INFINITY;
    int newBestMove = -2;

    LinkedList validMoves = GameState_getValidMoves(state);

    for (Node a = LinkedList_head(validMoves); a != NULL; a = Node_next(a)) {
        GameState newState = GameState_move(state, Node_value(a), false);
        double v2;
        int a2;

        // Maximize the value again if player is unchanged.
        if (GameState_getCurrentTurn(newState) == GameState_getCurrentTurn(state)) {
            maxValue(
                &v2, &a2,
                newState,
                alpha,
                beta,
                optimizeFor,
                depth);
        } else {
            minValue(
                &v2, &a2,
                newState,
                alpha,
                beta,
                optimizeFor,
                depth);
        }

        GameState_free(newState);

        if (v2 > v) {
            v = v2;
            newBestMove = Node_value(a);
            alpha = alpha > v ? alpha : v;
        }

        // Alpha > beta  ==>  prune
        if (v >= beta) {
            *util = v;
            *bestMove = newBestMove;
            LinkedList_free(validMoves);
            return;
        }
    }

    *util = v;
    *bestMove = newBestMove;
    LinkedList_free(validMoves);
}


static void minValue(
    double* util,
    int* bestMove,
    GameState state,
    const double alpha,
    double beta,
    const int optimizeFor,
    int depth)
{
    // If we are in a terminal state, evaluate utility.
    if (GameState_isTerminal(state)) {
        *util = utility(state, optimizeFor);
        *bestMove = -2;
        return;
    }

    struct timespec timer;
    clock_gettime(CLOCK_MONOTONIC, &timer);
    const time_t now = timespecToMs(timer);

    // If we have reached artificial limit, use the heuristic.
    if (depth <= 0 || (limit > 0 && now > start + limit)) {
        *util = h(state, optimizeFor);
        *bestMove = -2;
        return;
    }

    depth--;
    double v = INFINITY;
    int newBestMove = -2;

    LinkedList validMoves = GameState_getValidMoves(state);

    for (Node a = LinkedList_head(validMoves); a != NULL; a = Node_next(a)) {
        GameState newState = GameState_move(state, Node_value(a), false);
        double v2;
        int a2;

        // Minimize the value again if player is unchanged.
        if (GameState_getCurrentTurn(newState) == GameState_getCurrentTurn(state)) {
            minValue(
                &v2, &a2,
                newState,
                alpha,
                beta,
                optimizeFor,
                depth);
        } else {
            maxValue(
                &v2, &a2,
                newState,
                alpha,
                beta,
                optimizeFor,
                depth);
        }

        GameState_free(newState);

        if (v2 < v) {
            v = v2;
            newBestMove = Node_value(a);
            beta = beta < v ? beta : v;
        }

        // Alpha > beta  ==>  prune
        if (v <= alpha) {
            *util = v;
            *bestMove = newBestMove;
            LinkedList_free(validMoves);
            return;
        }
    }

    *util = v;
    *bestMove = newBestMove;
    LinkedList_free(validMoves);
}



