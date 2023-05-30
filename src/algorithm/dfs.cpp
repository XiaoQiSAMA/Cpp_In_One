#include <iostream>
#include <algorithm>
#include <stack>
using namespace std;

/*
深度遍历迷宫路径(OOP思想)

输入迷宫行列数：5, 5
(0表示可以走,1表示不能走)
0 0 0 1 1
1 0 0 0 1
1 1 0 1 1
1 1 0 0 1
1 1 1 0 0
*/

// 定义迷宫节点的四个方向
const int RIGHT = 0;
const int DOWN = 1;
const int LEFT = 2;
const int UP = 3;

// 每个节点方向的数量
const int WAY_NUM = 4;

// 定义节点行走状态
const int YES = 4;
const int NO = 5;

class Maze {
    public:
        Maze(int row, int col): _row(row), _col(col) {
            _pMaze = new Node* [_row];
            for (int i = 0; i < _row; ++i) {
                _pMaze[i] = new Node[_col];
            }
        }
        // 初始化迷宫路径节点信息
        void initNode(int x, int y, int data) {
            _pMaze[x][y]._x = x;
            _pMaze[x][y]._y = y;
            _pMaze[x][y]._val = data;
            for (int i = 0; i < WAY_NUM; ++i) {
                _pMaze[x][y]._state[i] = NO;
            }
        }

        void setNodeState() {
            for (int i = 0; i < _row; ++i) {
                for (int j = 0; j < _col; ++j) {
                    if (_pMaze[i][j]._val == 1) {
                        continue;
                    }

                    if (j < _col - 1 && _pMaze[i][j + 1]._val == 0) {
                        _pMaze[i][j]._state[RIGHT] = YES;
                    }
                    
                    if (i < _row - 1 && _pMaze[i + 1][j]._val == 0) {
                        _pMaze[i][j]._state[DOWN] = YES;
                    }
                    
                    if (j > 0 && _pMaze[i][j - 1]._val == 0) {
                        _pMaze[i][j]._state[LEFT] = YES;
                    }
                    
                    if (i > 0 && _pMaze[i - 1][j]._val == 0) {
                        _pMaze[i][j]._state[UP] = YES;
                    }
                }
            }
        }

        void searchMazePath() {
            if (_pMaze[0][0]._val == 1) {
                return;
            }
            _stack.push(_pMaze[0][0]);

            while (!_stack.empty()) {
                Node top = _stack.top();
                int x = top._x;
                int y = top._y;
                // 退出条件
                if (x == _row - 1 && y == _col - 1){
                    return;
                }

                if (_pMaze[x][y]._state[RIGHT] == YES) {
                    _pMaze[x][y]._state[RIGHT] = NO;
                    _pMaze[x][y + 1]._state[LEFT] = NO;
                    _stack.push(_pMaze[x][y + 1]);
                    continue;
                }
                if (_pMaze[x][y]._state[LEFT] == YES) {
                    _pMaze[x][y]._state[LEFT] = NO;
                    _pMaze[x][y - 1]._state[RIGHT] = NO;
                    _stack.push(_pMaze[x][y - 1]);
                    continue;
                }
                if (_pMaze[x][y]._state[UP] == YES) {
                    _pMaze[x][y]._state[UP] = NO;
                    _pMaze[x - 1][y]._state[DOWN] = NO;
                    _stack.push(_pMaze[x - 1][y]);
                    continue;
                }
                if (_pMaze[x][y]._state[DOWN] == YES) {
                    _pMaze[x][y]._state[DOWN] = NO;
                    _pMaze[x + 1][y]._state[UP] = NO;
                    _stack.push(_pMaze[x + 1][y]);
                    continue;
                }

                _stack.pop();
            }
        }

        void showMazePath() {
            if (_stack.empty()) {
                cout << "Path is Error!" << endl;
            } else {
                while (!_stack.empty()) {
                    Node top = _stack.top();
                    _pMaze[top._x][top._y]._val = '*';
                    _stack.pop();
                }

                for (int i = 0; i < _row; ++i) {
                    for (int j = 0; j < _col; ++j) {
                        if (_pMaze[i][j]._val == '*'){
                            cout << "* ";
                        } else {
                            cout << _pMaze[i][j]._val << " ";
                        }
                    }
                    cout << endl;
                }
            }
        }
    private:
        // 迷宫节点信息
        struct Node
        {
            int _x;
            int _y;
            int _val;
            int _state[WAY_NUM];
        };

        Node **_pMaze;
        int _row;
        int _col;
        stack<Node> _stack;
        
};

int main() {
    cout << "row & col : ";
    int row, col, data;
    cin >> row >> col;
    
    Maze maze(row, col);

    cout << "Maze's way(0 is YES, 1 is NO): " << endl;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < col; ++j) {
            cin >> data;
            maze.initNode(i, j, data);
        }
    }

    // 初始化行走状态
    maze.setNodeState();

    // serach
    maze.searchMazePath();

    // show path
    maze.showMazePath();

    return 0;
}