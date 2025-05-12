#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <array>
#include <string>
#include <vector>
using namespace std; 

class Cube {
public:
  
    enum class Face {
        FRONT = 0,
        BACK = 1,
        LEFT = 2,
        RIGHT = 3,
        UP = 4,
        DOWN = 5
    };

    enum class Rotation { CLOCKWISE, COUNTER_CLOCKWISE };
    enum class Colour { red, orange, green, blue, white, yellow, black };

   
    Cube();


    void reset();
    bool isSolved() const; 
    void rotateFace(Face face, Rotation direction);
    void rotateRow(int row, Rotation direction); 
    void rotateColumn(int col, Rotation direction); 
    void scramble(); 
    Colour getColour(Face face, int row, int col) const; 
    array<array<Colour, 3>, 3> getFace(Face face) const; 

    array<array<array<Colour, 3>, 3>, 6> faces; 

    void applyMove(const string& move);

   

    void startScrambleSequence();
    bool isScrambling() const { return !scrambleMoves.empty(); }
    string getNextScrambleMove();



private:

    void rotateFaceClockwise(Face face); 
    void rotateFaceCounterClockwise(Face face); 
    void updateAdjacentFaces(Face face, Rotation direction);
    array<Colour, 3> getRow(Face face, int row) const; 
    array<Colour, 3> getCol(Face face, int col) const; 
    void setRow(Face face, int row, const array<Colour, 3>& colours); 
    void setCol(Face face, int col, const array<Colour, 3>& colours); 

    vector<string> scrambleMoves;
};

inline bool operator<(Cube::Colour a, Cube::Colour b) {
    return static_cast<int>(a) < static_cast<int>(b);
}

inline bool operator<(Cube::Face a, Cube::Face b) {
    return static_cast<int>(a) < static_cast<int>(b);
}

inline bool operator<(const Cube& lhs, const Cube& rhs) {
    return lhs.getFace(Cube::Face::FRONT) < rhs.getFace(Cube::Face::FRONT);
}


#endif 
