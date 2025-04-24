#ifndef HINTSYSTEM_H
#define HINTSYSTEM_H

#include "Cube.h"
#include "CubeSolver.h"

class HintSystem {
public:
    HintSystem(Cube* cube, CubeSolver* solver);
    string getNextHint();
    bool canGiveHint() const;
    void resetHints();

private:
    Cube* cube;
    CubeSolver* solver;
    vector<string> currentHintSequence;
    int hintsUsed;
    const int maxHints = 3;
};

#endif
