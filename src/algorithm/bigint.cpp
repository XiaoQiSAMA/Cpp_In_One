#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

class BigInt {
    public:
        BigInt(const string str): stringDict(str) {}
    private:
        string stringDict;
        friend ostream& operator<<(ostream& out, const BigInt &src);
        friend BigInt operator+(const BigInt &lhs, const BigInt &rhs);
        friend BigInt operator-(const BigInt &lhs, const BigInt &rhs);
};

ostream& operator<<(ostream& out, const BigInt &src) {
    out << src.stringDict;
    return out;
}

BigInt operator+(const BigInt &lhs, const BigInt &rhs) {
    /*
    遍历字符串l,r 从后往前遍历
    同位相加，进位flag置True，存入res中
    某个字符先完成，仍要考虑进位
    */
   string result;
   bool flag = false;
   int i = lhs.stringDict.length() - 1;
   int j = rhs.stringDict.length() - 1;

   for (; i >= 0 && j >= 0; --i, --j) {
        int ret = lhs.stringDict[i] - '0' + rhs.stringDict[j] - '0';
        if (flag) {
            ret += 1;
            flag = false;
        }
        if (ret >= 10){
            ret = ret % 10;
            flag = true;
        }
        result.push_back(ret + '0');
   }

   // i > j
   if (i >= 0) {
        while (i >= 0){
            int ret = lhs.stringDict[i] - '0';
            if (flag) {
                ret += 1;
                flag = false;
            }
            if (ret >= 10){
                ret = ret % 10;
                flag = true;
            }
            result.push_back(ret + '0');
            --i;
        }
   }

   // i < j
   if (j >= 0) {
        while (j >= 0){
            int ret = rhs.stringDict[j] - '0';
            if (flag) {
                ret += 1;
                flag = false;
            }
            if (ret >= 10){
                ret = ret % 10;
                flag = true;
            }
            result.push_back(ret + '0');
            --j;
        }
   }

   if (flag){
        result.push_back('1');
   }

   reverse(result.begin(), result.end());
   return result;
}

BigInt operator-(const BigInt &lhs, const BigInt &rhs) {
    /*
    找大的字符串左减数，小的做被减数
    遍历两个字符串，减法，借位(flag)，string result
    */
    string result;
    bool flag = false;
    bool minor = false;  // 结果是否添加负号
    
    string MaxStr = lhs.stringDict;
    string MinStr = rhs.stringDict;
    
    if (MaxStr.length() < MinStr.length()){
        MaxStr = rhs.stringDict;
        MinStr = lhs.stringDict;
        minor = true;
    } else if (MaxStr.length() == MinStr.length()) {
        if (MaxStr < MinStr) {
            MaxStr = rhs.stringDict;
            MinStr = lhs.stringDict;
            minor = true;
        } else if (MaxStr == MinStr) {
            return string("0");
        }
    }
    
    int i = MaxStr.length() - 1;
    int j = MinStr.length() - 1;

    for (; i>=0 && j >= 0; --i, --j) {
        int ret = MaxStr[i] - MinStr[j];
        if (flag) {
            ret -= 1;
            flag = false;
        } 

        if (ret < 0) {
            ret += 10;
            flag = true;
        }
        result.push_back(ret + '0');
    }

    while (i >= 0) {
        int ret = MaxStr[i] - '0';
        if (flag) {
            ret -= 1;
            flag = false;
        }
        
        if (ret < 0) {
            ret += 10;
            flag = true;
        }
        result.push_back(ret + '0');
        --i;
    }

    string retStr;
    auto it = result.rbegin();
    for (; it != result.rend(); ++it) {
        if (*it != '0') {
            break;
        }
    }
    for (; it != result.rend(); ++it) {
        retStr.push_back(*it);
    }

    if (minor) {
        retStr.push_back('-');
    }

    // reverse(retStr.begin(), retStr.end());
    return retStr;
}



int main(){
    BigInt int1("123");
    BigInt int2("12");
    BigInt int3("120");

    cout << int2 + int1 << endl;

    cout << int1 - int3 << endl;

    return 0;
}