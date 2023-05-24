#include <iostream>
#include <memory>
using namespace std;

class VideoSite{
    public:
        virtual void freeMovie() = 0;
        virtual void vipMovie() = 0;
        virtual void ticketMovie() = 0;
};

// 被用来作代理行为
class FixBugVideoSite : public VideoSite {  // 委托类
    public:
        virtual void freeMovie() {
            cout << "Watch movie free" << endl;
        }
        virtual void vipMovie() {
            cout << "Watch movie with VIP" << endl;
        }
        virtual void ticketMovie() {
            cout << "Watch movie with ticket" << endl;
        }
};

// 代理类，代理FixBugVideoSite
class FreeVideoSiteProxy : public VideoSite {
    // 代理类由于权限管理，仅代表当前权限所能使用委托类中的部分行为
    public:
        FreeVideoSiteProxy() { pVideo = new FixBugVideoSite();}
        ~FreeVideoSiteProxy() { delete pVideo;}
        virtual void freeMovie() {
            pVideo->freeMovie();
        }
        virtual void vipMovie() {
            cout << "Can not watch VIP movies!" << endl;
        }
        virtual void ticketMovie() {
            cout << "Can not watch ticket movies!" << endl;
        }
    private:
        // 指针指向委托类，根据权限情况执行
        VideoSite *pVideo;
};

// 代理类，代理FixBugVideoSite
class VIPVideoSiteProxy : public VideoSite {
    // 代理类由于权限管理，仅代表当前权限所能使用委托类中的部分行为
    public:
        VIPVideoSiteProxy() { pVideo = new FixBugVideoSite();}
        ~VIPVideoSiteProxy() { delete pVideo;}
        virtual void freeMovie() {
            pVideo->freeMovie();
        }
        virtual void vipMovie() {
            pVideo->vipMovie();
        }
        virtual void ticketMovie() {
            cout << "Can not watch ticket movies!" << endl;
        }
    private:
        // 指针指向委托类，根据权限情况执行
        VideoSite *pVideo;
};

// 使用的是基类的引用，是通用的API接口
void watchMovie(unique_ptr<VideoSite> &ptr){
    ptr->freeMovie();
    ptr->vipMovie();
    ptr->ticketMovie();
}

int main(){
    unique_ptr<VideoSite> p1(new FreeVideoSiteProxy());
    unique_ptr<VideoSite> p2(new VIPVideoSiteProxy());
    
    watchMovie(p1);
    watchMovie(p2);

    return 0;
}