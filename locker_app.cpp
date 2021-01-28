#include "locker.h"
#include <thread>


bool locker_thread_sock_running = false;

static Locker* plocker = new Locker();


static void locker_thread_sock(void)
{
    char* msg_buffer = new char[plocker->msg_buffer_size];
    CSampleProvider* pprovider = NULL;
    wchar_t* wUsername = NULL;
    wchar_t* wPassword = NULL;
    wchar_t* wDomain = NULL;

    while(locker_thread_sock_running)
       {
            // will block here since rcvfrom is blocking as default
            if (plocker->read_data(msg_buffer, plocker->msg_read_len, plocker->msg_buffer_size) == SOCKET_ERROR)
            {
                wprintf(L"locker_thread_sock read failed!\n");
            }
            // we received something 
            else
            {
                // process the received msg
                //TODO: filter the msg


                plocker->enable_locker_auto_login();

                // reenum credential
                pprovider = plocker->get_provider();
                if (pprovider == nullptr)
                {
                    continue;
                }
                pprovider->_EnumerateOneCredentials(NULL, wUsername, wPassword, wDomain);

                // notify the logon ui
                if ((pprovider->_cred_provider_events == nullptr) || (pprovider->_upAdviseContext == NULL))
                {
                    continue;
                }
                pprovider->_cred_provider_events->CredentialsChanged(pprovider->_upAdviseContext);
            }
       }
}

void init_locker_thread(void)
{
    locker_thread_sock_running = true;
    
    plocker->locker_sock_thread = new std::thread(locker_thread_sock);
}

void deinit_locker_thread(void)
{
    locker_thread_sock_running = false;
    //  TODO: send rsp success
}

bool is_locker_auto_login()
{
    return plocker->is_locker_auto_login();
}

#ifdef _WIN32

void set_provider_handle(CSampleProvider* pprovider)
{
    plocker->set_provider(pprovider);
}

#endif // _WIN32