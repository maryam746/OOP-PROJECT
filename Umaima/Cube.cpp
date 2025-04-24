#include "Cube.h"
#include <algorithm>
#include <stdexcept>
#include <random>
using namespace std; // Use the std namespace

Cube::Cube() {
    reset();
}

void Cube::reset() {
    faces = {{
        {{{Colour::red, Colour::red, Colour::red}, {Colour::red, Colour::red, Colour::red}, {Colour::red, Colour::red, Colour::red}}},
        {{{Colour::orange, Colour::orange, Colour::orange}, {Colour::orange, Colour::orange, Colour::orange}, {Colour::orange, Colour::orange, Colour::orange}}},
        {{{Colour::green, Colour::green, Colour::green}, {Colour::green, Colour::green, Colour::green}, {Colour::green, Colour::green, Colour::green}}},
        {{{Colour::blue, Colour::blue, Colour::blue}, {Colour::blue, Colour::blue, Colour::blue}, {Colour::blue, Colour::blue, Colour::blue}}},
        {{{Colour::white, Colour::white, Colour::white}, {Colour::white, Colour::white, Colour::white}, {Colour::white, Colour::white, Colour::white}}},
        {{{Colour::yellow, Colour::yellow, Colour::yellow}, {Colour::yellow, Colour::yellow, Colour::yellow}, {Colour::yellow, Colour::yellow, Colour::yellow}}}
    }};
}

bool Cube::isSolved() const {
    for (const auto& face : faces) {
        const auto& firstColour = face[0][0];
        for (const auto& row : face) {
            for (const auto& colour : row) {
                if (colour != firstColour) return false;
            }
        }
    }
    return true;
}

void Cube::rotateFace(Face face, Rotation direction) {
    if (direction == Rotation::CLOCKWISE) {
        rotateFaceClockwise(face);
    } else {
        rotateFaceCounterClockwise(face);
    }
    updateAdjacentFaces(face, direction);
}

void Cube::rotateRow(int row, Rotation direction) {
    if (row < 0 || row > 2) {
        throw invalid_argument("Invalid row index");
    }

    if (direction == Rotation::CLOCKWISE) {
        auto frontRow = getRow(Face::FRONT, row);
        auto rightRow = getRow(Face::RIGHT, row);
        auto backRow = getRow(Face::BACK, row);
        auto leftRow = getRow(Face::LEFT, row);

        setRow(Face::FRONT, row, leftRow);
        setRow(Face::RIGHT, row, frontRow);
        setRow(Face::BACK, row, rightRow);
        setRow(Face::LEFT, row, backRow);
    } else {
        auto frontRow = getRow(Face::FRONT, row);
        auto rightRow = getRow(Face::RIGHT, row);
        auto backRow = getRow(Face::BACK, row);
        auto leftRow = getRow(Face::LEFT, row);

        setRow(Face::FRONT, row, rightRow);
        setRow(Face::RIGHT, row, backRow);
        setRow(Face::BACK, row, leftRow);
        setRow(Face::LEFT, row, frontRow);
    }
}

void Cube::rotateColumn(int col, Rotation direction) {
    if (col < 0 || col > 2) {
        throw invalid_argument("Invalid column index");
    }

    if (direction == Rotation::CLOCKWISE) {
        auto upCol = getCol(Face::UP, col);
        auto frontCol = getCol(Face::FRONT, col);
        auto downCol = getCol(Face::DOWN, col);
        auto backCol = getCol(Face::BACK, 2 - col); // Reverse column for BACK face

        setCol(Face::UP, col, backCol);
        setCol(Face::FRONT, col, upCol);
        setCol(Face::DOWN, col, frontCol);
        setCol(Face::BACK, 2 - col, downCol); // Reverse column for BACK face
    } else {
        auto upCol = getCol(Face::UP, col);
        auto frontCol = getCol(Face::FRONT, col);
        auto downCol = getCol(Face::DOWN, col);
        auto backCol = getCol(Face::BACK, 2 - col); // Reverse column for BACK face

        setCol(Face::UP, col, frontCol);
        setCol(Face::FRONT, col, downCol);
        setCol(Face::DOWN, col, backCol);
        setCol(Face::BACK, 2 - col, upCol); // Reverse column for BACK face
    }
}

void Cube::scramble() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> faceDist(0, 5);
    uniform_int_distribution<> dirDist(0, 1);

    for (int i = 0; i < 20; ++i) {
        Face randomFace = static_cast<Face>(faceDist(gen));
        Rotation randomDir = dirDist(gen) == 0 ? Rotation::CLOCKWISE : Rotation::COUNTER_CLOCKWISE;
        rotateFace(randomFace, randomDir);
    }
}

Cube::Colour Cube::getColour(Face face, int row, int col) const {
    return faces[static_cast<int>(face)][row][col];
}

array<array<Cube::Colour, 3>, 3> Cube::getFace(Face face) const {
    return faces[static_cast<int>(face)];
}

void Cube::rotateFaceClockwise(Face face) {
    auto& f = faces[static_cast<int>(face)];
    swap(f[0][0], f[2][0]);
    swap(f[0][1], f[1][0]);
    swap(f[0][2], f[0][0]);
    swap(f[1][2], f[0][1]);
    swap(f[2][2], f[1][2]);
    swap(f[2][1], f[2][2]);
    swap(f[2][0], f[2][1]);
    swap(f[1][0], f[2][0]);
}

void Cube::rotateFaceCounterClockwise(Face face) {
    rotateFaceClockwise(face);
    rotateFaceClockwise(face);
    rotateFaceClockwise(face);
}

array<Cube::Colour, 3> Cube::getRow(Face face, int row) const {
    return faces[static_cast<int>(face)][row];
}

array<Cube::Colour, 3> Cube::getCol(Face face, int col) const {
    return {faces[static_cast<int>(face)][0][col], 
            faces[static_cast<int>(face)][1][col], 
            faces[static_cast<int>(face)][2][col]};
}

void Cube::setRow(Face face, int row, const array<Colour, 3>& colours) {
    faces[static_cast<int>(face)][row] = colours;
}

void Cube::setCol(Face face, int col, const array<Colour, 3>& colours) {
    for (int i = 0; i < 3; ++i) {
        faces[static_cast<int>(face)][i][col] = colours[i];
    }
}

void Cube::updateAdjacentFaces(Face face, Rotation direction) {
    // Implementation for updating adjacent faces after a face rotation
    // This is already implemented in your provided code.
}
