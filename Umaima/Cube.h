#ifndef CUBE_H
#define CUBE_H

#include <iostream>
#include <array>
#include <string>
using namespace std; // Use the std namespace

class Cube {
public:
    // Enums for faces, rotations, and colours
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

    // Constructor
    Cube();

    // Public methods
    void reset(); // Reset the cube to its solved state
    bool isSolved() const; // Check if the cube is solved
    void rotateFace(Face face, Rotation direction); // Rotate a face clockwise or counterclockwise
    void rotateRow(int row, Rotation direction); // Rotate a specific row
    void rotateColumn(int col, Rotation direction); // Rotate a specific column
    void scramble(); // Scramble the cube with random rotations
    Colour getColour(Face face, int row, int col) const; // Get the colour of a specific tile
    array<array<Colour, 3>, 3> getFace(Face face) const; // Get the colours of a face

private:
    // Private helper methods
    void rotateFaceClockwise(Face face); // Rotate a face clockwise
    void rotateFaceCounterClockwise(Face face); // Rotate a face counterclockwise
    void updateAdjacentFaces(Face face, Rotation direction); // Update adjacent faces after a face rotation
    array<Colour, 3> getRow(Face face, int row) const; // Get a row from a face
    array<Colour, 3> getCol(Face face, int col) const; // Get a column from a face
    void setRow(Face face, int row, const array<Colour, 3>& colours); // Set a row on a face
    void setCol(Face face, int col, const array<Colour, 3>& colours); // Set a column on a face

    // Private member variables
    array<array<array<Colour, 3>, 3>, 6> faces; // 3D array representing the cube's state
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


#endif // CUBE_H
