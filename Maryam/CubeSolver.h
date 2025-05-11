#pragma once
#ifndef CUBESOLVER_H
#define CUBESOLVER_H

#include "Cube.h"
#include <vector>
#include <string>
#include <queue>
#include <unordered_set>
#include <map>
#include <iostream>

using namespace std;



class CubeSolver {
public:
    CubeSolver(const Cube& initialCube);
    vector<string> solve(const Cube& currentCube);

private:
    Cube start;

    int heuristic(const Cube& cube);
    vector<string> getPossibleMoves();
    Cube applyMove(const Cube& cube, const string& move);
};

#endif
