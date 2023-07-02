#pragma once

#include <iostream>
#include <string>

// 时间类
class Timestamp {
    public:
        Timestamp();
        explicit Timestamp(int64_t microSecondSinceEpoch);
        static Timestamp now();
        std::string toString() const;
    private:
        int64_t microSecondSinceEpoch_;
};

