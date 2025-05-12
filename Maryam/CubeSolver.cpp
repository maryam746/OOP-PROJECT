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
    Cube::Face face = Cube::Face::FRONT; 

    switch (move[0]) {
    case 'F': face = Cube::Face::FRONT; break;
    case 'B': face = Cube::Face::BACK; break;
    case 'U': face = Cube::Face::UP; break;
    case 'D': face = Cube::Face::DOWN; break;
    case 'L': face = Cube::Face::LEFT; break;
    case 'R': face = Cube::Face::RIGHT; break;
    default:
        cerr << "Invalid move: " << move << endl;
        return newCube; 
    }

    newCube.rotateFace(face, dir);
    return newCube;
}
vector<string> CubeSolver::solve(const Cube& currentCube) {
    using Path = vector<string>;
    using QueueNode = pair<int, pair<Cube, Path>>;

    auto serializeCube = [](const Cube& cube) {
        string result;
        for (int face = 0; face < 6; ++face) {
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    result += to_string(static_cast<int>(cube.faces[face][i][j]));
                }
            }
        }
        return result;
        };

    priority_queue<QueueNode, vector<QueueNode>, greater<>> pq;
    unordered_set<string> visited;

    pq.push({ heuristic(currentCube), {currentCube, {}} });

    while (!pq.empty()) {
        auto [cost, node] = pq.top(); pq.pop();
        Cube cube = node.first;
        Path path = node.second;

        string serialized = serializeCube(cube);
        if (visited.count(serialized)) continue;
        visited.insert(serialized);

        if (cube.isSolved()) return path;
        if (path.size() >= 20) continue;

        for (const auto& move : getPossibleMoves()) {
            Cube next = applyMove(cube, move);
            Path nextPath = path;
            nextPath.push_back(move);

            int newCost = heuristic(next) + static_cast<int>(nextPath.size());
            pq.push({ newCost, {next, nextPath} });
        }
    }

    return {};
}
