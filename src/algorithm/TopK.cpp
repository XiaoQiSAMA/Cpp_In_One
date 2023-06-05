#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

/*
海量数据求top k的问题
1. 求最大/最小的前K个元素
1. 求最大/最小的第K个元素

1. 大根堆/小根堆 ==> 优先级队列
    : K个元素构建成堆，依次比较堆顶元素进行淘汰
2. 快排分割函数
    : 在O(logn)内，把小于基准数的整数调整到左边，大于基准数的调整到右边。
*/

int partation(vector<int> arr, int i, int j) {
    int k = arr[i];
    while (i < j) {
        while (i < j && arr[j] > k)
            --j;
        if (i < j)
            arr[i++] = arr[j--];

        while (i < j && arr[i] < k)
            ++i;
        if (i < j)
            arr[j--] = arr[i++];
    }
    arr[i] = k;
    return i;
} 

int selectNoK(vector<int> arr, int i, int j, int k) {
    int pos = partation(arr, i, j);
    if (pos == k) 
        return pos;
    else if (pos < k - 1)
    {
        return selectNoK(arr, pos + 1, j, k);
    } else {
        return selectNoK(arr, i, pos - 1, k);
    }
}

int main() {
    vector<int> vec;
    for (int i = 0; i < 10000; ++i) {
        vec.push_back(rand() + i);
    }

    int pos = selectNoK(vec, 0, vec.size() - 1, 10);
    cout << vec[pos] << endl;
    return 0;
}


# if 0
int main() {
    vector<int> vec;
    for (int i = 0; i < 10000; ++i) {
        vec.push_back(rand() + i);
    }

    // 定义小根堆
    priority_queue<int, vector<int>, greater<int>> minHeap;
    // 大根堆
    // priority_queue<int, vector<int>> minHeap;

    // 前十大的数
    int k = 10;
    for (int i = 0; i < k; ++i) {
        minHeap.push(vec[i]);
    }

    /*
    vec中剩下的数和minHeap的堆顶元素对比，比堆顶元素大的就删除堆顶，把当前元素添加到小根堆中。以此遍历整个vec
    */
    for (; k < vec.size(); ++k) {
        if (vec[k] > minHeap.top()) {
            minHeap.pop();
            minHeap.push(vec[k]);
        }
    }

    while (!minHeap.empty()) {
        cout << minHeap.top() << " ";
        minHeap.pop();
    }

    return 0;
}
# endif