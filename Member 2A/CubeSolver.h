#ifndef CUBESOLVER_H
#define CUBESOLVER_H

#include "RubiksCube.h"
#include <vector>
#include <string>
using namespace std;

class CubeSolver {
private:
    RubiksCube cube;

    // Heuristic: Calculates the number of misplaced pieces
    int heuristic(const RubiksCube& cube);
    
    // Generate all valid moves for the current cube state
    vector<string> getPossibleMoves(const RubiksCube& cube);
    
    // A* algorithm
    vector<string> aStarSolver(const RubiksCube& startCube);

public:
    CubeSolver(RubiksCube& initialCube);
    vector<string> solve();
};

#endif
