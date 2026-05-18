// 1. 初始化 4 个从Reactor（CPU核心数）
    g_sub_reactors.resize(4);
    for (auto& r : g_sub_reactors) r.start();
    while (true)
    {
        if (m_timer->get_tw_slot()) {
            m_timer->tick();
        }
        int num = epoll_wait(m_epollfd, m_events, MAX_EVENT_NUMBER, -1);
        for (int i = 0; i < num; i++) {
            int sockfd = m_events[i].data.fd;
            // 主Reactor 只处理 2 件事！
            // 1. 新连接 accept
            // 2. 信号处理
            // 【1】新连接 → 交给从Reactor（核心改造点）
            if (sockfd == m_listenfd) {
                bool ret = dealclientdata();
                if (!ret) continue;
                // 关键：不加入主线程epoll，直接分给从Reactor
                int idx = get_next_sub_reactor_idx();
                g_sub_reactors[idx].add_client_fd(m_clientfd);
                continue;
            }
            // 【2】信号处理（保留不变）
            else if ((sockfd == m_pipefd[0]) && (m_events[i].events & EPOLLIN)) {
                dealwithsignal(ret, stop_server);
            }
        }
    }
    
    
    //http_conn::init  ------- addfd(m_epollfd, sockfd, true, m_TRIGMode);需要删除
    //sub_reactor.cpp --------仅保留一行addfd(m_epollfd, fd, true, 1);
    //WebServer::dealclientdata_real------取消注释
