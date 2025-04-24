#ifndef CUBE_H
#define CUBE_H

#include <array>
#include <string>
using namespace std; // Use the std namespace

class Cube {
public:
    // Enums for faces, rotations, and colours
    enum class Face { FRONT, BACK, LEFT, RIGHT, UP, DOWN };
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

#endif // CUBE_H
