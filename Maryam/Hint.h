#include <thread>
#include <atomic>
#include "Cube.h"
#include "CubeSolver.h"
#include <vector>

class HintSystem {
private:
    Cube* cube;
    CubeSolver* solver;
    int hintsUsed;
    const int maxHints = 3;

    // NEW for threading
    thread solverThread;
    atomic<bool> solvingInProgress;
    atomic<bool> solutionReady;
    string solvingStatus;

public:
    vector<string> currentHintSequence;
    HintSystem(Cube* cube, CubeSolver* solver);
    ~HintSystem();

    bool canGiveHint() const;
    string getNextHint();
    void resetHints();

    void startSolving(); // starts solve in background
    bool isSolving() const;
    bool isSolutionReady() const;
    string getSolvingStatus() const;
};
