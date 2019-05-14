/// \file chat.cpp
///
/// \author 俱源懋
/// \date 2019-04-01
///
/// \brief 程序核心部分
///

#include "chat.hpp"

#include <unistd.h>

/// \namespace chat
/// \headerfile chat.hpp
int chat::recv(int sock, int out_fd, sockaddr * src, socklen_t * socklen) {
    static char buffer[buffer_size];
    size_t len;
    len = recvfrom(sock, buffer, buffer_size, 0, src, socklen);
    return write(out_fd, buffer, len);
}

/// \namespace chat
/// \headerfile chat.hpp
int chat::send(int sock, int in_fd, const sockaddr * dest) {
    static char buffer[buffer_size];
    size_t len;
    len = read(in_fd, buffer, buffer_size);
    return sendto(sock, buffer, len, 0, dest, sizeof(sockaddr));
}

