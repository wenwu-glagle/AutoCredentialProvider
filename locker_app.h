#pragma once

#include "CSampleProvider.h"
#include <string>

#define MAX_CREDENTIALS                (1)



void init_locker_thread(CSampleProvider* pprovider);
void deinit_locker_thread(void);

#ifdef _WIN32

static void set_provider_handle(CSampleProvider* pprovider);

#endif // _WIN32


