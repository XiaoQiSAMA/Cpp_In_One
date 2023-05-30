#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

/*
广搜迷宫最短路径(OOP思想)
队列思想：记录节点和记录路径信息

输入迷宫行列数：6， 6
(0表示可以走,1表示不能走)
0 0 1 1 1 1
1 0 0 0 0 1
1 0 1 1 0 1
1 0 0 0 0 1
1 0 1 1 1 1
1 0 0 0 0 0
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
            _pPath.resize(_row * _col);
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
            _queue.push(_pMaze[0][0]);

            while (!_queue.empty()) {
                Node top = _queue.front();
                int x = top._x;
                int y = top._y;
                // // 退出条件
                // if (x == _row - 1 && y == _col - 1){
                //     return;
                // }

                if (_pMaze[x][y]._state[RIGHT] == YES) {
                    _pMaze[x][y]._state[RIGHT] = NO;
                    _pMaze[x][y + 1]._state[LEFT] = NO;
                    // 辅助数组中记录节点行走路径
                    _pPath[x * _row + y + 1] = _pMaze[x][y];
                    _queue.push(_pMaze[x][y + 1]);
                    if (check(_pMaze[x][y + 1]))
                        return;
                }
                if (_pMaze[x][y]._state[LEFT] == YES) {
                    _pMaze[x][y]._state[LEFT] = NO;
                    _pMaze[x][y - 1]._state[RIGHT] = NO;
                    // 辅助数组中记录节点行走路径
                    _pPath[x * _row + y - 1] = _pMaze[x][y];
                    _queue.push(_pMaze[x][y - 1]);
                    if (check(_pMaze[x][y - 1]))
                        return;
                }
                if (_pMaze[x][y]._state[UP] == YES) {
                    _pMaze[x][y]._state[UP] = NO;
                    _pMaze[x - 1][y]._state[DOWN] = NO;
                    // 辅助数组中记录节点行走路径
                    _pPath[(x - 1) * _row + y] = _pMaze[x][y];
                    _queue.push(_pMaze[x - 1][y]);
                    if (check(_pMaze[x - 1][y]))
                        return;
                }
                if (_pMaze[x][y]._state[DOWN] == YES) {
                    _pMaze[x][y]._state[DOWN] = NO;
                    _pMaze[x + 1][y]._state[UP] = NO;
                    // 辅助数组中记录节点行走路径
                    _pPath[(x + 1) * _row + y] = _pMaze[x][y];
                    _queue.push(_pMaze[x + 1][y]);
                    if (check(_pMaze[x + 1][y]))
                        return;
                }

                _queue.pop();
            }
        }

        void showMazePath() {
            if (_queue.empty()) {
                cout << "Path is Error!" << endl;
            } else {
                // 回溯寻找迷宫路径节点
                int x = _row - 1;
                int y = _col - 1;
                for (;;) {
                    _pMaze[x][y]._val = '*';
                    if (x == 0 && y == 0) 
                        break;
                    Node node = _pPath[x * _row + y];
                    x = node._x;
                    y = node._y;
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

        bool check(Node &node) {
            return node._x == _row - 1 && node._y == _col - 1;
        }

        Node **_pMaze;
        int _row;
        int _col;
        queue<Node> _queue;     // 广度遍历依赖的队列结构
        vector<Node> _pPath;    // 记录广度优先遍历时，节点的行走信息
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