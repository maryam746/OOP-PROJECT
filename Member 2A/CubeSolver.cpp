#include "CubeSolver.hpp"
#include <iostream>
#include <unordered_map>
#include <queue>
#include <algorithm>

using namespace std;

CubeSolver::CubeSolver(RubiksCube c) : cube(c) {}

int CubeSolver::heuristic(const RubiksCube& cube) {
    int misplaced = 0;
    for (int i = 0; i < 6; ++i) {
        for (int r = 0; r < 3; ++r) {
            for (int c = 0; c < 3; ++c) {
                if (cube.toString()[i * 9 + r * 3 + c] != cube.toString()[i * 9]) {
                    misplaced++;
                }
            }
        }
    }
    return misplaced;
}

vector<string> CubeSolver::getPossibleMoves(const RubiksCube& cube) {
    return cube.getValidMoves();
}

vector<string> CubeSolver::aStarSolve(RubiksCube start) {
    unordered_map<string, pair<string, string>> cameFrom;
    unordered_map<string, int> costSoFar;
    using PQElement = pair<int, RubiksCube>;

    auto cmp = [](const PQElement& a, const PQElement& b) { return a.first > b.first; };
    priority_queue<PQElement, vector<PQElement>, decltype(cmp)> frontier(cmp);

    string startKey = start.toString();
    frontier.push({heuristic(start), start});
    costSoFar[startKey] = 0;

    while (!frontier.empty()) {
        RubiksCube current = frontier.top().second;
        frontier.pop();

        string currentKey = current.toString();

        if (current.isSolved()) {
            vector<string> moves;
            while (cameFrom.find(currentKey) != cameFrom.end()) {
                moves.push_back(cameFrom[currentKey].second);
                currentKey = cameFrom[currentKey].first;
            }
            reverse(moves.begin(), moves.end());
            return moves;
        }

        for (const string& move : getPossibleMoves(current)) {
            RubiksCube next = current.getNextState(move);
            string nextKey = next.toString();
            int newCost = costSoFar[currentKey] + 1;

            if (costSoFar.find(nextKey) == costSoFar.end() || newCost < costSoFar[nextKey]) {
                costSoFar[nextKey] = newCost;
                int priority = newCost + heuristic(next);
                frontier.push({priority, next});
                cameFrom[nextKey] = {currentKey, move};
            }
        }
    }

    return {};
}

vector<string> CubeSolver::solve() {
    return aStarSolve(cube);
}
