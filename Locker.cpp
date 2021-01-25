#include "locker.h"
#include<iostream>


#ifdef _WIN32

#include <windows.h>
#pragma comment(lib, "ws2_32.lib")

#endif // _WIN32

using namespace std;
 
Locker::Locker()
{
    SOCKADDR_IN sock_local_if;
    USHORT nPort = 55320;

    this->last_locked_time = clock();
    this->current_unlock_time = clock();
    this->locker_state = Locker_state_t ::LOCKER_LOCKED;
    this->locker_socket = INVALID_SOCKET;

#ifdef _WIN32

    // Declare and initialize variables
    WSADATA wsaData = { 0 };
    int iResult = 0;    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        wprintf(L"WSAStartup failed: %d\n", iResult);
    }

#endif // _WIN32

    this->locker_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (this->locker_socket == INVALID_SOCKET)
        wprintf(L"socket function failed with error = %d\n", WSAGetLastError());
    
    sock_local_if.sin_family = AF_INET;
    sock_local_if.sin_port = htons(nPort);
    sock_local_if.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(bind(this->locker_socket,(SOCKADDR *) &sock_local_if, sizeof(sock_local_if)) == SOCKET_ERROR)
        wprintf(L"bind Locker socket failed: %d\n", iResult);

}


Locker::~Locker()
{

    if(this->locker_socket)
    {
        closesocket(this->locker_socket);

#ifdef _WIN32
        
        WSACleanup();

#endif // _WIN32

    }
}


int Locker::read_data(char *buffer, unsigned int len, unsigned int size)
{
    int nRet = 0;
    SOCKADDR_IN sock_remote;
    int addr_size = sizeof(sock_remote);
    if(len > size)
    {
        wprintf(L"read buffer overflow \n");
        return ERROR;
    }

    if(!buffer)
    {
        wprintf(L"buffer is NULL \n");
        return ERROR;
    }

    nRet = recvfrom(this->locker_socket,buffer,len, 0, (SOCKADDR*)&sock_remote,&addr_size);
    //  TODO:
    //      log sock_remote
    return nRet;   
}

int Locker::write_data(char* buffer, unsigned int len, unsigned int size)
{
    int nRet = 0;
    if (len > size)
    {
        wprintf(L"write buffer overflow \n");
        return ERROR;
    }

    if (!buffer)
    {
        wprintf(L"buffer is NULL \n");
        return ERROR;
    }

    // TODO:
    //      send rsp data
    return nRet;
}








