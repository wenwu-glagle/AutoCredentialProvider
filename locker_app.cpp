#include "locker.h"
#include "locker_app.h"
#include <thread>
#include <tuple>
#include <string>
#include <chrono>
#include "util.h"

bool locker_thread_sock_running = false;

static Locker* plocker = new Locker();


static void locker_thread_sock(void)
{
    char* msg_buffer = new char[plocker->msg_buffer_size];
    CSampleProvider* pprovider = NULL;
    std::string Username;
    std::string Password;
    std::string Domain;
  
    while(locker_thread_sock_running)
       {
            odprintf("enter locker_thread_sock \n");
            plocker->disable_locker_auto_login();

            // will block here since rcvfrom is blocking as default
            // rcvfrom reading data from internal queue, may cause some error, like the older msg
            if (plocker->read_data(msg_buffer, plocker->msg_read_len, plocker->msg_buffer_size) == SOCKET_ERROR)
            {
                odprintf("locker_thread_sock read failed!\n");
            }
            // we received something 
            else
            {
                // process the received msg
                // TODO: filter the msg
                // TODO: compare current received msg, if is same with previous that failed,drop it and continue

                Password = std::string(msg_buffer);
           
                plocker->enable_locker_auto_login();

#ifdef _WIN32
                // reenum credential
                pprovider = plocker->get_provider();
                if (pprovider == nullptr)
                {
                    continue;
                }

                

                pprovider->_ReEnumerateOneCredentials(Username, Password, Domain);

                // notify the logon ui
                if ((pprovider->_cred_provider_events == nullptr) || (pprovider->_upAdviseContext == NULL))
                {
                    continue;
                }
                pprovider->_cred_provider_events->CredentialsChanged(pprovider->_upAdviseContext);
#endif
            }

            // sleep for some time everytime received msg
            // this works well when receiving with high frequency
            std::this_thread::sleep_for(std::chrono::seconds(15));
       }
}

void init_locker_thread(CSampleProvider* pprovider)
{
    locker_thread_sock_running = true;
    set_provider_handle(pprovider);

    plocker->plocker_sock_thread = new std::thread(locker_thread_sock);

    //  TODO: start soft ap 
}

void deinit_locker_thread(void)
{
    locker_thread_sock_running = false;
    //  TODO: send rsp success with confirmable

    //  TODO: stop soft ap 
}

bool is_locker_auto_login()
{
    return plocker->is_locker_auto_login();
}

#ifdef _WIN32

static void set_provider_handle(CSampleProvider* pprovider)
{
    plocker->set_provider(pprovider);
}

#endif // _WIN32