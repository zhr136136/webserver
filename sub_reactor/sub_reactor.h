#ifndef SUB_REACTOR_H
#define SUB_REACTOR_H

#include <sys/epoll.h>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>

class SubReactor {
public:
    SubReactor();
    ~SubReactor();

    void start();                // 启动从Reactor线程
    void add_client_fd(int fd);  // 加入客户端连接

private:
    void loop();                 // 事件循环
    static void* run(void* arg); // 线程入口

    int m_epollfd;
    bool m_running;
    static const int MAX_EVENTS = 1024;
};

// 全局管理从Reactor
extern std::vector<SubReactor> g_sub_reactors;
int get_next_sub_reactor_idx();

#endif
