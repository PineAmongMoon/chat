/// \file main.cpp
///
/// \author 俱源懋
/// \data 2019-04-01
///
/// \brief 程序入口
///


#include <cstdio>
#include <cerrno>
#include <vector>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>

#include "chat.hpp"
#include "Task.hpp"

using std::vector;


/// \brief 将套接字设置为非阻塞状态
/// \param sock 要修改的套接字
/// \return
/// 返回套接字的fcntl的标志位
/// 若有错,误则返回-1,并设置erron
int set_noblock(int sock);

/// \brief 错误处理
/// \param err 用于提示错误的字符串
void error_handle(const char * err);

int set_noblock(int sock) {
    int flgs = fcntl(sock, F_GETFL);
    if (flgs == -1)
        return -1;
    return fcntl(sock, F_SETFL, flgs | O_NONBLOCK);
}

/// \details
/// 调用perror报告错误并结束程序
void error_handle(const char * err) {
    perror(err);
    exit(errno);
}

/// main函数
int main(int args, char * argv[]) {
    int srv_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(srv_sock == -1)
        error_handle("socket create error");

    sockaddr_in srv_addr;
    memset(&srv_addr, 0, sizeof(sockaddr_in));
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_port = htons(chat::port);

    if (bind(srv_sock, (sockaddr*)&srv_addr, sizeof(sockaddr_in)) == -1)
        error_handle("bind error");

    if(set_noblock(srv_sock) == -1)
        error_handle("set non-block error");


    int clt_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(clt_sock == -1)
        error_handle("socket create error");
    if (set_noblock(clt_sock) == -1)
        error_handle("set non-block error");


    sockaddr_in des;
    memset(&des, 0, sizeof(sockaddr_in));
    des.sin_family = AF_INET;
    des.sin_port = htons(chat::port);
    des.sin_addr.s_addr = inet_addr(argv[1]);

    vector<chat::Task<vector<int>>> tasks {
        {
            {srv_sock}, {STDOUT_FILENO},
            [=](vector<int> in_fds, vector<int> out_fds)->int
            {return chat::recv(in_fds[0], out_fds[0], nullptr, nullptr);}
        },
        {
            {STDIN_FILENO}, {clt_sock},
            [=](vector<int> in_fds, vector<int> out_fds)->int
            {return chat::send(out_fds[0], in_fds[0], (sockaddr*)(&des));}
        }
    };

    fd_set read_fds, write_fds;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);

    timeval time{0, 500};
    
	int s;
	for(;;) {
        FD_SET(srv_sock, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);

        FD_SET(clt_sock, &write_fds);
        FD_SET(STDOUT_FILENO, &write_fds);

        s = select(1 + std::max(srv_sock, clt_sock), &read_fds, &write_fds, nullptr, &time);
		if (s < 0)
			error_handle("select error");
        if (s != 0) {
            for (auto &task : tasks) {
                if (task.is_ready(read_fds, write_fds))
                    if (task.run() == 0)
                        return 0;
            }
        }
    }
}