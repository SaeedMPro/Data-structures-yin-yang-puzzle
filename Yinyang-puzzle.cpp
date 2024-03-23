#include <iostream>
#include <vector>
#include <queue>

// Define the colors
#define WHITE 0
#define BLACK 1

int count;

// Function to get the color of a circle at a given position
unsigned int getColor(unsigned int state, int pos) {
    return (state >> (pos + 16)) & 1;
}

// Function to set the color of a circle at a given position
unsigned int setColor(unsigned int state, int pos, int color) {
    if (color == BLACK) {
        return state | (1 << (pos + 16));
    } else {
        return state & ~(1 << (pos + 16));
    }
}

// Function to check the existence of a circle at a given position
bool exists(unsigned int state, int pos) {
    return (state >> pos) & 1;
}

// Function to set the existence of a circle at a given position
unsigned int setExistence(unsigned int state, int pos, bool existence) {
    if (existence) {
        return state | (1 << pos);
    } else {
        return state & ~(1 << pos);
    }
}

// Function to perform a depth-first search on the board
void dfsConnectivity(unsigned int state, int pos, int color, std::vector<bool>& visited) {
    int row = pos / 4;
    int col = pos % 4;

    // Directions for the neighboring cells (up, down, left, right)
    int dr[4] = {-1, 1, 0, 0};
    int dc[4] = {0, 0, -1, 1};

    // Mark the current cell as visited
    visited[pos] = true;

    // Visit all unvisited neighboring cells of the same color
    for (int i = 0; i < 4; i++) {
        int newRow = row + dr[i];
        int newCol = col + dc[i];
        int newPos = newRow * 4 + newCol;
        if (newRow >= 0 && newRow < 4 && newCol >= 0 && newCol < 4 && !visited[newPos] && (getColor(state, newPos) == color || !exists(state, newPos))) {
            dfsConnectivity(state, newPos, color, visited);
        }
    }
}

// Function to check if a state is valid according to the rules of the Yin Yang puzzle
bool isValid(unsigned int state, int pos, bool isBFS) {
    // Rule 1: Each cell should be filled with either a black or white circle
   // for (int i = 0; i < 16; i++) {
        //if (!exists(state, i)) {
         //   return false;
      //  }
    //}

    // Rule 2: All white circles and all black circles should be orthogonally connected
    for (int color = WHITE; color <= BLACK; color++) {
        std::vector<bool> visited(16, false);
        for (int i = 0; i < 16; i++) {
            if (getColor(state, i) == color) {
                dfsConnectivity(state, i, color, visited);
                break;
            }
        }
        for (int i = 0; i < 16; i++) {
            if (exists(state, i)) {
                if ((getColor(state, i) == color) && !visited[i]) {
                    return false;
                }
            }
        }
    }

    // Rule 3: There may not be any 2x2 cell region consisting of the same circle color
    int row = pos / 4;
    int col = pos % 4;
    for (int i = -1; i <= 0; i++) {
        for (int j = -1; j <= 0; j++) {
            int newRow = row + i;
            int newCol = col + j;
            if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3) {
                if (exists(state, newRow * 4 + newCol) && exists(state, newRow * 4 + newCol + 1) && exists(state, (newRow + 1) * 4 + newCol) && exists(state, (newRow + 1) * 4 + newCol + 1) &&
                    getColor(state, newRow * 4 + newCol) == getColor(state, newRow * 4 + newCol + 1) &&
                    getColor(state, newRow * 4 + newCol) == getColor(state, (newRow + 1) * 4 + newCol) &&
                    getColor(state, newRow * 4 + newCol) == getColor(state, (newRow + 1) * 4 + newCol + 1)) {
                    return false;
                }
            }
        }
    }

    return true;
}

std::string printState(unsigned int state) {
    std::string result;
    for (int i = 0; i < 16; i++) {
        if (exists(state, i)) {
            if (getColor(state, i) == BLACK) {
                result += 'B';
            } else {
                result += 'W';
            }
        } else {
            result += ' ';  // No circle at this position
        }
        if (i % 4 == 3) {
            result += '\n';  // New line every 4 characters for a 4x4 grid
        }
    }
    return result;
}

// DFS function
void dfs(unsigned int state, int pos) {
    // Base case: if pos is out of bounds, return
    if (pos == 16) {
        //if (isValid(state, pos, false)) {
            std::cout << "Found a valid state:\n" << printState(state) << std::endl;
            count++;
       // }
        return;
    }

    // Generate the next states
    for (int color = WHITE; color <= BLACK; color++) {
        unsigned int newState = setExistence(setColor(state, pos, color), pos, true);
        if (isValid(newState, pos, false)) {
            dfs(newState, pos + 1);
        }
    }
}

void bfs(unsigned int initialState) {
    std::queue<std::pair<unsigned int, int>> q;
    q.push({initialState, 0});

    while (!q.empty()) {
        unsigned int state = q.front().first;
        int pos = q.front().second;
        q.pop();

        if (pos == 16) {
            std::cout << "Found a valid state:\n" << printState(state) << std::endl;
            count++;
        }

        if (pos < 16) {
            for (int color = WHITE; color <= BLACK; color++) {
                unsigned int newState = setExistence(setColor(state, pos, color), pos, true);
                if (isValid(newState, pos, true)) {
                    q.push({newState, pos + 1});
                }
            }
        }
    }
}


int main() {
    unsigned int initialState = 0;  // Initial state with no circles
    count = 0;
    bfs(initialState);
    std::cout << count << std::endl;

    return 0;
}
