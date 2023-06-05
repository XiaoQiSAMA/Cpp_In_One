#include <iostream>
#include <unordered_map>
using namespace std;

/*
海量数据的求TOP K和查重/去重的问题
1. 哈希表  50亿(5G)个整数(20G+20G=40G)
2. 分支思想
3. Bloom Filter : 布隆过滤器
4.(高频) 字符串类型   TrieTree字典树(前缀树) 
*/

int main() {
    // #3 a,b两个文件，里面都有10亿个整数，内存限制400M，求出a,b两个文件中重复的元素有哪些
    /*
    把a和b两个大文件，划分成个数相等的一系列小文件(27)
    a1.txt b1.txt
    a2.txt b2.txt
    ...
    a3.txt b3.txt

    从a文件中读取数据，通过 data % 27 = file_index
    从b文件中读取数据，通过 data % 27 = file_index
    
    a和b两个文件中，数据相同的元素进行哈希映射后，必定在相同序号的小文件中

    */


    // #2 50亿个整数，内存限制400M
    /*
    分支思想: 大文件划分成小文件，使得每一个小文件能够加载到内存中，\
    求出对应的重复元素，把结果写入到存储重复元素的文件中
    大文件 ==> 小文件个数(40G / 400M = 120)
    data0.txt
    data2.txt
    ...
    data126.txt
    遍历大文件的元素，把每一个元素根据哈希映射函数，放到对应序号的小文件中
    data % 127 = file_index
    */ 

    # if 0 // #1 数据查重基本思想: 哈希表
    const int SIZE = 10000;
    int ar[SIZE] = {0};
    for (int i = 0; i < SIZE; ++i) {
        ar[i] = rand() % 100;
    }

    unordered_map<int, int> map;
    for (int val : ar) {
        map[val]++;
    }

    for (auto pair : map) {
        if (pair.second > 1) {
            cout << "Num : " << pair.first << " Repate: " << pair.second << endl;
        }
    }
    # endif

    return 0;
}