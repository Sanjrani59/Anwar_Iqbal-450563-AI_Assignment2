//A* Search Algorithm for solving 8 puzzle          (Anwar Iqbal RIME 23)
#include <bits/stdc++.h>
using namespace std;
const int N = 3;
const int maxSteps = 500;   //Feasibility

struct Puzzle {
    vector<vector<int>> board;
    int heuristic;
    int moves;

    Puzzle(const vector<vector<int>>& b, int h, int m) : board(b), heuristic(h), moves(m) {}

    bool operator<(const Puzzle& other) const {
        return (heuristic + moves) > (other.heuristic + other.moves);
    }
};
//manhatten Distance
int calculateManhattanDistance(const vector<vector<int>>& state, const vector<vector<int>>& goal) {
    int distance = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int tile = state[i][j];
            if (tile != 0) {
                int goalRow, goalCol;
                for (int k = 0; k < N; ++k) {
                    for (int l = 0; l < N; ++l) {
                        if (goal[k][l] == tile) {
                            goalRow = k;
                            goalCol = l;
                        }
                    }
                }
                distance += abs(i - goalRow) + abs(j - goalCol);
            }
        }
    }
    return distance;
}

vector<Puzzle> generateSuccessors(const Puzzle& puzzle, const vector<vector<int>>& goal) {
    vector<Puzzle> successors;
    int blankRow, blankCol;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (puzzle.board[i][j] == 0) {
                blankRow = i;
                blankCol = j;
            }
        }
    }

    const int moves[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    for (const auto& move : moves) {
        int newRow = blankRow + move[0];
        int newCol = blankCol + move[1];

        if (newRow >= 0 && newRow < N && newCol >= 0 && newCol < N) {
            vector<vector<int>> newBoard = puzzle.board;
            swap(newBoard[blankRow][blankCol], newBoard[newRow][newCol]);
            int h = calculateManhattanDistance(newBoard, goal);
            int m = puzzle.moves + 1;
            successors.push_back(Puzzle(newBoard, h, m));
        }
    }

    return successors;
}
void printBoard(const vector<vector<int>>& board, int h, int m) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cout << board[i][j] << "(" << h << ", " << m << ") ";
        }
        cout << endl;
    }
}

bool isSolvable(const vector<vector<int>>& initial, const vector<vector<int>>& goal) {
    int invCount = 0;
    vector<int> initialArray(N * N, 0);
    vector<int> goalArray(N * N, 0);

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            initialArray[i * N + j] = initial[i][j];
            goalArray[i * N + j] = goal[i][j];
        }
    }

    for (int i = 0; i < N * N - 1; i++) {
        for (int j = i + 1; j < N * N; j++) {
            if (initialArray[j] && initialArray[i] && initialArray[i] > initialArray[j]) {
                invCount++;
            }
        }
    }

    for (int i = 0; i < N * N - 1; i++) {
        for (int j = i + 1; j < N * N; j++) {
            if (goalArray[j] && goalArray[i] && goalArray[i] > goalArray[j]) {
                invCount++;
            }
        }
    }

    return (invCount % 2 == 0);
}

vector<vector<int>> generateRandomState() {
    vector<int> numbers(N * N, 0);
    iota(numbers.begin(), numbers.end(), 0);
    random_shuffle(numbers.begin(), numbers.end());

    vector<vector<int>> randomState(N, vector<int>(N, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            randomState[i][j] = numbers[i * N + j];
        }
    }

    return randomState;
}

void solvePuzzle(const vector<vector<int>>& initial, const vector<vector<int>>& goal) {
    priority_queue<Puzzle> openList;
    unordered_set<string> visited;
    int h = calculateManhattanDistance(initial, goal);
    Puzzle start(initial, h, 0);
    openList.push(start);

    while (!openList.empty()) {
        Puzzle current = openList.top();
        openList.pop();

        if (current.board == goal) {
            cout << "Goal Achieved!" << current.moves << " moves." << endl;
            printBoard(current.board, current.heuristic, current.moves);
            return;
        }

        string hash = "";
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                hash += to_string(current.board[i][j]);
            }
        }

        if (visited.find(hash) == visited.end()) {
            visited.insert(hash);
            vector<Puzzle> successors = generateSuccessors(current, goal);
            for (const Puzzle& successor : successors) {
                openList.push(successor);
            }
            cout << "Moved to state (heuristic cost: " << current.heuristic << ", cost: " << current.moves << "):" << endl;
            printBoard(current.board, current.heuristic, current.moves);
        }
    }

    cout << "Solution Doesn't Exist!" << endl;
}

int main() {
    vector<vector<int>> initial(N, vector<int>(N));
    vector<vector<int>> goal(N, vector<int>(N));

    cout << "Enter the Initial Matrix [use 0 for the blank space]: " << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> initial[i][j];
        }
    }

    cout << "Enter the Goal Matrix [use 0 for the blank space]: " << endl;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> goal[i][j];
        }
    }

    solvePuzzle(initial, goal);

    return 0;
}
