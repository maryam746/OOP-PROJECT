#include "Hint.h"

HintSystem::HintSystem(Cube* cube, CubeSolver* solver)
    : cube(cube), solver(solver), hintsUsed(0) {}

bool HintSystem::canGiveHint() const {
    return hintsUsed < maxHints;
}

string HintSystem::getNextHint() {
    if (!canGiveHint()) return "Max hints used.";

    currentHintSequence = solver->solve(*cube);
    if (!currentHintSequence.empty()) {
        string hint = currentHintSequence.front();
        currentHintSequence.erase(currentHintSequence.begin());
        hintsUsed++;
        return hint;
    }
    return "Cube already solved!";
}

void HintSystem::resetHints() {
    hintsUsed = 0;
    currentHintSequence.clear();
}
