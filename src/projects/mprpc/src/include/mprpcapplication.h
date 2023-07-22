#pragma once

// mprpc框架的初始化类
class MprpcApplication {
    public:
        static void Init(int argc, char **argv);

        static MprpcApplication& GetInstance();
    
    private:
        MprpcApplication();
        MprpcApplication(const MprpcApplication&) = delete;
        MprpcApplication(MprpcApplication&&) = delete;
};