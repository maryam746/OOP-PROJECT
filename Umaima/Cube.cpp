#include "Cube.h"
#include <algorithm>
#include <stdexcept>
#include <random>
#include <iostream>

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
    try {
        if (row < 0 || row > 2) {
            throw invalid_argument("Invalid row index");
        }

        auto frontRow = getRow(Face::FRONT, row);
        auto rightRow = getRow(Face::RIGHT, row);
        auto backRow = getRow(Face::BACK, row);
        auto leftRow = getRow(Face::LEFT, row);

        if (direction == Rotation::CLOCKWISE) {
            setRow(Face::FRONT, row, leftRow);
            setRow(Face::RIGHT, row, frontRow);
            setRow(Face::BACK, row, rightRow);
            setRow(Face::LEFT, row, backRow);
        }
        else {
            setRow(Face::FRONT, row, rightRow);
            setRow(Face::RIGHT, row, backRow);
            setRow(Face::BACK, row, leftRow);
            setRow(Face::LEFT, row, frontRow);
        }

        // Rotate UP or DOWN face if needed
        if (row == 0) {
            rotateFace(Face::UP, direction);
        }
        else if (row == 2) {
            rotateFace(Face::DOWN, direction == Rotation::CLOCKWISE ? Rotation::COUNTER_CLOCKWISE : Rotation::CLOCKWISE);
        }

    }
    catch (const invalid_argument& e) {
        cerr << "Error in rotateRow: " << e.what() << endl;
    }
}
void Cube::rotateColumn(int col, Rotation direction) {
    try {
        if (col < 0 || col > 2) {
            throw invalid_argument("Invalid column index");
        }

        auto upCol = getCol(Face::UP, col);
        auto frontCol = getCol(Face::FRONT, col);
        auto downCol = getCol(Face::DOWN, col);
        auto backCol = getCol(Face::BACK, 2 - col); // BACK is mirrored

        if (direction == Rotation::CLOCKWISE) {
            setCol(Face::UP, col, backCol);
            setCol(Face::FRONT, col, upCol);
            setCol(Face::DOWN, col, frontCol);
            setCol(Face::BACK, 2 - col, downCol);
        }
        else {
            setCol(Face::UP, col, frontCol);
            setCol(Face::FRONT, col, downCol);
            setCol(Face::DOWN, col, backCol);
            setCol(Face::BACK, 2 - col, upCol);
        }

        // Rotate LEFT or RIGHT face if needed
        if (col == 0) {
            rotateFace(Face::LEFT, direction == Rotation::CLOCKWISE ? Rotation::COUNTER_CLOCKWISE : Rotation::CLOCKWISE);
        }
        else if (col == 2) {
            rotateFace(Face::RIGHT, direction);
        }

    }
    catch (const invalid_argument& e) {
        cerr << "Error in rotateColumn: " << e.what() << endl;
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
    if (row < 0 || row > 2 || col < 0 || col > 2)
        throw out_of_range("getColour index out of bounds");

    return faces[static_cast<int>(face)][row][col];
}




array<array<Cube::Colour, 3>, 3> Cube::getFace(Face face) const {
    return faces[static_cast<int>(face)];
}




void Cube::rotateFaceClockwise(Face face) {
    auto& f = faces[static_cast<int>(face)];
    Colour temp = f[0][0];
    f[0][0] = f[2][0];
    f[2][0] = f[2][2];
    f[2][2] = f[0][2];
    f[0][2] = temp;

    temp = f[0][1];
    f[0][1] = f[1][0];
    f[1][0] = f[2][1];
    f[2][1] = f[1][2];
    f[1][2] = temp;
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
    array<array<Colour, 3>, 4> rows = {};

    auto rotate = [&](array<Colour, 3>& a, array<Colour, 3>& b, array<Colour, 3>& c, array<Colour, 3>& d) {
        if (direction == Rotation::CLOCKWISE) {
            swap(a, b);
            swap(a, c);
            swap(a, d);
        }
        else {
            swap(a, d);
            swap(a, c);
            swap(a, b);
        }
        };

    switch (face) {
    case Face::UP:
        for (int i = 0; i < 3; ++i) {
            rows[0][i] = faces[static_cast<int>(Face::BACK)][0][static_cast<size_t>(2) - static_cast<size_t>(i)];
            rows[1][i] = faces[static_cast<int>(Face::RIGHT)][0][i];
            rows[2][i] = faces[static_cast<int>(Face::FRONT)][0][i];
            rows[3][i] = faces[static_cast<int>(Face::LEFT)][0][static_cast<size_t>(2) - static_cast<size_t>(i)];
        }
        rotate(rows[0], rows[1], rows[2], rows[3]);
        for (int i = 0; i < 3; ++i) {
            faces[static_cast<int>(Face::BACK)][0][static_cast<size_t>(2) - static_cast<size_t>(i)] = rows[0][i];
            faces[static_cast<int>(Face::RIGHT)][0][i] = rows[1][i];
            faces[static_cast<int>(Face::FRONT)][0][i] = rows[2][i];
            faces[static_cast<int>(Face::LEFT)][0][static_cast<size_t>(2) - static_cast<size_t>(i)] = rows[3][i];
        }
        break;

    case Face::DOWN:
        for (int i = 0; i < 3; ++i) {
            rows[0][i] = faces[static_cast<int>(Face::BACK)][2][static_cast<size_t>(2) - static_cast<size_t>(i)];
            rows[1][i] = faces[static_cast<int>(Face::LEFT)][2][static_cast<size_t>(2) - static_cast<size_t>(i)];
            rows[2][i] = faces[static_cast<int>(Face::FRONT)][2][i];
            rows[3][i] = faces[static_cast<int>(Face::RIGHT)][2][i];
        }
        rotate(rows[0], rows[1], rows[2], rows[3]);
        for (int i = 0; i < 3; ++i) {
            faces[static_cast<int>(Face::BACK)][2][static_cast<size_t>(2) - static_cast<size_t>(i)] = rows[0][i];
            faces[static_cast<int>(Face::LEFT)][2][static_cast<size_t>(2) - static_cast<size_t>(i)] = rows[1][i];
            faces[static_cast<int>(Face::FRONT)][2][i] = rows[2][i];
            faces[static_cast<int>(Face::RIGHT)][2][i] = rows[3][i];
        }
        break;

    case Face::FRONT:
        for (int i = 0; i < 3; ++i) {
            rows[0][i] = faces[static_cast<int>(Face::UP)][2][i];
            rows[1][i] = faces[static_cast<int>(Face::RIGHT)][i][0];
            rows[2][i] = faces[static_cast<int>(Face::DOWN)][0][static_cast<size_t>(2) - static_cast<size_t>(i)];
            rows[3][i] = faces[static_cast<int>(Face::LEFT)][static_cast<size_t>(2) - static_cast<size_t>(i)][2];
        }
        rotate(rows[0], rows[1], rows[2], rows[3]);
        for (int i = 0; i < 3; ++i) {
            faces[static_cast<int>(Face::UP)][2][i] = rows[0][i];
            faces[static_cast<int>(Face::RIGHT)][i][0] = rows[1][i];
            faces[static_cast<int>(Face::DOWN)][0][static_cast<size_t>(2) - static_cast<size_t>(i)] = rows[2][i];
            faces[static_cast<int>(Face::LEFT)][static_cast<size_t>(2) - static_cast<size_t>(i)][2] = rows[3][i];
        }
        break;

    case Face::BACK:
        for (int i = 0; i < 3; ++i) {
            rows[0][i] = faces[static_cast<int>(Face::UP)][0][static_cast<size_t>(2) - static_cast<size_t>(i)];
            rows[1][i] = faces[static_cast<int>(Face::LEFT)][i][0];
            rows[2][i] = faces[static_cast<int>(Face::DOWN)][2][i];
            rows[3][i] = faces[static_cast<int>(Face::RIGHT)][static_cast<size_t>(2) - static_cast<size_t>(i)][2];
        }
        rotate(rows[0], rows[1], rows[2], rows[3]);
        for (int i = 0; i < 3; ++i) {
            faces[static_cast<int>(Face::UP)][0][static_cast<size_t>(2) - static_cast<size_t>(i)] = rows[0][i];
            faces[static_cast<int>(Face::LEFT)][i][0] = rows[1][i];
            faces[static_cast<int>(Face::DOWN)][2][i] = rows[2][i];
            faces[static_cast<int>(Face::RIGHT)][static_cast<size_t>(2) - static_cast<size_t>(i)][2] = rows[3][i];
        }
        break;

    case Face::LEFT:
        for (int i = 0; i < 3; ++i) {
            rows[0][i] = faces[static_cast<int>(Face::UP)][i][0];
            rows[1][i] = faces[static_cast<int>(Face::FRONT)][i][0];
            rows[2][i] = faces[static_cast<int>(Face::DOWN)][i][0];
            rows[3][i] = faces[static_cast<int>(Face::BACK)][static_cast<size_t>(2) - static_cast<size_t>(i)][2];
        }
        rotate(rows[0], rows[1], rows[2], rows[3]);
        for (int i = 0; i < 3; ++i) {
            faces[static_cast<int>(Face::UP)][i][0] = rows[0][i];
            faces[static_cast<int>(Face::FRONT)][i][0] = rows[1][i];
            faces[static_cast<int>(Face::DOWN)][i][0] = rows[2][i];
            faces[static_cast<int>(Face::BACK)][static_cast<size_t>(2) - static_cast<size_t>(i)][2] = rows[3][i];
        }
        break;

    case Face::RIGHT:
        for (int i = 0; i < 3; ++i) {
            rows[0][i] = faces[static_cast<int>(Face::UP)][i][2];
            rows[1][i] = faces[static_cast<int>(Face::BACK)][static_cast<size_t>(2) - static_cast<size_t>(i)][0];
            rows[2][i] = faces[static_cast<int>(Face::DOWN)][i][2];
            rows[3][i] = faces[static_cast<int>(Face::FRONT)][i][2];
        }
        rotate(rows[0], rows[1], rows[2], rows[3]);
        for (int i = 0; i < 3; ++i) {
            faces[static_cast<int>(Face::UP)][i][2] = rows[0][i];
            faces[static_cast<int>(Face::BACK)][static_cast<size_t>(2) - static_cast<size_t>(i)][0] = rows[1][i];
            faces[static_cast<int>(Face::DOWN)][i][2] = rows[2][i];
            faces[static_cast<int>(Face::FRONT)][i][2] = rows[3][i];
        }
        break;
    }
}


void Cube::applyMove(const string& move) {
    Rotation direction = Rotation::CLOCKWISE;
    int times = 1;

    string baseMove = move;
    if (move.size() > 1) {
        if (move[1] == '\'') {
            direction = Rotation::COUNTER_CLOCKWISE;
            baseMove = move[0];
        }
        else if (move[1] == '2') {
            times = 2;
            baseMove = move[0];
        }
    }

    Face face;
    if (baseMove == "R") face = Face::RIGHT;
    else if (baseMove == "L") face = Face::LEFT;
    else if (baseMove == "U") face = Face::UP;
    else if (baseMove == "D") face = Face::DOWN;
    else if (baseMove == "F") face = Face::FRONT;
    else if (baseMove == "B") face = Face::BACK;
    else {
        cerr << "Invalid move: " << move << endl;
        return;
    }

    for (int i = 0; i < times; ++i) {
        rotateFace(face, direction);
    }
}
