#include "sub_reactor.h"
#include "webserver.h"
#include "http_conn.h"
#include <cstring>

std::vector<SubReactor> g_sub_reactors;
static int g_round_robin = 0;

SubReactor::SubReactor() {
    m_epollfd = epoll_create(MAX_EVENTS);
    m_running = false;
}

SubReactor::~SubReactor() {
    close(m_epollfd);
}

void SubReactor::start() {
    m_running = true;
    pthread_t tid;
    pthread_create(&tid, nullptr, run, this);
}

void* SubReactor::run(void* arg) {
    SubReactor* r = (SubReactor*)arg;
    r->loop();
    return nullptr;
}

// 设置非阻塞
static void set_nonblock(int fd) {
    int fl = fcntl(fd, F_GETFL);
    fcntl(fd, F_SETFL, fl | O_NONBLOCK);
}

// 加入客户端fd到从Reactor
void SubReactor::add_client_fd(int fd) { 
    //addfd(m_epollfd, fd, true, 1);
    set_nonblock(fd);
    epoll_event ev{};
    ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
    ev.data.fd = fd;
    epoll_ctl(m_epollfd, EPOLL_CTL_ADD, fd, &ev);
}

// 从Reactor核心循环（处理所有客户端读写）
void SubReactor::loop() {
    epoll_event events[MAX_EVENTS];
    while (m_running) {
        int n = epoll_wait(m_epollfd, events, MAX_EVENTS, -1);

        for (int i = 0; i < n; ++i) {
            int sockfd = events[i].data.fd;
            uint32_t e = events[i].events;

            if (e & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
                http_conn::deal_timer(sockfd);
                continue;
            }
            if (e & EPOLLIN) {
                WebServer::dealwithread(sockfd);
            }
            if (e & EPOLLOUT) {
                WebServer::dealwithwrite(sockfd);
            }
        }
    }
}

// 轮询选择从Reactor
int get_next_sub_reactor_idx() {
    return g_round_robin++ % g_sub_reactors.size();
}
