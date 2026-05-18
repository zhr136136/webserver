CXX ?= g++

DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CXXFLAGS += -g
else
	CXXFLAGS += -O2
endif

# 协程库必须的 C++17 标准
CXXFLAGS += -std=c++17

# 协程库头文件路径
INCLUDES = -I../coroutine-lib-main/fiber_lib/1thread \
	-I../coroutine-lib-main/fiber_lib/2fiber \
	-I../coroutine-lib-main/fiber_lib/3scheduler \
	-I../coroutine-lib-main/fiber_lib/4timer \
	-I../coroutine-lib-main/fiber_lib/5iomanager \
	-I../coroutine-lib-main/fiber_lib/6hook

# 编译规则（注意：下面这行命令开头是一个 Tab 键）
server: main.cpp ./timer/lst_timer.cpp ./http/http_conn.cpp ./log/log.cpp ./CGImysql/sql_connection_pool.cpp webserver.cpp config.cpp
	$(CXX) -o server $^ $(CXXFLAGS) $(INCLUDES) -lpthread -lmysqlclient -L./ -lcoroutine

# 清理规则（下面这行开头也是一个 Tab 键）
clean:
	rm -f server
