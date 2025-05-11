#include "CubeSolver.h"
#include <iostream>

using namespace std;


CubeSolver::CubeSolver(const Cube& initialCube) : start(initialCube) {}

int CubeSolver::heuristic(const Cube& cube) {
    int misplaced = 0;
    for (int f = 0; f < 6; ++f) {
        auto face = cube.getFace(static_cast<Cube::Face>(f));
        Cube::Colour center = face[1][1];
        for (auto& row : face)
            for (auto& color : row)
                if (color != center) ++misplaced;
    }
    return misplaced;
}

vector<string> CubeSolver::getPossibleMoves() {
    vector<string> moves;
    string faces = "FBUDLR";
    for (char f : faces) {
        moves.push_back(string(1, f) + "");
        moves.push_back(string(1, f) + "'");
    }
    return moves;
}
Cube CubeSolver::applyMove(const Cube& cube, const string& move) {
    Cube newCube = cube;
    Cube::Rotation dir = (move[1] == '\'') ? Cube::Rotation::COUNTER_CLOCKWISE : Cube::Rotation::CLOCKWISE;
    Cube::Face face = Cube::Face::FRONT; // Initialize with a default value

    switch (move[0]) {
    case 'F': face = Cube::Face::FRONT; break;
    case 'B': face = Cube::Face::BACK; break;
    case 'U': face = Cube::Face::UP; break;
    case 'D': face = Cube::Face::DOWN; break;
    case 'L': face = Cube::Face::LEFT; break;
    case 'R': face = Cube::Face::RIGHT; break;
    default:
        cerr << "Invalid move: " << move << endl;
        return newCube; // Return the cube unchanged for invalid moves
    }

    newCube.rotateFace(face, dir);
    return newCube;
}

vector<string> CubeSolver::solve(const Cube& currentCube) {
    using State = pair<int, Cube>;
    priority_queue<pair<int, pair<Cube, vector<string>>>, vector<pair<int, pair<Cube, vector<string>>>>, greater<>> pq;
    unordered_set<string> visited;

    pq.push({ heuristic(currentCube), {currentCube, {}} });

    while (!pq.empty()) {
        auto& [cost, node] = pq.top(); pq.pop();
        while (!pq.empty()) {
            const auto& topElement = pq.top();
            pq.pop();
            int cost = topElement.first;
            auto& node = topElement.second;
            Cube cube = node.first;
            vector<string> path = node.second;

            if (cube.isSolved()) return path;
            if (path.size() >= 20) continue;

            for (auto& move : getPossibleMoves()) {
                Cube next = applyMove(cube, move);
                vector<string> nextPath = path;
                nextPath.push_back(move);
                pq.push({ heuristic(next) + static_cast<int>(nextPath.size()), {next, nextPath} });
            }
        }
        Cube cube = node.first;
        vector<string> path = node.second;

        if (cube.isSolved()) return path;
        if (path.size() >= 20) continue;

        for (auto& move : getPossibleMoves()) {
            Cube next = applyMove(cube, move);
            vector<string> nextPath = path;
            nextPath.push_back(move);
            pq.push({ heuristic(next) + static_cast<int>(nextPath.size()), {next, nextPath} });
        }
    }

    return {};
}
