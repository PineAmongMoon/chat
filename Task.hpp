/// \file Task.cpp
///
/// \author 俱源懋
/// \data 2019-04-01
///

#ifndef CHAT_TASK_HPP
#define CHAT_TASK_HPP

#include <functional>

#include <sys/select.h>

/// \copybrief chat.hpp
namespace chat {
    /// \brief 通用任务模版
    /// \tparam T
    /// 用于储存文件描述符的容器
    /// 要求可以使用基于范围的for循环
    template <class T> class Task;
}

template <class T>
class chat::Task {
    ///  用于输入的的文件描述符的集合
    const T in_fds;

    /// 用于输出的的文件描述符的集合
    const T out_fds;

    /// 任务的执行函数
    const std::function<int(const T, const T)> run_func;

public:
    /// 构造函数
    /// \param in_fds 用于输入的的文件描述符的集合
    /// \param out_fds 用于输出的的文件描述符的集合
    /// \param run_func 任务的执行函数
    Task(T in_fds, T out_fds, std::function<int(const T, const T)> && run_func):
        in_fds(in_fds), out_fds(out_fds), run_func(run_func) {};

    /// \brief 判断任务执行条件是否满足
    /// \param readable_fds 所有可读的文件描述符
    /// \param writable_fds 所有可写的文件描述符
    /// \return 若就绪 则返回true 否则返回false
    bool is_ready(const fd_set & readable_fds, const fd_set & writable_fds) const;

    /// \brief 执行任务
    /// \return 未定义
    int run() const;
};


template <class T>
bool chat::Task<T>::is_ready(const fd_set & readable_fds, const fd_set & writable_fds) const {
    for (int fd : in_fds) {
        if(! FD_ISSET(fd, &readable_fds)){
            return false;
        }
    }
    for (int fd : out_fds) {
        if(! FD_ISSET(fd, &writable_fds)){
            return false;
        }
    }
    return true;
}


template <class T>
int chat::Task<T>::run() const {
    return run_func(in_fds, out_fds);
}

#endif //CHAT_TASK_HPP
