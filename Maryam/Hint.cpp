#include "Hint.h"

#include <iostream>
using namespace std;
HintSystem::HintSystem(Cube* cube, CubeSolver* solver)
    : cube(cube), solver(solver), hintsUsed(0), solvingInProgress(false), solutionReady(false) {
}

HintSystem::~HintSystem() {
    if (solverThread.joinable()) solverThread.join();
}

void HintSystem::resetHints() {
    hintsUsed = 0;
    currentHintSequence.clear();
    solutionReady = false;
    solvingInProgress = false;
}

bool HintSystem::canGiveHint() const {
    return hintsUsed < maxHints;
}

void HintSystem::startSolving() {
    if (solvingInProgress) return; // already solving
    solvingInProgress = true;
    solutionReady = false;
    solvingStatus = "Solving...";

    solverThread = std::thread([this]() {
        vector<string> solution = solver->solve(*cube);
        if (!solution.empty()) {
            currentHintSequence = solution;
            solvingStatus = "Solution ready!";
            solutionReady = true;
        }
        else {
            solvingStatus = "Cube already solved!";
            solutionReady = true;
        }
        solvingInProgress = false;
        });
    solverThread.detach(); // background thread
}

bool HintSystem::isSolving() const { return solvingInProgress; }
bool HintSystem::isSolutionReady() const { return solutionReady; }
string HintSystem::getSolvingStatus() const { return solvingStatus; }

string HintSystem::getNextHint() {
    if (!canGiveHint()) return "Max hints used.";

    // Only call solve() if solution not already computed
    if (currentHintSequence.empty()) {
        currentHintSequence = solver->solve(*cube); // 🛑 THIS is expensive!
    }

    if (!currentHintSequence.empty()) {
        string hint = currentHintSequence.front();
        currentHintSequence.erase(currentHintSequence.begin());
        hintsUsed++;
        return hint;
    }
    return "Cube already solved!";
}
