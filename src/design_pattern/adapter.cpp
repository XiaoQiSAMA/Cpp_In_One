#include <iostream>
#include <memory>
using namespace std;

// 电视投影接口场景

class VGA { // VGA接口类
    public:
        virtual void play() = 0;
};

// 只支持VGA接口的投影仪
class TV01 : public VGA {
    public:
        void play() {
            cout << "VGA linked" << endl;
        }
};

// 电脑类，只支持VGA接口
class Mac {
    public:
        // 只支持VGA类接口的指针/引用
        void playVideo(VGA *pVGA) {
            pVGA->play();
        }
};

// HDMI接口的投影仪
class HDMI {
    public:
        virtual void play() = 0;
};

// 只支持HDMI接口的投影仪
class TV02 : public HDMI {
    public:
        void play() {
            cout << "HDMI linked" << endl;
        }
};

// 适配器类
// 只能接收VGA接口的电脑，构建一个接收HDMI信号的指针，用来适配HDMI接口
// class VGAToHDMIAdapter : public VGA, public HDMI {
class VGAToHDMIAdapter : public VGA {
    public:
        VGAToHDMIAdapter(HDMI *p): pHDMI(p) {}
        void play() {
            pHDMI->play();
        }
    private:
        HDMI *pHDMI;
};

int main(){
    Mac mac;
    // mac通过投影仪，再经过VGA接口播放视频
    mac.playVideo(new TV01());

    mac.playVideo(new VGAToHDMIAdapter(new TV02()));

    return 0;
}