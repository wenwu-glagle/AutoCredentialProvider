#pragma once

#include"locker.h"

void init_locker_thread(void);
void deinit_locker_thread(void);
void set_provider_handle(CSampleProvider* pprovider);