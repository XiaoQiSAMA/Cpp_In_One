#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>
#include <ctime>
using namespace std;

template <typename Container>
void showContainer(Container &con){
    typename Container::iterator it = con.begin();
    for (; it != con.end(); ++it){
        cout << *it << " ";
    }
    cout << endl;
}

template <typename Compare, typename T>
class _mybind1st{
    public:
        _mybind1st(Compare cmp, T val): _cmp(cmp), _val(val) {}
        bool operator()(const T &second){
            return _cmp(_val, second);  // greater
        }
    private:
        Compare _cmp;
        T _val;
};

template <typename Compare, typename T>
_mybind1st<Compare, T> mybind1st(Compare cmp, const T &val){
    return _mybind1st<Compare, T>(cmp, val);
}

template <typename Iterator, typename Compare>
Iterator my_find_if(Iterator first, Iterator last, Compare cmp){
    for (; first != last; ++first){
        if (cmp(*first)){   // cmp.operator()(*first)
            return first;
        }
    }
    return last;
}


int main(){
    vector<int> vec;
    srand(time(nullptr));
    for (int i = 0; i < 20; ++i){
        vec.push_back(rand() % 100 + 1);
    }

    showContainer(vec);
    sort(vec.begin(), vec.end());   // 默认从小到大
    showContainer(vec);

    // greater是二元函数对象
    sort(vec.begin(), vec.end(), greater<int>());
    showContainer(vec);

    /*
    把70按顺序插入到vec容器中   找到第一个小于70的数
    需要两个参数
    greater(left a, right b):   a > b
    less(left a, right b):      a < b

    绑定器 + 二元函数对象 ==> 一元函数对象
    bind1st + greater bool operator() (70, const _Ty& _Right)
    bind2st + less bool operator() (const _Ty& _Left, 70)
    */

    // auto it1 = find_if(vec.begin(), vec.end(), 
    //     bind1st(greater<int>(), 70));
    // auto it1 = find_if(vec.begin(), vec.end(), 
    //     bind2nd(less<int>(), 70));

    // 底层实现绑定器
    auto it1 = my_find_if(vec.begin(), vec.end(), mybind1st(greater<int>(), 70));

    if (it1 != vec.end()){
        vec.insert(it1, 70);
    }
    showContainer(vec);

    return 0;
}