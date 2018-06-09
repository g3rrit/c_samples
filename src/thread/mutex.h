#ifndef MUTEX_H
#define MUTEX_H

#include "mstd.h"

//-----WINDOWS-----
#ifdef _WIN32

#define mutex_declare(name)         HANDLE name

#define mutex_init(name)            name = CreateMutex(0, FALSE, 0);     \
                                    if(!name)                            \
                                        set_error(254);                     

#define mutex_delete(name)          CloseHandle(name);


#define mutex_lock(name)            WaitForSingleObject(name, INFINITE);

#define mutex_unlock(name)          ReleaseMutex(name);


#define sleep_ms(milliseconds)      Sleep(milliseconds);


#else
//-----UNIX--------
#include <time.h>   

#define mutex_declare(name)         pthread_mutex_t name

#define mutex_init(name)            if(pthread_mutex_init(&name, 0))     \
                                        set_error(254);                     

#define mutex_delete(name)          if(pthread_mutex_destroy(&name))     \
                                        set_error(255);                     

#define mutex_lock(name)            pthread_mutex_lock(&name);

#define mutex_unlock(name)          pthread_mutex_unlock(&name);


#define sleep_ms(milliseconds)      struct timespec _ts;                                \
                                    _ts.tv_sec = milliseconds / 1000;                   \
                                    _ts.tv_nsec = (milliseconds % 1000) * 1000000;      \
                                    nanosleep(&ts, 0);


#endif


#endif
