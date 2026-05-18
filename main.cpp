#include "config.h"
#include "scheduler.h"
#include "ioscheduler.h"
#include "hook.h"
int main(int argc, char *argv[])
{
    //协程化（io调度，协程切换，hook）等
    sylar::set_hook_enable(true);
    //需要修改的数据库信息,登录名,密码,库名
    string user = "root";
    string passwd = "123456";
    string databasename = "yourdb";

    //命令行解析
    Config config;
    config.parse_arg(argc, argv);

    WebServer server;

    //初始化
    server.init(config.PORT, user, passwd, databasename, config.LOGWrite, 
                config.OPT_LINGER, config.TRIGMode,  config.sql_num,  config.thread_num, 
                config.close_log, config.actor_model);
    

    //日志
    server.log_write();

    //数据库
    server.sql_pool();

    //线程池
    server.thread_pool();

    //触发模式
    server.trig_mode();

    //监听
    server.eventListen();

    //运行
    //server.eventLoop();
    
    // 协程模式启动
    sylar::IOManager iom;
    iom.scheduleLock([&](){
        server.eventLoop();
    });
    iom.start();

    return 0;
}
