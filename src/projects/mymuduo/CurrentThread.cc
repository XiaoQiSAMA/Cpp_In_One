#include "CurrentThread.h"

namespace CurrentThread
{
    __thread int t_cachedTid = 0;

    void cacheTid() {
        if (t_cachedTid == 0) {
            // linux系统获取当前线程的pid
            t_cachedTid = static_cast<pid_t>(::syscall(SYS_gettid));
        }
    }
} // namespace CurrentThread
