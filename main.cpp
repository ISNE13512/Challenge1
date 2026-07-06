#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

const int SIZE = 10;
const int NUM_RIDERS = 3;

struct Position {
    int row;
    int col;
};
//  10x10 grid

void printGrid(Position frodo, Position exitPos, vector<Position> &riders) {
    char grid[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            grid[i][j] = '.';

    grid[exitPos.row][exitPos.col] = 'E';
    for (int i = 0; i < (int)riders.size(); i++)
        grid[riders[i].row][riders[i].col] = 'R';
    grid[frodo.row][frodo.col] = 'F'; // drawn last so we can always see Frodo

    cout << endl;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++)
            cout << grid[i][j] << " ";
        cout << endl;
    }
    cout << endl;
}

// a numpad-style direction (1,2,3,4,6,7,8,9) into a row/col change
bool getMoveDelta(int direction, int &dRow, int &dCol) {
    switch (direction) {
        case 8: dRow = -1; dCol =  0; break;
        case 2: dRow =  1; dCol =  0; break;
        case 4: dRow =  0; dCol = -1; break;
        case 6: dRow =  0; dCol =  1; break;
        case 7: dRow = -1; dCol = -1; break;
        case 9: dRow = -1; dCol =  1; break;
        case 1: dRow =  1; dCol = -1; break;
        case 3: dRow =  1; dCol =  1; break;
        default: return false;
    }
    return true;
}

bool isInsideGrid(int row, int col) {
    return (row >= 0 && row < SIZE && col >= 0 && col < SIZE);
}

// riders "see" the 8 squares around them (i.e. within 1 square, including diagonals)
bool isAdjacent(Position a, Position b) {
    int rowDiff = abs(a.row - b.row);
    int colDiff = abs(a.col - b.col);
    return (rowDiff <= 1 && colDiff <= 1);
}

Position randomEmptyPosition(Position frodo, Position exitPos, vector<Position> &riders) {
    Position p;
    bool ok;
    do {
        p.row = rand() % SIZE;
        p.col = rand() % SIZE;
        ok = true;

        if (p.row == frodo.row && p.col == frodo.col) ok = false;
        if (p.row == exitPos.row && p.col == exitPos.col) ok = false;
        for (int i = 0; i < (int)riders.size(); i++)
            if (riders[i].row == p.row && riders[i].col == p.col) ok = false;

    } while (!ok);
    return p;
}

// returns true if a Rider catches Frodo this turn
bool moveRiders(vector<Position> &riders, Position frodo) {
    for (int i = 0; i < (int)riders.size(); i++) {
        if (isAdjacent(riders[i], frodo)) {
            riders[i] = frodo; // rider moves onto Frodo's square -> caught
            return true;
        }
    }

    // no rider saw Frodo -> each rider takes one random step
    for (int i = 0; i < (int)riders.size(); i++) {
        int dRow = (rand() % 3) - 1; // -1, 0, or 1
        int dCol = (rand() % 3) - 1;
        int newRow = riders[i].row + dRow;
        int newCol = riders[i].col + dCol;
        if (isInsideGrid(newRow, newCol)) {
            riders[i].row = newRow;
            riders[i].col = newCol;
        }
    }
    return false;
}

int main() {
    srand(time(0));

    Position frodo = {0, 0};
    Position exitPos = {9, 9};
    vector<Position> riders;

    for (int i = 0; i < NUM_RIDERS; i++)
        riders.push_back(randomEmptyPosition(frodo, exitPos, riders));

    cout << "=== Frodo and the Dark Forest (Version 2) ===" << endl;
    cout << "Reach the Exit (E) and avoid the Dark Riders (R)!" << endl;
    cout << "Directions: 1,2,3,4,6,7,8,9" << endl;

    while (true) {
        printGrid(frodo, exitPos, riders);

        if (frodo.row == exitPos.row && frodo.col == exitPos.col) {
            cout << "You reached the Exit! You win!" << endl;
            break;
        }

        cout << "Move (1,2,3,4,6,7,8,9): ";
        int choice;
        cin >> choice;

        int dRow, dCol;
        if (!getMoveDelta(choice, dRow, dCol)) {
            cout << "Invalid direction, try again." << endl;
            continue;
        }

        int newRow = frodo.row + dRow;
        int newCol = frodo.col + dCol;

        if (!isInsideGrid(newRow, newCol)) {
            cout << "You can't leave the forest that way!" << endl;
            continue;
        }

        frodo.row = newRow;
        frodo.col = newCol;

        if (moveRiders(riders, frodo)) {
            printGrid(frodo, exitPos, riders);
            cout << "A Dark Rider has caught you! Game Over." << endl;
            break;
        }
    }

    return 0;
}