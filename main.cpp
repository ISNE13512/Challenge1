#include <iostream>
using namespace std;

const int SIZE = 10;

struct Position {
    int row;
    int col;
};

//  10x10 grid
void printGrid(Position frodo, Position exitPos) {
    char grid[SIZE][SIZE];

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            grid[i][j] = '.';
        }
    }

    grid[exitPos.row][exitPos.col] = 'E';
    grid[frodo.row][frodo.col] = 'F';

    cout << endl;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

// a numpad-style direction (1,2,3,4,6,7,8,9) into a row/col change

bool getMoveDelta(int direction, int &dRow, int &dCol) {
    switch (direction) {
        case 8: dRow = -1; dCol =  0; break; // up
        case 2: dRow =  1; dCol =  0; break; // down
        case 4: dRow =  0; dCol = -1; break; // left
        case 6: dRow =  0; dCol =  1; break; // right
        case 7: dRow = -1; dCol = -1; break; // up-left
        case 9: dRow = -1; dCol =  1; break; // up-right
        case 1: dRow =  1; dCol = -1; break; // down-left
        case 3: dRow =  1; dCol =  1; break; // down-right
        default: return false; // invalid direction
    }
    return true;
}

bool isInsideGrid(int row, int col) {
    return (row >= 0 && row < SIZE && col >= 0 && col < SIZE);
}

int main() {
    Position frodo = {0, 0};
    Position exitPos = {9, 9};

    cout << "=== Frodo and the Dark Forest (Version 1) ===" << endl;
    cout << "Reach the Exit (E). Directions: 1,2,3,4,6,7,8,9" << endl;

    while (true) {
        printGrid(frodo, exitPos);

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
    }

    return 0;
}