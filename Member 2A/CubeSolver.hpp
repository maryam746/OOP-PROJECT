#ifndef CUBESOLVER_HPP
#define CUBESOLVER_HPP

#include "RubiksCube.hpp"
#include <vector>
#include <string>

using namespace std;

class CubeSolver {
private:
    RubiksCube cube;

    int heuristic(const RubiksCube& cube);
    vector<string> getPossibleMoves(const RubiksCube& cube);
    vector<string> aStarSolve(RubiksCube start);

public:
    CubeSolver(RubiksCube c);
    vector<string> solve();
};

#endif
