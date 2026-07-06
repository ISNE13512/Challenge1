//Challenge: 1 
//ISNE13512 Aung Khant Min Myat

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

const int SIZE = 10;
const int TOTAL_LEVELS = 10;
const int RIDERS_PER_LEVEL = 3; // level n has 3*n riders

struct Position {
    int row;
    int col;
};
// 10x10 grid

void printGrid(Position frodo, Position exitPos, vector<Position> &riders, bool invisible) {
    char grid[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            grid[i][j] = '.';

    grid[exitPos.row][exitPos.col] = 'E';
    for (int i = 0; i < (int)riders.size(); i++)
        grid[riders[i].row][riders[i].col] = 'R';

    // lowercase 'f' just tells the player (on their own screen) that
    // Frodo is currently invisible thanks to the Ring
    grid[frodo.row][frodo.col] = invisible ? 'f' : 'F';

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

bool isAdjacent(Position a, Position b) {
    int rowDiff = abs(a.row - b.row);
    int colDiff = abs(a.col - b.col);
    return (rowDiff <= 1 && colDiff <= 1);
}

// riders "see" the 8 squares around them (i.e. within 1 square, including diagonals)
Position randomEmptyPosition(Position frodo, Position exitPos, vector<Position> &riders) {
    Position p;
    bool ok;
    do {
        p.row = rand() % SIZE;
        p.col = rand() % SIZE;
        ok = true;

        if (p.row == frodo.row && p.col == frodo.col) ok = false;
        if (p.row == exitPos.row && p.col == exitPos.col) ok = false;
        if (isAdjacent(p, frodo)) ok = false; // don't spawn right next to Frodo
        for (int i = 0; i < (int)riders.size(); i++)
            if (riders[i].row == p.row && riders[i].col == p.col) ok = false;

    } while (!ok);
    return p;
}

// returns true if Frodo gets caught this turn
bool moveRiders(vector<Position> &riders, Position frodo, bool frodoInvisible) {
    if (frodoInvisible) return false; // riders are frozen and can't sense Frodo

    for (int i = 0; i < (int)riders.size(); i++) {
        if (isAdjacent(riders[i], frodo)) {
            riders[i] = frodo;
            return true;
        }
    }

    for (int i = 0; i < (int)riders.size(); i++) {
        int dRow = (rand() % 3) - 1;
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

// returns 1 if Frodo reaches the exit, 2 if he is caught
int playLevel(int levelNum) {
    Position frodo = {0, 0};
    Position exitPos = {SIZE - 1, SIZE - 1};
    vector<Position> riders;

    int numRiders = RIDERS_PER_LEVEL * levelNum;
    for (int i = 0; i < numRiders; i++)
        riders.push_back(randomEmptyPosition(frodo, exitPos, riders));

    bool ringAvailable = true;

    cout << "\n----- LEVEL " << levelNum << " -----" << endl;
    cout << "Riders this level: " << numRiders << endl;
    cout << "Directions: 1,2,3,4,6,7,8,9   Ring: 0 (once per level)" << endl;

    while (true) {
        printGrid(frodo, exitPos, riders, false);

        if (frodo.row == exitPos.row && frodo.col == exitPos.col)
            return 1; // level cleared

        cout << "Move (1,2,3,4,6,7,8,9) or 0 for the Ring: ";
        int choice;
        cin >> choice;

        if (choice == 0) {
            if (!ringAvailable) {
                cout << "You already used the Ring this level!" << endl;
                continue;
            }
            ringAvailable = false;
            cout << "Frodo puts on the Ring and becomes invisible!" << endl;

            // 3 free moves while the Riders are frozen
            for (int step = 0; step < 3; step++) {
                printGrid(frodo, exitPos, riders, true);

                if (frodo.row == exitPos.row && frodo.col == exitPos.col)
                    return 1;

                cout << "(Ring move " << step + 1 << "/3) Move (1,2,3,4,6,7,8,9): ";
                int ringChoice;
                cin >> ringChoice;

                int dRow, dCol;
                if (!getMoveDelta(ringChoice, dRow, dCol)) {
                    cout << "Invalid direction, that move is wasted." << endl;
                    continue;
                }

                int newRow = frodo.row + dRow;
                int newCol = frodo.col + dCol;
                if (isInsideGrid(newRow, newCol)) {
                    frodo.row = newRow;
                    frodo.col = newCol;
                } else {
                    cout << "You can't leave the forest that way!" << endl;
                }
            }
            continue; // back to normal play after the 3 ring moves
        }

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

        if (moveRiders(riders, frodo, false)) {
            printGrid(frodo, exitPos, riders, false);
            return 2; // caught
        }
    }
}

int main() {
    srand(time(0));

    cout << "=== Frodo and the Lord of the Rings Game ===" << endl;
    cout << "Escape the Dark Forest, avoid the Riders, and clear all "
         << TOTAL_LEVELS << " levels!" << endl;

    for (int level = 1; level <= TOTAL_LEVELS; level++) {
        int result = playLevel(level);
        if (result == 2) {
            cout << "\nA Dark Rider has caught Frodo! Game Over at level "
                 << level << "." << endl;
            return 0;
        }
        cout << "Level " << level << " complete!" << endl;
    }

    cout << "\nFrodo escaped the Dark Forest completely. YOU WIN!" << endl;
    return 0;
}