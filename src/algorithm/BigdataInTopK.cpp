#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <functional>
using namespace std;

/*
数据重复次数最大/最小的前K个/第K个
哈希统计(map) + 堆/快排分割

限制: 内存限制
*/

int main() {
    vector<int> vec;
    for (int i = 0; i < 200000; ++i) {
        vec.push_back(rand());
    }

    // 统计所有数字的的重复次数
    unordered_map<int, int> numMap;
    for (int val : vec) {
        numMap[val]++;
    }

    // 小根堆定义
    using P = pair<int, int>;
    using FUNC = function<bool(P&, P&)>;
    using MinMap = priority_queue<P, vector<P>, FUNC>;
    MinMap minheap([](auto &a, auto &b) -> bool {
        return a.second > b.second;     // 自定义小根堆元素大小比较方式
    });
    
    // 求top-k
    int k = 0;
    auto it = numMap.begin();

    // 取10个放入小根堆中
    for (; it != numMap.end() && k < 10; ++it, ++k) {
        minheap.push(*it);
    }
    // 遍历剩下的元素，与堆顶元素比较
    for (; it != numMap.end(); ++it) {
        if (it->second > minheap.top().second) {
            minheap.pop();
            minheap.push(*it);
        }
    }

    while (!minheap.empty()) {
        auto &pair = minheap.top();
        cout << pair.first << " : " << pair.second << endl;
        minheap.pop();
    }

    return 0;
}