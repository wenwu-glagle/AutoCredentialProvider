#pragma once

#include<cstdio>
#include<cstdlib>
#include<ctime>

#include<WinSock2.h>


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

    

};