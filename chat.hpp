/// \file chat.hpp
///
/// \author 俱源懋
/// \date 2019-04-01.
//

#ifndef CHAT_CHAT_HPP
#define CHAT_CHAT_HPP

#include <cstdio>

#include <sys/socket.h>

/// \brief 用于与其他函数做区分
namespace chat {

    /// 缓存大小
    const size_t buffer_size = 1500;

    /// 默认接收端口
    const int port = 60000;

    /// \brief 用于接收信息
    /// \param sock 接收信息的套接字
    /// \param out_fd 用于输出的文件描述符
    /// \param src 用于储存发送方地址
    /// \param socklen 用于储存发送方地址结构体的长度
    /// \return 接收的字节数
    int recv(int sock, int out_fd, sockaddr * src, socklen_t * socklen);

    /// \brief
    /// \param sock
    /// \param in_fd
    /// \param dest
    /// \return
    int send(int sock, int in_fd, const sockaddr * dest);
}

#endif //CHAT_CHAT_HPP
