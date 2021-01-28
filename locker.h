#pragma once

#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<thread>


#ifdef _WIN32

//  The order is impordant, because "CSampleProvider.h" include <windows.h>
//  And <windows.h> include <Winsock.h>
#include<WinSock2.h>
#include "CSampleProvider.h"

#endif // _WIN32

#define INIT_MSG_BUFFER_SIZE         (1*1024)
#define INIT_MSG_READ_LEN            (512)

enum class Locker_state_t
{
    LOCKER_UNLOCKED =1,
    LOCKER_LOCKED,
    LOCKER_MAX_STATE
};


class Locker
{
private:

    //    last time locked
    clock_t last_locked_time;

    //    current unlock time
    clock_t current_unlock_time;

    //    locker state
    Locker_state_t locker_state;

    //    socket handle
    SOCKET locker_socket;

    //    whether auto log in
    bool locker_auto_log_in;

#ifdef _WIN32

    CSampleProvider* pprovider;

#endif // _WIN32


public:
    //    locker construct
    Locker();
    ~Locker();

    //    read data to buffer
    int read_data(char* buffer, unsigned int len, unsigned int buf_size);

    //    write data in buffer
    //    len real data length
    //    bug_size size of buffer
    int write_data(char* buffer, unsigned int len, unsigned int buf_size);


    bool is_locker_auto_login(void);

#ifdef _WIN32
    //  Set provider handle
    void set_provider(CSampleProvider* pprovider);

    //  Get provider handle
    CSampleProvider* get_provider(void);

#endif

    void enable_locker_auto_login(void);
    void disable_locker_auto_login(void);

    //    locker sock thread pointer
    std::thread* locker_sock_thread;
    
    //    queue for socket rcv msg
    
    //   msg_buffer size 
    size_t msg_buffer_size;

    //  msg read len in one rcv
    size_t msg_read_len;
};